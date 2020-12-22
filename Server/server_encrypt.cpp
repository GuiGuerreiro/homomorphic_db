
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

	if(argc < 3){
		cout << "Not enough arguments.\nUsage: ./encrypt userid filename\n";
		exit(-1);
	}

	string userid(argv[1]);
	string filename(argv[2]);

	char sign[200];
	char enc_command[200];
	string path = "cd ../app/server/";

	string new_filename = filename.substr(0, filename.size()-4);

	sprintf(enc_command, "%s%s && sudo openssl rsautl -encrypt -oaep -in %s -pubin "
						 "-inkey %s_pubkey.pem -out %s.enc", path.c_str(), userid.c_str(), filename.c_str(), userid.c_str(), new_filename.c_str());

	string command(enc_command);
	system(command.c_str());

	path.append(userid.c_str());
	path.append(" && sudo rm -r ");
	path.append(filename.c_str());
	system(path.c_str());
	path = "cd ../app/server/";
	sprintf(sign, "%s%s && sudo openssl dgst -sha256 -sign "
				  "../server_privkey.pem -out %s.sha256 %s.enc",path.c_str(), userid.c_str(),new_filename.c_str(), new_filename.c_str());

	system(sign);
}
