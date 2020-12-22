cmake_minimum_required(VERSION 3.12)

#cmake . -DCMAKE_PREFIX_PATH=~/mylibs

#set the project name
project(client)

add_library(Encryptlib encryptfiles.cpp)

#add the executable
add_executable(encrypt encrypt.cpp)
add_executable(decrypt decrypt.cpp)
add_executable(verify verify_sign.cpp)


target_link_libraries(encrypt Encryptlib)
target_link_libraries(Encryptlib SEAL::seal)
find_package(SEAL 3.6.1 REQUIRED)
target_link_libraries(decrypt SEAL::seal)
target_link_libraries(encrypt SEAL::seal)
