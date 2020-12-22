//
// Created by Gui on 20/12/2020.
//

#include <iostream>
#include <string>
#include "path_handling.h"
//#include <cstddef>
//#include <fstream>
//#include <iomanip>
//#include <vector>
#include "seal/seal.h"
#include "stdio.h"
#include <filesystem>
//#include<iostream>

using namespace std;
using namespace seal;


void load_bitfiles(Ciphertext *bits, char* path, SEALContext context){
	ifstream bitfile;

	for(int i=0; i<=3; i++){
		//load all .bit files into bits[]
		string aux =std::filesystem::current_path();
		string bit_path(path);
		bit_path.append(to_string(i));
		bit_path.append(".bit");
		aux.append(bit_path);
		bitfile.open(aux, ios::binary);
		//if(bitfile.is_open())
			//cout<<"bitfileisopen";
		//cout << "bit " << i << ":" << aux << endl;

		bits[i].load(context, bitfile);
		bitfile.close();//*/
	}
}
void return_result(Ciphertext result, string path){
	ofstream file;
	file.open(path, ios::binary);

	//cout<<path<<" return result"<<endl;

	//if(file.is_open())
		//cout<<"aleluia mermao\n";
	result.save(file);
	file.close();//*/
}
Ciphertext load_cipher(string path, SEALContext context){
	ifstream bitfile;
	Ciphertext file;

	bitfile.open(path, ios::binary);
	//if(bitfile.is_open())
	//	cout<<"load cipher"<<endl;

	file.load(context, bitfile);
	bitfile.close();//*/
	return file;

}