#pragma once

#include <fstream>
#include "../../containers/exception.hpp"


#define UT_OPEN_SEPARATE_SYMBOL  '['
#define UT_CLOSE_SEPARATE_SYMBOL ']'


namespace ut { /* 'ut' = unit-tests */


template <typename CONTAINER_T>
void print_container(const CONTAINER_T & cont, std::ofstream & fout)
{
    if(!fout.is_open())
        throw ctl::exception("Given file is closed", "ut::print_container()");

    fout << UT_OPEN_SEPARATE_SYMBOL << "  ";
    for(auto elem : cont)
        fout << elem << "  ";
    fout << UT_CLOSE_SEPARATE_SYMBOL << '\n';
}

template <typename CONTAINER_T>
void print_container_of_containers(const CONTAINER_T & cont_of_conts, std::ofstream & fout)
{
    if(!fout.is_open())
        throw ctl::exception("Given file is closed", "ut::print_container_of_containers()");

    fout << UT_OPEN_SEPARATE_SYMBOL << UT_OPEN_SEPARATE_SYMBOL;
    for(auto cont : cont_of_conts){
        fout << "\n   " << UT_OPEN_SEPARATE_SYMBOL << "  ";
        for(auto elem : cont)
            fout << elem << "  ";
        fout << UT_CLOSE_SEPARATE_SYMBOL;
    }
    fout << '\n' << UT_CLOSE_SEPARATE_SYMBOL << UT_CLOSE_SEPARATE_SYMBOL << '\n';
}


} /* namespece ut */
