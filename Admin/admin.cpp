/******************************************************************************
*
* File Name: admin.cpp
* Authors:   Guiherme Guerreiro, João Marques, Joaquim Inácio
* Revision:
*
* NAME
*  admin - Execution of all the functions needed to setup the Homomorphic Database for n users
*
* DESCRIPTION
*  The execution of this file creates a set of folders necessary for the
* execution of the developed database system. This sets the root CA certificate,
* root CA private key, the users certificates and private keys. It also sets
* the database public key. Besides all of the execution of this
* code also sets the necessary folders and signs the necessary documents for the
* system to run
*
******************************************************************************/
#include <iostream>
#include <string>
//#include <cstddef>
//#include <fstream>
//#include <iomanip>
//#include <vector>
#include "generations.h"
//#include "seal/seal.h"

using namespace std;
using namespace seal;

int main(int argc, char *argv[]){

	int nusers;

  	if(argc < 2){
    	cout << "Introduce as an argument the number of users for the";
    	cout << " database!\n";
    	exit(-1);
	}
  	string s(argv[1]);
  	if(!isNumber(s)){
    	cout << "Introduce a number and nothing else as an argument!\n";
    	exit(-1);
  	}
 	if(stoi(argv[1]) <= 0){
   		cout << "The number must be positive!\n";
    	exit(-1);
  	}
  	nusers = stoi(argv[1]);

  	clean_install();      //delete app folder
   	generaterootca();     //create CA folder and CA certificate
  	installserverrootCA();//create server and db folder, install CA certificates in it
	server_certificate(); //Create server certificate
  	generate_homomorphic_keys();

  	system("sudo mkdir ../app/users");
  	for(int i=0; i < nusers; i = i + 1){
    	createuserfolder(i);
		installuserrootCA(i);               //install CA certificate in user folder
		user_certificate(i);                //Create user certificate
		installuser_servercertificate(i);	//install the server certificate
		install_homomorphic_keypair(i);
		installusercertificate_inserver(i);
  	}
  	system("sudo rm -r db_privkey.txt");
  	system("sudo rm -r db_pubkey.txt");//*/
}
