//
// Created by Gui on 20/12/2020.
//

#ifndef ADMIN_PATH_HANDLING_H
#define ADMIN_PATH_HANDLING_H
#include <iostream>
#include <string>
#include <bits/stdc++.h>
#include "seal/seal.h"

using namespace std;
using namespace seal;

void load_bitfiles(Ciphertext*, char*, SEALContext);
void return_result(Ciphertext, string);
Ciphertext load_cipher(string, SEALContext);

#endif //ADMIN_PATH_HANDLING_H
