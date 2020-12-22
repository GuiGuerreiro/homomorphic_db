#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "seal/seal.h"
#include "path_handling.h"


using namespace std;
using namespace seal;


Ciphertext and_func(Ciphertext ,Ciphertext ,RelinKeys, SEALContext);
Ciphertext not_func(Ciphertext,RelinKeys, SEALContext);
void comparator(Ciphertext, Ciphertext, Ciphertext, Ciphertext, Ciphertext, Ciphertext*, Ciphertext*, Ciphertext*, RelinKeys, SEALContext);
void comparator1(Ciphertext, Ciphertext, Ciphertext*, Ciphertext*, Ciphertext*, RelinKeys, SEALContext);
void comparator_4_bits(Ciphertext*, Ciphertext*,Ciphertext*, Ciphertext*, Ciphertext*, RelinKeys, SEALContext);

int main(int argc, char* argv[]){

	char path[200];
	char userpath[200];
	char returnpath[200];
	ifstream file;

	if(argc < 4){
		cout << "Usage ./compare tipo nomedatabela col1 row user AND|OR tipo2 col2 row\"\n";
		exit(-1);
	}
	string type(argv[1]);
	EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 16384;
	parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(128);
	SEALContext context(parms);
	Ciphertext O_bigger, O_equal, O_smaller;
	string table(argv[2]);
	string column(argv[3]);
	string row(argv[4]);
	string user(argv[5]);
	sprintf(path, "/../app/db/%s/%s/%s.bin/", table.c_str(), column.c_str(), row.c_str());

	//caminho para a pasta onde é guardado o valor da querie
	sprintf(userpath, "/../app/server/%s/value.bin/",user.c_str());
	sprintf(returnpath, "../app/server/%s/result.bin/%s.bit",user.c_str(),row.c_str());

	//!Abrir os 4 ficheiros dos bits
	Ciphertext abits[4];
	Ciphertext bbits[4];

	load_bitfiles(abits, path, context);
	load_bitfiles(bbits, userpath, context);

	//!Load Relin keys
	RelinKeys r_key;
	string aux(std::filesystem::current_path());
	file.open(aux.append("/../app/server/relin_key.txt"), ios::binary);
	r_key.load(context, file);
	file.close();
	comparator_4_bits(abits, bbits, &O_bigger, &O_equal, &O_smaller, r_key, context);

	if(type == "equal")
		return_result(O_equal, returnpath);
	else if(type == "bigger")
		return_result(O_bigger, returnpath);
	else if(type == "smaller")
		return_result(O_smaller, returnpath);//*/
}

Ciphertext and_func(Ciphertext x,Ciphertext y,RelinKeys relin_keys, SEALContext context)
{

    Ciphertext result;
	Evaluator evaluator(context);
    evaluator.multiply(x,y,result);
    evaluator.relinearize_inplace(result, relin_keys);

    return result;
}

Ciphertext not_func(Ciphertext x,RelinKeys relin_keys, SEALContext context)
{

    Ciphertext not_x;
	Evaluator evaluator(context);
    Plaintext plain_one("1");
    evaluator.sub_plain(x, plain_one, not_x);
    evaluator.square_inplace(not_x);
    evaluator.relinearize_inplace(not_x, relin_keys);

    return not_x;
}

void comparator(Ciphertext A, Ciphertext B, Ciphertext I_bigger, Ciphertext I_equal, Ciphertext I_smaller, 
				Ciphertext *O_bigger, Ciphertext *O_equal, Ciphertext *O_smaller, RelinKeys relin_keys, SEALContext context)
{
	Ciphertext aux1, aux2, aux3, aux4, aux6, aux7, aux8, aux9;

	aux1=and_func(not_func(B,relin_keys,context),A,relin_keys,context);
	aux2=and_func(not_func(A,relin_keys,context),B,relin_keys,context);

	aux3=not_func(I_bigger,relin_keys,context);
	aux4=not_func(I_smaller,relin_keys,context);

	aux6=and_func(aux1,I_equal,relin_keys,context);
	aux7=and_func(aux2,I_equal,relin_keys,context);

	aux8=and_func(not_func(aux6,relin_keys,context),aux3,relin_keys,context);
	aux9=and_func(not_func(aux7,relin_keys,context),aux4,relin_keys,context);

	*O_bigger=not_func(aux8,relin_keys,context);
	*O_equal=and_func(aux8,aux9,relin_keys,context);
	*O_smaller=not_func(aux9,relin_keys,context);

}

void comparator1(Ciphertext A, Ciphertext B, Ciphertext *O_bigger, Ciphertext *O_equal, Ciphertext *O_smaller, RelinKeys relin_keys, SEALContext context)
{
	Ciphertext aux1, aux2, aux3, aux4, aux5, aux6, aux7, aux8, aux9;

	aux1=and_func(not_func(B,relin_keys,context),A,relin_keys,context);
	aux2=and_func(not_func(A,relin_keys,context),B,relin_keys,context);

	*O_bigger=aux1;
	*O_equal=and_func(not_func(aux1,relin_keys,context),not_func(aux2,relin_keys,context),relin_keys,context);
	*O_smaller=aux2;

}

void comparator_4_bits(Ciphertext*A, Ciphertext*B,Ciphertext *O_bigger, Ciphertext *O_equal, Ciphertext *O_smaller, RelinKeys relin_key, SEALContext context)
{
	comparator1(A[3],B[3],O_bigger,O_equal,O_smaller,relin_key,context);

	for(int i=2;i>=0;i--)
		comparator(A[i],B[i],*O_bigger,*O_equal,*O_smaller,O_bigger,O_equal,O_smaller,relin_key,context);//*/

}
