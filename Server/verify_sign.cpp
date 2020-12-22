#include <iostream>
#include <string>
//#include <cstddef>
#include <fstream>
//#include <iomanip>
//#include <vector>
//#include "seal/seal.h"

using namespace std;

string system_listen(string);

int main(int argc, char *argv[]){

	ofstream file;
	char verify_sign[200];
	if(argc < 2){
		cout << "Not enough arguments.\nUsage: ./verify_sign file|bin userfolder filename\n";
		exit(-1);
	}
	string type(argv[1]);
	string userfolder(argv[2]);
	string filename(argv[3]);
	string new_filename = filename.substr(0, filename.size()-4);

	if(type=="file"){
		sprintf(verify_sign, "cd ../app/server/%s && sudo openssl dgst -sha256 -verify "
							 "%s_pubkey.pem -signature %s.sha256 %s", userfolder.c_str(), userfolder.c_str(),
				new_filename.c_str(), filename.c_str());
		system(verify_sign);
		string resultname;
		resultname.append(new_filename);
		resultname.append(".vfy");
		file.open(resultname.c_str());
		if(system_listen(verify_sign) == "Verified OK\n")
			file << "ok";
		else
			file << "not ok";
		file.close();//*/
	}
	else if(type=="bin"){

		for(int i=0; i<=3;i++){
			sprintf(verify_sign, "cd ../app/server/%s/%s && sudo openssl dgst -sha256 -verify "
						"../%s_pubkey.pem -signature %d.sha256 %d.bit", userfolder.c_str(),filename.c_str(), userfolder.c_str(),
					i,i);
			system(verify_sign);
			string resultname;
			resultname.append(to_string(i));
			resultname.append(".vfy");
			file.open(resultname.c_str());
			if(system_listen(verify_sign) == "Verified OK\n")
				file << "ok";
			else
				file << "not ok";
			file.close();//*/
		}
	}
}

string system_listen(string cmd){
	string data;
	FILE *stream;
	const int max_buffer = 512;
	char buffer [max_buffer];

	stream = popen(cmd.c_str(), "r");

	if (stream){
		while(!feof(stream)){
			if (fgets(buffer, max_buffer, stream) != NULL)
				data.append(buffer);
		}
		pclose(stream);
	}
	return data;
}