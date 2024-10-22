//
// Created by Kelvin Chow on 15/10/2024.
//
#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "./my_lambda_func.hpp"

void my_lambda_func() {
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;
    std::for_each(
     in(std::cin), in(), std::cout << (_1 * 3) << " " );
}