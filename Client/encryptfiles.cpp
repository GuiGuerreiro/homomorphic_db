//
// Created by Gui on 18/12/2020.
//

#include "encryptfiles.h"
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "seal/seal.h"
#include <stdlib.h>

using namespace std;
using namespace seal;

/******************************************************************************
 * asymmetric_encryption()
 *
 * Arguments: - int userid
 *            - string filename
 * Returns: void
 *
 * Description: This function encrypts asymmetricaly the file that will be passed to
 * the server with the user commands and deletes the unencripted file
 *****************************************************************************/
void asymmetric_encryption(int userid, string filename){

	char enc_command[200];
	char dec_command[200];
	string path = "cd ../app/users/";

	sprintf(enc_command, "%s%d && sudo openssl rsautl -encrypt -oaep -in %s -pubin "
					  "-inkey server_pubkey.pem -out %d.enc", path.c_str(), userid, filename.c_str(), userid);
	string command(enc_command);
	system(command.c_str());
	path.append(to_string(userid));
	path.append(" && sudo rm -r ");
	path.append(filename.c_str());
	system(path.c_str());
}

/******************************************************************************
 * homomorphic_encryption()
 *
 * Arguments:
 * Returns: void
 *
 * Description: Encrypts the entire row of values
 *
 *****************************************************************************/
void homomorphic_encryption(int value, int userid, string filename){

	string db_pubkey_path = "../app/users/";
	string path = "cd ../app/users/";
	string new_filename = filename.substr(0, filename.size()-4);

	string value_file;
	string bit_file;

	// Setting the parameters for the encryption
	EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 16384;
	parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(128);
	SEALContext context(parms);

	// Getting the server public key from the corresponding file
	ifstream pubkey;
	db_pubkey_path.append(to_string(userid));
	db_pubkey_path.append("/db_pubkey.txt");
	pubkey.open(db_pubkey_path, ios::binary);
	PublicKey db_public_key;
	db_public_key.load(context, pubkey);
	pubkey.close();

	// Setting the encryption process
	Encryptor encryptor(context, db_public_key);

	//! Encrypts the value according to the first variant
	Plaintext plainvalue(to_string(value));
	Ciphertext encrypted_value;

	encryptor.encrypt(plainvalue, encrypted_value);
	// Creating the file to store the encrypted vote
	value_file.append(new_filename.c_str());
	value_file.append(".hmf");
	ofstream value_f;
	value_f.open(value_file, ofstream::binary);

	encrypted_value.save(value_f);
	value_f.close();

	//! Encrypts the value in a bitwise fashion, 4 less significant bits
	Plaintext plainbinaryvalue;
	string binaryvalue = std::bitset<4>(value).to_string();
	string binaryvalue_leftpadded = std::string(4 - binaryvalue.length(), '0') + binaryvalue;
	char move_binaries[100];

	ofstream bvalue_f;
	path.append(to_string(userid));
	path.append(" && sudo mkdir ");
	path.append(new_filename);
	path.append(".bin");
	system(path.c_str());
	string binaries;
	int aux;
	for(int i=0; i<=3; i++){

		aux= binaryvalue_leftpadded[3-i] - '0';
		//cout<<aux<<endl;
		plainbinaryvalue = to_string(aux);
		encryptor.encrypt(plainbinaryvalue, encrypted_value);


		bit_file.append(to_string(i));
		bit_file.append(".bit");
		bvalue_f.open(bit_file, ofstream::binary);
		encrypted_value.save(bvalue_f);
		bvalue_f.close();

		sprintf(move_binaries,"sudo mv %d.bit ../app/users/%d/%s.bin",i, userid, new_filename.c_str());
		binaries.assign(move_binaries);
		system(binaries.c_str());

		move_binaries[0]='\0';
		//move_binaries ="";
		bit_file.clear();
	}


	// Moving the file to the right place
	string mv1 = "sudo mv ";
	string mv2(value_file);
	string mv3 = " ../app/users/";
	mv3.append(to_string(userid));

	mv1.append(mv2);
	mv1.append(mv3);
	system(mv1.c_str());

// */
}