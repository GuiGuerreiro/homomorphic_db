# -*- coding: utf-8 -*-
"""
CSC PROJECT - Guilherme Guerreiro, Joaquim Inácio, João Marques
"""


import os
import pathlib
import re
from glob import glob



def get_name_from_type(path_to_file):
    
    command_sender = path_to_file[::-1]
    command_sender = command_sender.split("/",1)[0]
    enc_filename = command_sender[::-1]
    
    name = enc_filename.split(".")[0]
    
    return name


############################################################
#Check the number of clients created by the admin
############################################################
def number_of_clients():
    
    #Get path of the current folder
    current_path = pathlib.Path(__file__).parent.absolute()
    #Get the path of the destination folder
    path_to_dir = str(current_path.parent) +'/app/users'
    
    
    #Gives all the subdirectoriesof the users folder
    #x[0] contains the parent - users folder
    aux = next(os.walk(path_to_dir))[1]
    
    client_num = 0
    for i in aux:
        client_num+=1
    
    return client_num

#######################################################
#Execute the client side of the create table procedure#
#######################################################
def send_command_to_dbserver(client,command):
    
    filename ="command.txt"
    
    current_path = pathlib.Path(__file__).parent.absolute()
    path_to_file = str(current_path.parent) +'/app/users/' + str(client) + '/' + filename
    
    #Creates the command in the 
    file = open(path_to_file,'w')
    
    file.write(command)

    file.close()
    
    #Call the Cpp program to encrypt and send the message to the server
    os.system('sudo ./encrypt %s -asymmetric command.txt' %client)
    
    
    
    filename_enc = str(client) + '.enc'
    path_now = str(current_path.parent) +'/app/users/' + str(client) + '/' + filename_enc
    destination_path = str(current_path.parent) +'/app/server/' + str(client)
    
    #Moves the encrypted command to the server
    os.system('sudo mv %s %s' %(path_now, destination_path))
    print('SENT')
    return 0


#########################################
#SEND The values encrypted to the server
#########################################
    
def send_encrypted_values_to_server(value_hmf,value_bin,client):
    
    current_path = pathlib.Path(__file__).parent.absolute()
    path_to_hmf = str(current_path.parent) + '/app/users/' + str(client) + '/' + value_hmf
    path_to_bin = str(current_path.parent) + '/app/users/' + str(client) + '/' + value_bin
    
    destination_path = str(current_path.parent) +'/app/server/' + str(client)
    
    
    #Moves the encrypted .hmf to the server
    os.system('sudo mv %s %s' %(path_to_hmf, destination_path))
    
    #Moves the encrypted .bin to the server
    os.system('sudo mv %s %s' %(path_to_bin, destination_path))
    
#######################################################
#Deals with the input command
#######################################################
    
def input_to_dbserver(client,command):
    try:
        #Separates the tablename and the values
        new_command_list = command.split (" ")[0:4]
        new_command = ""
        
        #Get the command to be encrypted
        i=0
        for part in new_command_list:
            if i == 0:
                new_command = part
            else:
                new_command = new_command + " " + part
            i+=1
        
        #Get all the columns
        column_list = command.split (" ")[3]
        all_columns = re.sub('[()]', '', column_list)
        all_columns = all_columns.split(",")
        #Get table_name
        table_name = command.split(" ",3)[2]
        #Get the column values
        values_list = command.split(" ")[5]
        
        #Put the columns values in a list
        all_values = re.sub('[()]', '', values_list )
        #Get the values
        all_values = all_values.split(",")
        
        #Command verification
        if len(all_columns) != len(all_values):
            print('Invalid Command! Number of values and columns do not match')
            return -1
        
        #For each column and value of the new table
        for i in range(len(all_values)):
            #Creates an encrypted file .enc and a binary encrypted to each value
            #print('./encrypt %s -homomorphic %s %s' %(client,all_columns[i], all_values[i]))
            os.system('sudo ./encrypt %s -homomorphic %s %s' %(client,(all_columns[i]+ ".col"), all_values[i]))
            
            #Send to the user folder in the server the encrypted values in .hmf format and .bin
            send_encrypted_values_to_server((all_columns[i]+ ".hmf"),(all_columns[i]+ ".bin/"),client)
    
        #Send to the server the command(the values are already there in the user folder)
        trash = send_command_to_dbserver(client,new_command)
    
    except:
        print('Invalid Command')
        return -1
    
    return 0
    
###################################
#Get the result response from server
#####################################
def server_response(client):
    
    #Get path opf current folder
    current_path = pathlib.Path(__file__).parent.absolute()
    server_path = str(current_path.parent) +'/app/users/' + str(client) + '/'
    
    received_answer = []
    pattern = "answer.enc"
    new_path = server_path + 'answer.txt'
    while True:
        
        for dir,_,_ in os.walk(server_path):
            received_answer.extend(glob(os.path.join(dir,pattern)))
        
        if received_answer:
            
            #Decrypt the message in the folder of the client in app server
            os.system('sudo ./decrypt %s -asymmetric answer.enc' %client)
            file = open(new_path,'r')
            
            #Get the message written by the file
            for line in file:
                #The message from the server has 1 line
                file.close()
                os.system('sudo rm -r %s'%new_path)
                return line
        
    
    return ("MISS Response")
    
#Translate the symbols so they can be an input of the cpp function later
def translate_select_signal(sign):
    
    if sign == "=":
        return ("equal")
    if sign == ">":
        return ("bigger")
    if sign == "<":
        return ("smaller")
    
#Function to deal with selects without sum
def just_select(command, client):
    
    command_list = command.split(" ")
    
    if (len(command_list) == 8):
        print('Simple one')
        
        all_columns = command_list[1]
        compare_table = command_list[3]
        compare_column = command_list[5]
        sign = translate_select_signal(command_list[6])
        value_to_encrypt1 = command_list[7]
        #It has to be value.col so the comparator can recognize it
        compare_value = "value"
        
        new_command = 'SELECT' + ' ' + all_columns + ' ' + compare_table + ' ' + compare_column + ' ' + sign + ' ' + compare_value
        
        os.system('sudo ./encrypt %s -homomorphic %s %s' %(client,(compare_value + ".val"), value_to_encrypt1))
        send_command_to_dbserver(client,new_command )
        
        send_encrypted_values_to_server("value.hmf","value.bin",client)
        
        #WAIT FOR THE SERVER RESPONSE!!!!!
        ###################################
        return 0
    elif(len(command_list) == 12 and (command_list[8] =="AND" or command_list[8] == "OR")):
        print('SELECT With ands, ors')
    
    else:
        print('INVALID COMMAND')
    
    
    
    
    return -1


def desencrypt_select(client,current_path):
    path_to_result = str(current_path.parent)+'/app/users/'+str(client)+'/result.bin'
    
    all_bits = []
    all_answers = []
    pattern = '*.bit'
    pattern2 = '*.txt'
    for dir,_,_ in os.walk(path_to_result):
        all_bits.extend(glob(os.path.join(dir,pattern))) 
        
    
    
    for bit in all_bits:
        file = 'result.bin/' + get_name_from_type(bit) + '.bit'
        
        os.system('sudo ./decrypt %s -homomorphic %s' %(client, file))
        
    for dir,_,_ in os.walk(path_to_result):
        all_answers.extend(glob(os.path.join(dir,pattern2)))
  
    #loop through all txt
    i = 1
    print("(----------------PRINT-SELECT-RESULT----------------------)")
    for txt in all_answers:
        f = open(txt,'r')
        #Loop through each resul
        for line in f:
            #print(line)
            if int(line) != 0:
                print('Line %d - %s'%(i,line))
            break
        f.close()
        i+=1

    #os.system('sudo rm -r %s' %path_to_result)   


def print_line_found(client, current_path):
    path_to_result = str(current_path.parent) + '/app/users/' + str(client) + '/'

    # hmf files answered
    all_hmf = []
    # txt answered
    all_answers = []

    # patterns to look at
    pattern = '*.hmf'
    pattern2 = '*.txt'
    for dir, _, _ in os.walk(path_to_result):
        all_hmf.extend(glob(os.path.join(dir, pattern)))

    for hmf in all_hmf:
        hmf_name = get_name_from_type(hmf) + '.hmf'

        os.system('sudo ./decrypt %s -homomorphic %s' % (client, hmf_name))

    for dir, _, _ in os.walk(path_to_result):
        all_answers.extend(glob(os.path.join(dir, pattern2)))

    print("The values in this line are:")
    i = 1
    # loop through all txt
    for txt in all_answers:
        # If the file is not a key
        if txt.find("db_privkey") == -1 and txt.find("db_pubkey") == -1:
            f = open(txt, 'r')
            # Loop through each resul
            for line in f:
                # Save the select answer
                print('%d column: %s' % (i, line))

                break
            f.close()
            os.system('sudo rm -r %s' % txt)
        i += 1

    return 0


###############
#Main function#
###############
def main():
    
    current_path = pathlib.Path(__file__).parent.absolute()
    menu = 0
    chosen_line = -1
    num = number_of_clients()
    print('There are %s active clients'%num)
    while True:
        try:
            client = int(input ('Which client am I?\n'))
            if client <= num and client > 0:
                print('Running CLIENT %d API...' %client)
                break
            
            else:
                print('This client does not exist!')
        except: 
            print('This client does not exist!')
            
            
    while menu == 0:
        
        command = input()
        
        if (command.find("CREATE TABLE") != -1):
            
            flag = send_command_to_dbserver(client,command)
            #Wait for server response
            if flag == 0:
                answer = server_response(client)
                print(answer)
            
            
        elif (command.find("INSERT INTO") != -1):
            
            #Calls the input function
            flag = input_to_dbserver(client, command)
            
            #Wait for server response
            answer = server_response(client)
            print (answer)
        
        #Este vai ser especial
        elif (command.find("SELECT LINE") != -1):
            print('Selecting line to delete row')
            
            #If the command is valid
            if len(command.split(" ")) == 5:
                #Send the command to server and wait for a response
                trash = send_command_to_dbserver(client,command)
                answer = server_response(client)
                print(answer)
                #If the table, column and row were found
                if answer == "line exists":
                    chosen_line = 0
                    print_line_found(client, current_path)
                else:
                    print(answer)
                    continue
            else:
                continue
            
        elif (command.find("DELETE") != -1 and chosen_line == 0):
            print('Deleting')
            #Send the command to the db
            chosen_line == -1
            trash = send_command_to_dbserver(client,command)
            answer = server_response(client)
            print(answer)
            
        elif (command.find("SELECT SUM") != -1):
            print('Summing')
            #send_to_dbserver("slstable_",3,command)
       
        elif (command.find("SELECT") != -1):
            flag = 0
            flag = just_select(command, client)
            if flag == 0:
                print('Waiting...')
                select = server_response(client)
                print("\n")
                
                if select == "SELECT RESULT":
                    desencrypt_select(client,current_path)
            
        
        #To change the client user during program execution
        elif (command.find("CHANGE CLIENT ") != -1):
            
            #Get current number of clients
            #num = number_of_clients()
            command_list = command.split(" ")
            
            #Search for the introduced client number
            cnt = 0
            for i in command_list:
                if cnt == 2:
                    
                    #Check if the client exists
                    if int(i) <= num and int (i) > 0:
                        print('Running CLIENT %d API...' %int(i))
                        client = int(i)
                    else:
                        print('Invalid Client!')
                        break
                cnt+=1
                    
        elif command == "EXIT":
            print('See you next time!')
            menu = -1
        
        else:
            print('INVALID COMMAND, Try again')
            
main()

#END