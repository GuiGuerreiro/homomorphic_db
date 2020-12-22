//
// Created by Gui on 19/12/2020.
//

#include <iostream>
#include <string>
//#include <cstddef>
//#include <fstream>
//#include <iomanip>
//#include <vector>
//#include "seal/seal.h"

using namespace std;
//using namespace seal;

int main(int argc, char *argv[]){

	char dec_command[200];
	if(argc < 2){
		cout << "Not enough arguments.\nUsage: ./decrypt userfolder\n";
		exit(-1);
	}
	int userfolder = stoi(argv[1]);

	sprintf(dec_command, "cd ../app/server/%d/ && sudo openssl rsautl -decrypt -oaep -in %d.enc "
					  "-inkey ../server_privkey.pem -out %d.txt && sudo rm -r %d.enc",userfolder, userfolder, userfolder, userfolder);
	string cmd(dec_command);

	system(cmd.c_str());
}