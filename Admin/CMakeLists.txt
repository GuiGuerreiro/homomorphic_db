cmake_minimum_required(VERSION 3.12)

#cmake . -DCMAKE_PREFIX_PATH=~/mylibs

#set the project name
project(admin)

add_library(Generations generations.cpp)

#add the executable
add_executable(admin admin.cpp)


target_link_libraries(admin Generations)
target_link_libraries(Generations SEAL::seal)
find_package(SEAL 3.6.1 REQUIRED)
target_link_libraries(admin SEAL::seal)
