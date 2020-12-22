#include <iostream>
#include <string>
//#include <cstddef>
#include <fstream>
//#include <iomanip>
//#include <vector>
//#include "seal/seal.h"

using namespace std;

int main(int argc, char *argv[]) {

	char sign[200];
	if (argc < 2) {
		cout << "Not enough arguments.\nUsage: ./verify_sign filepath rows \n";
		exit(-1);
	}
	string filepath(argv[1]);
	int rows(stoi(argv[2]));
	string new_filename = filepath.substr(0, filepath.size() - 4);

	sprintf(sign, "sudo openssl dgst -sha256 -sign "
				  "../../server_privkey.pem -out %s.sha256 %s.bit", new_filename.c_str(), filepath.c_str());

	system(sign);
}


