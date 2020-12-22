/******************************************************************************
*
* File Name: generations.cpp
* Authors:   Guilherme Guerreiro, João Marques, Joaquim Inácio
* Revision:
*
* NAME
*  generations - Header file for the generations.cpp file
*
* DESCRIPTION
* This file is just an header file for the generations.cpp, with just the
* necessary declarations and includes
*
*****************************************************************************/
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "seal/seal.h"

using namespace std;

bool isNumber(string);
void clean_install();
void generaterootca();
void installserverrootCA();
void createuserfolder(int);
void user_certificate(int);
void server_certificate();
void installuserrootCA(int);
void installuser_servercertificate(int);
void installusercertificate_inserver(int);
void install_homomorphic_keypair(int);
void generate_homomorphic_keys();

