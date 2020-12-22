# Server
Requirements: This program requires you to install the CMake version at least 3.12 which can be done following this website https://cmake.org/download/, and also the SEAL Microsoft Library for Homomorphic encryption, at least version 3.6.1, which can be done following the guide on this website https://github.com/Microsoft/SEAL.

Description: This program manages all the operations server-side.

Receives the user commands and acts accordingly.
Also performs operations in the stored values without ever decrypting them.


Usage: To run this program first it is need to generate a make file using a CMake command, then compile the program using this make file and run the program.

cmake . -DCMAKE_PREFIX_PATH=~/mylibs
make
sudo python3 database_server.py
