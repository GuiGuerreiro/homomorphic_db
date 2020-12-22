/******************************************************************************
*
* File Name: generations.cpp
* Authors:   Guilherme Guerreiro, João Marques, Joaquim Inácio
* Revision:
*
* NAME
*  generations - Declaration of the functions for generating certificates and keys
*
* DESCRIPTION
*  Most of the functions declaread on this file are for general verifications
* that the data needs to be subjected too, but also for folder creations,
* certififcates and keys creations with openssl and signing documents
*
*****************************************************************************/
#include "generations.h"

using namespace std;
using namespace seal;


/******************************************************************************
 * isNumber()
 *
 * Arguments: string inputed
 * Returns: boolean true if s is a number and false if not
 *
 * Description: This function verifies if the inputed string is a number and
 *  returns a boolean according to that
 *
 *****************************************************************************/
bool isNumber(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (isdigit(s[i]) == false)
            return false;
    return true;
}

/******************************************************************************
 * clean_install()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Deletes any previous directory with the name /app and
 * re creates it
 *
 *****************************************************************************/
void clean_install(){
    system("cd .. && sudo rm -r app");
    system("cd .. && sudo mkdir app");
}

/******************************************************************************
 * generaterootca()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Creates the folder for the certificate authority.
 * This function also creates the key and certificate for the root CA
 *
 *****************************************************************************/
void generaterootca(){
  system("cd .. && sudo mkdir app/CA");
  system("cd ../app/CA && sudo openssl genpkey -out my-ca.key -algorithm rsa -pkeyopt rsa_keygen_bits:2048");
  system("cd ../app/CA && sudo openssl req -new -x509 -days 3650 -key my-ca.key -out my-ca.crt");
}

/******************************************************************************
 * installserverrootCA()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Creates the server folder and installs the CA certificate in it
 * Also creates the database folder
 *
 *****************************************************************************/
void installserverrootCA(){
  	system("sudo mkdir ../app/server");
  	system("sudo cp ../app/CA/my-ca.crt ../app/server");
	system("sudo mkdir ../app/db");
}

/******************************************************************************
 * createuserfolder()
 *
 * Arguments: - user
 * Returns: none
 *
 * Description: Creates a folder for the user with the corresponding user number
 *
 *****************************************************************************/
void createuserfolder(int user){
  	string userfolder;
  	string command;
  	string openuserfolder;

  	command = "sudo mkdir "; //create a directory
	userfolder = "../app/users/";
	userfolder.append(to_string(user + 1));
  	command.append(userfolder);
  	system(command.c_str());
}

/******************************************************************************
 * user_certificate()
 *
 * Arguments: - user number
 * Returns: none
 *
 * Description: Creates a certificate for the user
 *  and also signs the certificate with the root CA private key. The
 *  certificate is fullfiled automaticaly
 *
 *****************************************************************************/
void user_certificate(int user){
	string userfolder;
	string command;
  	string openuserfolder;
  	string usercertdata;
  	string mv1 = " && sudo mv user-cert.csr ../../CA";
  	string mv2 = "cd ../app/CA/ && sudo mv user-cert.crt ../users/";

  	string ca_signer = "cd ../app/CA/ && sudo openssl x509 -req -days 360 "
					  "-in user-cert.csr -CA my-ca.crt -CAkey my-ca.key -CAcreateserial -out user-cert.crt";

	string gen_pkey_and_csr = " && sudo openssl req -new -newkey rsa:2048 "
						   "-nodes -keyout user_privkey.pem -out user-cert.csr -subj";

	usercertdata = " /C=LX/ST=Portugal/L=Lisbon/O=CSC/OU=CProject/CN=User";

	//create private key and CSR
  	userfolder = "cd ../app/users/"; //open the directory
  	userfolder.append(to_string(user + 1));
  	userfolder.append(gen_pkey_and_csr); //private key and certificate signing request creation
	usercertdata.append(std::to_string(user + 1));
  	userfolder.append(usercertdata);
	system(userfolder.c_str());

	//SEND csr to CA and creates certificate
	userfolder = "cd ../app/users/"; //open the directory
	userfolder.append(to_string(user + 1));
	userfolder.append(mv1); //moves the csr to CA
	system(userfolder.c_str());
	system(ca_signer.c_str());

	//return new certificate to user
  	mv2.append(to_string(user + 1));
  	system(mv2.c_str()); //moves the mint new certificate to its user
  	//delete csr file
  	system("cd ../app/CA && sudo rm -r user-cert.csr");
}

/******************************************************************************
 * server_certificate()
 *
 * Arguments: - none
 * Returns: none
 *
 * Description: Creates a certificate for the server
 *  and also signs the certificate with the root CA private key. The
 *  certificate is fullfiled automaticaly
 *
 *****************************************************************************/
void server_certificate(){
	string serverfolder;
	string command;
	string openserverfolder;
	string servercertdata;
	string mv1 = "cd ../app/server/ && sudo mv server-cert.csr ../CA";
	string mv2 = "cd ../app/CA/ && sudo mv server-cert.crt ../server/";

	string ca_signer = "cd ../app/CA/ && sudo openssl x509 -req -days 360 "
					   "-in server-cert.csr -CA my-ca.crt -CAkey my-ca.key -CAcreateserial -out server-cert.crt";

	string gen_pkey_and_csr = "cd ../app/server/ && sudo openssl req -new -newkey rsa:2048 "
							  "-nodes -keyout server_privkey.pem -out server-cert.csr -subj";

	servercertdata = " /C=LX/ST=Portugal/L=Lisbon/O=CSC/OU=CProject/CN=Server";

	//create private key and CSR
	gen_pkey_and_csr.append(servercertdata);
	system(gen_pkey_and_csr.c_str());

	//SEND csr to CA and creates certificate
	system(mv1.c_str());
	system(ca_signer.c_str());

	//return new certificate to user
	system(mv2.c_str()); //moves the mint new certificate to its user
	//delete csr file
	system("cd ../app/CA && sudo rm -r server-cert.csr");
}

/******************************************************************************
 * installuserrootCA()
 *
 * Arguments: - user number
 * Returns: none
 *
 * Description: Copies the root CA certificate to the folder of the user
 *
 *****************************************************************************/
void installuserrootCA(int user){
  string command;

  command = "sudo cp ../app/CA/my-ca.crt ../app/users/";
  command.append(to_string(user + 1));
  system(command.c_str());
}

/******************************************************************************
 * installuser_servercertificate()
 *
 * Arguments: - user number
 * Returns: none
 *
 * Description: Installs the server certificate in the user folder
 *
 *****************************************************************************/
void installuser_servercertificate(int user){
    string command;
    char extract[200];

    command = "sudo cp ../app/server/server-cert.crt ../app/users/";
    command.append(to_string(user + 1));
    system(command.c_str());

    //extract server pubkey from certificate
    sprintf(extract, "sudo openssl x509 -pubkey "
					 "-in ../app/users/%d/server-cert.crt -out ../app/users/%d/server_pubkey.pem", user+1, user+1);
    string c(extract);
    system(c.c_str());
}

/******************************************************************************
 * generate_homomorphic_keys()
 *
 * Arguments: none
 * Returns: none
 *
 * Description: Generates the chosen context and the keys for this context.
 *  Then this saves the generated keys to the corresponding files and puts each
 *  file in the right place.
 *
 *****************************************************************************/
void generate_homomorphic_keys(){
	// Defining the context and parameters for the key generation
	seal::EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 16384;
	parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(128);

	SEALContext context(parms);
	// Key Generation
	KeyGenerator keygen(context);
	PublicKey database_public_key;
 	keygen.create_public_key(database_public_key);
	SecretKey database_secret_key = keygen.secret_key();
	seal::Serializable <RelinKeys> relin_keys = keygen.create_relin_keys();

	// Saving the keys to files
	ofstream publickeyfile;
	publickeyfile.open("db_pubkey.txt", ofstream::binary);
	database_public_key.save(publickeyfile);
	publickeyfile.close();
	ofstream secretkeyfile;
	secretkeyfile.open("db_privkey.txt", ofstream::binary);
	database_secret_key.save(secretkeyfile);
	secretkeyfile.close();
	ofstream relinkeysfile;
	relinkeysfile.open("relin_key.txt", ofstream::binary);
	relin_keys.save(relinkeysfile);
	relinkeysfile.close();


	system("sudo mv relin_key.txt ../app/server");
	//*/
}

/******************************************************************************
 * install_homorphic_keypair()
 *
 * Arguments: user
 * Returns: none
 *
 * Description: Installs the private and public homomorphic keys in every user
 *
 *****************************************************************************/
 void install_homomorphic_keypair(int user){

 	string command1 = "sudo cp db_privkey.txt ../app/users/";
 	string command2 = "sudo cp db_pubkey.txt ../app/users/";
 	command1.append(to_string(user+1));
 	command2.append(to_string(user+1));
 	system(command1.c_str());
 	system(command2.c_str());
 }

/******************************************************************************
* installusercertificate_inserver()
*
* Arguments: - user number
* Returns: none
*
* Description: Installs the server certificate in the user folder
*
*****************************************************************************/
void installusercertificate_inserver(int user){

	char extract[200];
	string part1 = "cd ../app/server/";
	string part3 = " && sudo mv user-cert.crt ";
	string part2 = " && sudo cp user-cert.crt ../../server/";
	string rename = ".crt";
	string directory = "cd ../app/users/";
	string mkdir = "sudo mkdir ../app/server/";


	mkdir.append(to_string(user+1));
	system(mkdir.c_str());
	//mkdir.append("/result.bin");
	//system(mkdir.c_str());

	directory.append(to_string(user+1));
	directory.append(part2);
	directory.append(to_string(user+1));

	part1.append(to_string(user+1));
	part3.append(to_string(user+1));
	part3.append(rename);
	part1.append(part3);

	system(directory.c_str());
	system(part1.c_str());

	sprintf(extract, "sudo openssl x509 -pubkey "
					 "-in ../app/server/%d/%d.crt -out ../app/server/%d/%d_pubkey.pem", user+1,user+1,user+1, user+1);
	string c(extract);
	system(c.c_str());
}