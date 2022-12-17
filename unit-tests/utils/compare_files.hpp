#pragma once

#include <fstream>
#include <string>

#include "../../containers/exception.hpp"


namespace ut { /* 'ut' = unit-tests */

bool are_same_files(std::ifstream & f1, std::ifstream & f2)
{
    char ch1,
         ch2;
    bool are_equal = true;
    while (f1.get(ch1) && f2.get(ch2)){
        if (ch1 != ch2){
            are_equal = false;
            break;
        }
    }

    return are_equal;
}

} /* namespece ut */
