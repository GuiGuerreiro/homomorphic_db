cmake_minimum_required(VERSION 3.12)

#cmake . -DCMAKE_PREFIX_PATH=~/mylibs

#set the project name
project(server)

add_library(path_handling path_handling.cpp)

#add the executable
add_executable(decrypt server_decrypt.cpp)
add_executable(compare comparator.cpp)
add_executable(operation homo_operations.cpp)
add_executable(encrypt server_encrypt.cpp)
add_executable(verify verify_sign.cpp)
add_executable(sign sign_result.cpp)

target_link_libraries(operation path_handling)
target_link_libraries(compare path_handling)
target_link_libraries(path_handling SEAL::seal)
find_package(SEAL 3.6.1 REQUIRED)
target_link_libraries(compare SEAL::seal)
target_link_libraries(operation SEAL::seal)
