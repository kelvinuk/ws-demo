//
// Created by Kelvin Chow on 15/10/2024.
//
#include "./my_memory_check.hpp"
#include <iostream>

void my_memory_check() {

}

void throw_error_with_memory_leak() {
    try
    {
        // Allocate 1kB buffer
        const char *buffer = new char[1024];

        std::cout << buffer << std::endl;;
        // Throw
        throw 1;

        // Free up buffer
        delete buffer;
    }
    catch(const int e)
    {
        // printf("We have a memory leak\n");
    }
}
