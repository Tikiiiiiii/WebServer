# TinyWebserver
Achieve A Tiny Echo Web Server in order to learn the principle of network communications

# REquired
OS：Ubuntu22.04
CMake：3.22.1
G++：11.4.0
python:3.10.12
clang:14.0.0-1ubuntu1.1
clang-format:14.0.0-1ubuntu1.1
cpplint:1.5.5
LLVM:14.0.0

# Introduction
In file study_process is the process of my learning road.
It includes some learn notes.

In file project is the completed project

# Complied
command：
>cd project/  
mkdir build    
cd build  
cmake ..  
chmod +x ../build_support/run_clang_format.  
chmod +x ../build_support/cpplint.py   
chmod +x ../build_support/run_clang_tidy.py   
make format  
make cpplint   
make clang_tidy  
make echo_server  
make echo_clients  
make echo_client  
./bin/echo_server  
./bin/echo_client   
./bin/echo_clients -t 100 -m 5 (-w 5)  



