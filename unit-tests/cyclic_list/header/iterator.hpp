#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <initializer_list>
#include <string>
#include <vector>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"


#define UT_DEFAULT_CONTAINER_SIZE 20


namespace ut { /* 'ut' = unit-tests */


void t_begin_end(std::ofstream & fout)
{
    // TEST '(C)BEGIN()' AND '(C)END()'

    UT_CONTAINER_NAME<double> c1 = {};
    fout << (c1.begin() == c1.end() ? "equal" : "non-equal") << '\n';
    fout << (c1.begin() != c1.end() ? "equal" : "non-equal") << '\n';
    fout << (c1.cbegin() == c1.cend() ? "equal" : "non-equal") << '\n';
    fout << (typename UT_CONTAINER_NAME<double>::const_iterator(c1.begin()) == c1.cend() ? "equal" : "non-equal") << '\n';


    UT_CONTAINER_NAME<int> c2 = {1, 2, 3};
    fout << (c2.begin() == c2.end() ? "equal" : "non-equal") << '\n';
    fout << (c2.cbegin() == c2.cend() ? "equal" : "non-equal") << '\n';
    fout << (c2.cbegin() == c2.end() ? "equal" : "non-equal") << '\n';
    fout << (c2.cbegin() != c2.end() ? "equal" : "non-equal") << '\n';

    typename UT_CONTAINER_NAME<int>::iterator it = c2.begin();
    typename UT_CONTAINER_NAME<int>::const_iterator cit = c2.cend();
    auto ait = c2.cbegin();

    *it, *cit, *ait;

}


void t_arithmetic_and_accessing(std::ofstream & fout)
{
    // TEST ARITHMETIC AND ACCESSING

    UT_CONTAINER_NAME<int> ic = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    auto it1 = ic.begin(),
         it2 = ic.begin();

    ++it1, ++it1, ++it1, ++it1;
    it2++, it2++, it2++, it2++;
    fout << (it1 == it2 ? "equal" : "non-equal") << '\n';
    fout << (it1 == ((((ic.begin()++)++)++)++) ? "equal" : "non-equal") << '\n';
    it1++;
    fout << (it1 == it2 ? "equal" : "non-equal") << '\n';
    fout << (it1 != it2 ? "equal" : "non-equal") << '\n';

    for(auto it = ic.cbegin(); it != ic.cend(); ++it)
        fout << *it << ' ';
    fout << '\n';

    const UT_CONTAINER_NAME<int> cic = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    for(auto it = cic.begin(); it != cic.end(); ++it)
        fout << *it << ' ';
    fout << '\n';
    for(auto elem : cic)
        fout << elem << ' ';
    fout << '\n';

    UT_CONTAINER_NAME<std::string> strc = {"abcdef", "ABCDEF", "uuuu"};
    fout << (strc.begin()++)->at(3) << '\n';

    typename UT_CONTAINER_NAME<std::string>::const_iterator(strc.end());
    fout << (typename UT_CONTAINER_NAME<std::string>::const_iterator(strc.end()) != strc.cend() ? "equal" : "non-equal") << '\n';
}


void t_before_begin(std::ofstream & fout)
{
    // TEST 'BEFORE_BEGIN()'

    UT_CONTAINER_NAME<std::vector<int>> c = { {1, 2}, {3, 4, 5}, {}, {6}, {7, 8, 9}, {10, 11, 12}, {13}, {0, 0, 0, 0, 0, 0}, {-1} };
    fout << (*((c.before_begin()++)++) != *(c.begin()++) ? "equal" : "non-equal") << '\n';
    fout << (*((c.before_begin()++)++) == *(c.begin()++) ? "equal" : "non-equal") << '\n';

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section iterator_section(
        "Iterators",
        file_names[1],
        {
            {"'(c)begin()' and '(c)end()'",     ut::t_begin_end               },
            {"Arithmetic and accessing",        ut::t_arithmetic_and_accessing},
            {"'(c)before_begin()'",             ut::t_before_begin            }
        }
    );

    iterator_section.run();

    return 0;
}
