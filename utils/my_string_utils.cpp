//
// Created by Kelvin Chow on 15/10/2024.
//
#include <string>
#include "./my_string_utils.hpp"

std::string revString(std::string str)
{
    int n = static_cast<int>(str.length());

    for (int i = 0; i < n / 2; i++) {
        std::swap(str[i], str[n - i - 1]);
    }
    return str;
}