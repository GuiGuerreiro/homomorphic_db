//
// Created by Gui on 18/12/2020.
//

#include <iostream>
#include <string>
//#include <cstddef>
//#include <fstream>
//#include <iomanip>
//#include <vector>
#include "encryptfiles.h"
//#include "seal/seal.h"

using namespace std;
//using namespace seal;

int main(int argc, char *argv[]){

	char sign[200];
	if(argc < 4){
		cout << "Not enough arguments.\nUsage: ./encrypt userid -homomorphic|-asymmetric filename valor\n";
		exit(-1);
	}
	string type(argv[2]);
	string filename(argv[3]);
	int userid(stoi(argv[1]));

	if(type == "-homomorphic"){
		//cout << "the file " << filename << " will be encrypted homomorphicly"<<endl;
		int value = stoi(argv[4]);
		homomorphic_encryption(value, userid, filename);


		string new_filename = filename.substr(0, filename.size()-4);
		sprintf(sign, "cd ../app/users/%d && sudo openssl dgst -sha256 -sign "
					  "user_privkey.pem -out %s.sha256 %s.hmf",userid,new_filename.c_str(),new_filename.c_str());
		system(sign);


		for(int i=0; i<=3; i++){
			char signbit[200];
			sprintf(signbit, "cd ../app/users/%d/%s.bin && sudo openssl dgst -sha256 -sign "
						  "../user_privkey.pem -out %d.sha256 %d.bit",userid,new_filename.c_str(),i,i);
			system(signbit);
		}


	}
	else if(type == "-asymmetric"){
		//cout << "the file " << filename << " will be encrypted asymetrically"<<endl;
		asymmetric_encryption(userid, filename);

		sprintf(sign, "cd ../app/users/%d && sudo openssl dgst -sha256 -sign "
					  "user_privkey.pem -out %d.sha256 %d.enc",userid,userid,userid);
		system(sign);
	}
}