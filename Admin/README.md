# Admin 
Requirements: This program requires you to install the CMake version at least 3.12 which can be done following this website https://cmake.org/download/, and also the SEAL Microsoft Library for Homomorphic encryption, at least version 3.6.1, which can be done following the guide on this website https://github.com/Microsoft/SEAL.

## Description: Setup the database system, by doing the following:

Generate a root CA certificate and private key.
Install the root CA certificate in every user and on the server.
Generate a homomorphic key pair for every user and a RelinKey for server-side operations.

For each user:
Generates a private key.
Installs the user certificate and private key on the corresponding folder.
Installs the election public key and signature on the corresponding folder.

### Usage: To run this program first it is need to generate a make file using a CMake command, then compile the program using this make file and run the program.

cmake . -DCMAKE_PREFIX_PATH=~/mylibs
make
./admin [number of users]
