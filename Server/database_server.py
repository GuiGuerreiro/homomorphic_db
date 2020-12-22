# -*- coding: utf-8 -*-
"""
CSC PROJECT - Guilherme Guerreiro, Joaquim Inácio, João Marques
"""
import os
import pathlib
import time
import re
from glob import glob


#The path where this program is located
current_path = pathlib.Path(__file__).parent.absolute()

##################################
#Get the client number and push the msg.enc file to this program folder
#################################################

def get_name_from_type(path_to_file):
    
    command_sender = path_to_file[::-1]
    command_sender = command_sender.split("/",1)[0]
    enc_filename = command_sender[::-1]
    
    name = enc_filename.split(".")[0]
    
    return name
    
    
#Returns the number of existing clients
def number_of_clients():
    
    #Get the path of the destination folder
    path_to_dir = str(current_path.parent) +'/app/users'
    
    #Gives all the subdirectoriesof the users folder
    #x[0] contains the parent - users folder
    aux = [x[0] for x in os.walk(path_to_dir)]
    
    client_num = 0
    for i in aux:
        client_num+=1
    
    return client_num


#Create table in DB
def create_table(command,client,command_path):
    
    #Get tablename
    tablename = command.split (" ")[2]
    
    path_to_db = str(current_path.parent) + '/app/db/'
    
    all_tables = next(os.walk(path_to_db))[1]
    
    for table in all_tables:
        #If there is an existing table with the same name
        if table == tablename:
            answer_client(("This table already exists! New table was not created!"),client)
            #os.remove(command_path)
            return -1
    
    #Creates new table in the database
    os.system('sudo mkdir ../app/db/%s '%tablename)
    
    #Get part of the columns
    columns_part = command.split (" ")[3]
    
    #Isolate each column
    all_columns = re.sub('[()]', '', columns_part)
    all_columns = all_columns.split(",")
    
    i = 0
    for column in all_columns:
         os.system('sudo mkdir ../app/db/%s/%s ' %(tablename,all_columns[i]))
         i+=1
    
    
    return 0

#Gives the client the answer to his/hers request
def answer_client(msg,client):
    
    filename = "answer.txt"
    final_name = "answer.enc"
    #Creates the txt message    
    path_now = str(current_path.parent) + '/app/server/' + client + '/' + filename
    path_than =  str(current_path.parent) + '/app/server/' + client + '/' + final_name
    destination_path = str(current_path.parent) + '/app/users/' + client + '/'
    f = open(path_now, 'w')
    
    f.write(msg)
    
    f.close()    
    
    os.system('sudo ./encrypt %s answer.txt' %client)
    os.system('sudo mv %s %s' %(path_than,destination_path))
    print('\n')
    
############################
#Returns he columns in the command
############################
def get_columns_list(command, position):
    
    column_list = command.split (" ")[position]
    all_columns = re.sub('[()]', '', column_list)
    all_columns = all_columns.split(",")
    
    return all_columns


#Get the available line to input new data
def get_input_line(column_path):
    
    pattern = "*.hmf"
    inserted_lines = []
    
    for dir,_,_ in os.walk(column_path):
        inserted_lines.extend(glob(os.path.join(dir,pattern))) 
    
    #If there are already inserts in the table
    i = 1
    if inserted_lines:
        #lets see the minimum line number available
        for line in inserted_lines:
            
            line_num = get_name_from_type(line)
            #If there is already this line, passes to the next one
            if i == int(line_num):
                i+=1
                continue
            else:
                return i
        return i
        
    else:
        return 1
        

################################
#When there is as undesired input, it is moved to the trash
##############################
        
def delete_bad_input(path_to_values):
    pattern1 = "*.bin"
    pattern2 = "*.hmf"
    
    #Remove the .bin bad files
    bad_values = []
    for dir,_,_ in os.walk(path_to_values):
        bad_values.extend(glob(os.path.join(dir,pattern1))) 
        
    for value in bad_values:
        os.system('sudo rm -r %s' %value)
        
    bad_values = []
    for dir,_,_ in os.walk(path_to_values):
        bad_values.extend(glob(os.path.join(dir,pattern2))) 
    
    for value in bad_values:
        os.system('sudo rm -r %s' %value)
    
    print('All bad files were deleted')
    
    
    
############################
#Deals with table inserts
############################
def insert_into_table(command,client,path_to_command):
    
    

    tablename = command.split (" ")[2]
    all_columns = get_columns_list(command, 3)
    

    
    ##Needed paths
    path_to_db = str(current_path.parent) + '/app/db/'
    path_to_table = path_to_db + tablename + '/'
    path_to_values = str(current_path.parent) + '/app/server/' + client +'/'
    
    all_tables = next(os.walk(path_to_db))[1]

    
    found_table = -1
    #Searches the table where there will be an insert
    for table in all_tables:
        if tablename == table:
            found_table = 0
            #When the table is found, it will search the columns
            all_db_columns = next(os.walk(path_to_table))[1]
            #For each db_column
            for i in range(len(all_db_columns)):
                
                db_column = all_db_columns[i]
                new_column = all_columns[i]
                #The columns correspond to 
                if new_column == db_column:
                        
                    #Path to the new column encrypted values
                    path_to_hmf = path_to_values + new_column + '.hmf'
                    path_to_bin = path_to_values + new_column + '.bin'
                    #Path to the db correct column
                    destination_path = path_to_table + new_column + '/'
                    
                    table_line = get_input_line(destination_path)
                    
                     #Moves the encrypted.hmf to the column
                    os.system('sudo mv %s %s' %(path_to_hmf, destination_path))
                    
                    #Moves the encrypted .bin to the server
                    os.system('sudo mv %s %s' %(path_to_bin, destination_path))
                    #Rename the new values
                    os.rename((destination_path + new_column + '.hmf'), (destination_path + str(table_line) + '.hmf'))
                    os.rename((destination_path + new_column + '.bin'), (destination_path + str(table_line) + '.bin'))
                else:
                    return -1
            
            #Evrything goes fine
            return 0
    
    #If the specified table do not exist                 
    if found_table ==-1:
        print('Table not found')
        delete_bad_input(path_to_values)
        return -1
    
    #Everything goes ok, return 0 to the flag
    return 0
    

def verify_line_exists(command,client):
    line_num =  int( command.split (" ")[2])
    tablename = command.split (" ")[4]
    
    path_to_db = str(current_path.parent) +'/app/db/'
    path_to_table = path_to_db + tablename + '/'
    
    all_tables = next(os.walk(path_to_db))[1]
    
    
    pattern = "*.hmf"
    inserted_lines = []
    
    line_found = -1
    found_table = 0
    for table in all_tables:
        if tablename == table:
            found_table=1
            all_db_columns = next(os.walk(path_to_table))[1]
            
            for column in all_db_columns:
                column_path = path_to_table + column + '/'
                for dir,_,_ in os.walk(column_path):
                    inserted_lines.extend(glob(os.path.join(dir,pattern)))
                
                #If there are inputs in the table
                
                if inserted_lines:
                    for line in inserted_lines:
                        aux = get_name_from_type(line)
                        
                        if int(aux) == line_num:
                            #Line exists
                            for line_col in all_db_columns:
                                path_to_client = str(current_path.parent) +'/app/users/' + client + '/'
                                path_of_line_value = path_to_table = path_to_db + tablename + '/' + line_col +'/'+ str(line_num) + '.hmf'
                                old_name = path_to_table = str(current_path.parent) +'/app/users/' + client + '/'+ str(line_num) + '.hmf'
                                new_name = path_to_table = str(current_path.parent) +'/app/users/' + client + '/'+ line_col + '.hmf'
                                
                                os.system('sudo cp %s %s' %(path_of_line_value,path_to_client))
                                os.system('sudo mv %s %s' %(old_name,new_name))
                            return 0
                    
                    if line_found == -1:
                        return -1
                        
                #No found lines
                else:
                    return -1
            #No valid line
            else:
                return -1
                        
    
    #If the table was not found
    if found_table == 0:
        return -1
            
    return 0


def delete_line(command):
    
    line_num =  int( command.split (" ")[1])
    tablename = command.split (" ")[3]
    deleted_line = -1
    
    path_to_db = str(current_path.parent) +'/app/db/'
    path_to_hmf_inc = path_to_db + tablename + '/'
    path_to_bin_inc = path_to_db + tablename + '/'
    
    all_tables = next(os.walk(path_to_db))[1]
    
    for table in all_tables:
        if table == tablename:
            #Add the table to the path
            path_to_columns = str(current_path.parent) +'/app/db/' + table + '/'
            
            all_columns = next(os.walk(path_to_columns))[1]
            
            for column in all_columns:
                
                #Removes the files that correspond to the line
                path_to_hmf = path_to_hmf_inc + column + '/' + str(line_num) + '.hmf'
                path_to_bin = path_to_bin_inc + column + '/' + str(line_num) + '.bin'
                
                os.system('sudo rm -r %s' %path_to_hmf)
                os.system('sudo rm -r %s' %path_to_bin)
                deleted_line = 0
                
            #Everything goes ok
            return 0
            
    
    #If there are no columns or lines to delete
    if deleted_line == -1:
        return -1
    
    
    pattern = "*.hmf"
    inserted_lines = []
    
    found_table = -1
    for table in all_tables:
        if tablename == table:
            found_table = 0
            
    #If the table was not found
    if found_table == -1:
        return -1
            
    
    return 0



def operations_in_queries(operator, path_to_results, path_db,client,current_path):
    
    #./operation mult file1 file2
    destination_path = str(current_path.parent) + '/app/users/' + client
    value_bin_path = str(current_path.parent) + '/app/server/' + client + '/value.bin'
    value_hmf_path = str(current_path.parent) + '/app/server/' + client + '/value.hmf'
    pattern1 = "*.hmf"
    pattern2 = "*.bit"
    #Remove the .bin bad files
    print("\n")
    files_2 = []
    files_1 = []
    for dir,_,_ in os.walk(path_to_results):
        files_2.extend(glob(os.path.join(dir,pattern2))) 
        
    for dir,_,_ in os.walk(path_db):
        files_1.extend(glob(os.path.join(dir,pattern1))) 
    
    

    for i in range (len(files_2)):
        
        #Execute multiplication
        
        os.system('sudo ./operation %s %s %s' % (operator, files_1[i], files_2[i] ))
    
          
    os.system('sudo mv %s %s' %(path_to_results,destination_path))
    
    #Remove the aux files
    os.system('sudo rm -r %s' %value_bin_path)
    os.system('sudo rm -r %s' %value_hmf_path)
    
    answer_client("SELECT RESULT",client)
    
    
    return 0
###############################################################################
def just_select_server(command,client,current_path):
    
    command_list = command.split(" ")
    
    #The command received is like:
    #SELECT selected_collumns table col_to_compare equal/smaller/bigger value_encrypted_name  
    if  len(command_list) == 6:

        
        table = command_list[2]
        
        #If more than one row column were selected
        if(len(command_list[1])>1):
            selected_cols = command_list[1].split(",")
        else:
            selected_cols = command_list[1]
        compare_col = command_list[3]
        symbol = command_list[4]
        value_file_no_extention = command_list[5]
        
        
        #For each selected column
        
        
        print("\n")
        path_to_values = str(current_path.parent) + '/app/db/' + table + '/' + compare_col + '/'
        
        for sel_col in selected_cols:
            all_lines = []
            #Get all the lines 
            
            #Get all the bin folders containning the respective line
            all_lines = next(os.walk(path_to_values))[1]
            
            #Check all input lines
            line_num = 0
            path_to_results = str(current_path.parent) + '/app/server/' + client + '/result.bin'
            
            os.system('sudo mkdir %s' %path_to_results)
            for line in all_lines:
                
                line_num = get_name_from_type((path_to_values + line))
                
                #invoke the comparator
                os.system('sudo ./compare %s %s %s %s %s' %(symbol, table, compare_col, line_num,client))

            path_db = str(current_path.parent) + '/app/db/' + table + '/' + sel_col + '/'
            
            # Multiplies the columns to get the result
            operations_in_queries("multiply", path_to_results, path_db, client, current_path)
            
            # Just for one column for now
    return 0



def analyze_command(client,path_to_file):
    #Opens the decrypted command
    file = open(path_to_file,'r')
    
    for line in file:
        #Reads only one line
        command = line
        break
    
    os.remove(path_to_file)
    if (command.find("CREATE TABLE") != -1):
        print('CREATING Table')
        
        flag = create_table(command,client,path_to_file)
        if flag == 0:
            answer_client("SUCCESS! Table Created!",client)
        else:
            print('Error.Table not created')
        
    elif (command.find("INSERT INTO") != -1):
        print('INSERTING INTO Table')
        
        flag = insert_into_table(command,client,path_to_file)
        
        if flag == 0:
            answer_client("SUCCESS! The values were Inserted!",client)
        else:
            answer_client("An error occured! NO value inserted",client)
        
        
    elif (command.find("SELECT LINE") != -1):
        print('Selecting to Delete row')
        
        #Function to verify if the line exists
        flag = verify_line_exists(command,client)
        
        #If the line wSA FOUND
        if flag == 0:
            answer_client("line exists",client)
        else:
            answer_client("LINE OR TABLE NOT FOUND!",client)
    
    elif (command.find("DELETE") != -1):
        
        #If the client already selected the line
        print('Available to Delete')
        flag = -1
        flag = delete_line(command)
        
        #If the line was successfully deleted
        if flag == 0:
            answer_client("SUCCESS DELETING THE LINE",client)
        else:
            answer_client("ERROR while trying to delete the line. Not deleted",client)
        
    elif (command.find("SELECT SUM") != -1):
        print('selecionar somar')
        
    elif (command.find("SELECT") != -1):
        print('selecting')
        just_select_server(command,client,current_path)
    
    file.close()
    
#######################################################################
#Main function
#######################################
def main():
    print('Running Server...')
    
    total_clients = number_of_clients()
    
    server_path = str(current_path.parent) +'/app/server/'
    pattern = "*.enc"

    menu = 0
    
    while menu == 0:
        
        #Always clean the old inputs
        received_commands = []
        #print(received_commands)
        for dir,_,_ in os.walk(server_path):
            received_commands.extend(glob(os.path.join(dir,pattern))) 
            
        if received_commands:
            
            client = get_name_from_type(received_commands[0])
            print('Command received from client %s' %client)
            
            #Decrypt the message in the folder of the client in app server
            os.system('sudo ./decrypt %s' %client)
            analyze_command(client, (str(current_path.parent) + '/app/server/' + client + '/' + client + '.txt'))
            
#######################################################################
            
        #Make the server wait until analyzing again
        time.sleep(1)
#######################################################################
       
main()

#END

