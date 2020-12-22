//
// Created by Gui on 20/12/2020.
//

#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "seal/seal.h"
#include <stdlib.h>
#include "path_handling.h"


using namespace std;
using namespace seal;


void homo_mult(Ciphertext, Ciphertext*, SEALContext, RelinKeys, string);
void homo_sum(Ciphertext, Ciphertext*, SEALContext,RelinKeys, string);

int main(int argc, char* argv[]){

	ifstream file;
	if(argc<3){
		cout<<"usage: ./operation   multiply|sum  file1 file2"<< endl;
		cout<<"operations are done and stored in file2"<<endl;
		exit(-1);
	}

	EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 16384;
	parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(128);
	SEALContext context(parms);
	Ciphertext cipher_f1, cipher_f2;
	//!Load Relin keys
	RelinKeys r_key;
	string aux(std::filesystem::current_path());
	file.open(aux.append("/../app/server/relin_key.txt"), ios::binary);
	r_key.load(context, file);
	file.close();

	string type(argv[1]);
	string file1(argv[2]);
	string file2(argv[3]);
	cipher_f1=load_cipher(file1, context);
	cipher_f2=load_cipher(file2, context);


	//cout<<file2.c_str()<<" file 2 path"<<endl;


	if(type == "multiply")
		homo_mult(cipher_f1,&cipher_f2, context, r_key, file2);
	else if(type == "sum")
		homo_sum(cipher_f1,&cipher_f2, context, r_key, file2);
}

void homo_mult(Ciphertext file1, Ciphertext *file2, SEALContext context,RelinKeys relin_keys, string path){

	Evaluator evaluator(context);
	evaluator.multiply_inplace(*file2,file1);
	evaluator.relinearize_inplace(*file2, relin_keys);
	return_result(*file2, path);
}

void homo_sum(Ciphertext file1, Ciphertext *file2, SEALContext context, RelinKeys relin_keys,string path){

	Evaluator evaluator(context);
	evaluator.add_inplace(*file2,file1);
	evaluator.relinearize_inplace(*file2, relin_keys);
	return_result(*file2, path);
}
