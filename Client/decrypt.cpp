
#include <iostream>
#include <string>
//#include <cstddef>
//#include <fstream>
//#include <iomanip>
//#include <vector>
#include "encryptfiles.h"
#include "seal/seal.h"

using namespace std;
using namespace seal;


void asymmetric_decryption(int, string);
void homomorphic_decryption(int, string);

int main(int argc, char *argv[]){

	if(argc < 3){
		cout << "Not enough arguments.\nUsage: ./decrypt userid -homomorphic|-asymmetric filename\n";
		exit(-1);
	}
	string type(argv[2]);
	string filename(argv[3]);
	int userid(stoi(argv[1]));

	if(type == "-homomorphic"){
		//cout << "the file " << filename << " will be decrypted homomorphicly"<<endl;
		homomorphic_decryption(userid, filename);
	}
	else if(type == "-asymmetric"){
		//cout << "the file " << filename << " will be decrypted asymetrically"<<endl;
		asymmetric_decryption(userid, filename);
	}
}

void asymmetric_decryption(int userid, string filename){
	char dec_command[250];
	string new_filename = filename.substr(0, filename.size()-4);
	sprintf(dec_command, "cd ../app/users/%d/ && sudo openssl rsautl -decrypt -oaep -in %s "
						 "-inkey user_privkey.pem -out %s.txt && sudo rm -r %s",userid, filename.c_str(), new_filename.c_str(), filename.c_str());

	string cmd(dec_command);
	system(cmd.c_str());//*/
}

void homomorphic_decryption(int userid, string filename){

	SecretKey privkey;
	Ciphertext cipher;
	EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 16384;
	parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(128);
	SEALContext context(parms);

	ifstream bitfile;
	string aux(std::filesystem::current_path());
	aux.append("/../app/users/");
	aux.append(to_string(userid));
	aux.append("/");
	aux.append(filename);

	bitfile.open(aux, ios::binary);
	cipher.load(context, bitfile);
	bitfile.close();

	string key_path(std::filesystem::current_path());
	key_path.append("/../app/users/");
	key_path.append(to_string(userid));
	key_path.append("/db_privkey.txt");


	bitfile.open(key_path,ios::binary);
	privkey.load(context, bitfile);
	bitfile.close();

	Plaintext plainresult;
	Decryptor decryptor(context, privkey);


	//cout<<decryptor.invariant_noise_budget(cipher)<<" bits <-- noise budget\n";

	decryptor.decrypt(cipher,plainresult);


	ofstream file;

	string remove(aux);

	string new_filename = aux.substr(0, aux.size()-4);
	new_filename.append(".txt");
	file.open(new_filename, ios::out);

	file<<plainresult.to_string();
	file.close();//*/

	string command = "sudo rm -r ";
	command.append(remove);
	system(command.c_str());
}