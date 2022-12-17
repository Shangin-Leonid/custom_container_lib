#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <initializer_list>
#include <string>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"



namespace ut { /* 'ut' = unit-tests */


void t_comparing(std::ofstream & fout)
{
    // Test with <int>

    UT_CONTAINER_NAME<int> c1 = {24, -55, -67, 87, 30},
                c2 = {24, -55, -67, 87, 30};
    fout << (c1 == c2 ? "==" : "!=") << "  " << (c1 != c2 ? "!=" : "==") << "  " << (c2 == c1 ? "==" : "!=") << '\n';
    fout << (c1 <= c2 ? "<=" : ">") << "  " << (c1 < c2 ? "<" : ">=") << "  " << (c2 < c1 ? "<" : ">=") << '\n';
    
    c1 = {};
    fout << (c1 == c2 ? "==" : "!=") << "  " << (c1 != c2 ? "!=" : "==") << "  " << (c2 == c1 ? "==" : "!=") << '\n';
    fout << (c1 >= c2 ? ">=" : "<") << "  " << (c1 > c2 ? ">" : "<=") << "  " << (c2 < c1 ? "<" : ">=") << '\n';

    c2 = {};
    fout << (c1 == c2 ? "==" : "!=") << "  " << (c1 != c2 ? "!=" : "==") << '\n';
    fout << (c1 <= c2 ? "<=" : ">") << "  " << (c1 < c2 ? "<" : ">=") << "  " << (c2 < c1 ? "<" : ">=") << '\n';

    c1 = {0, 1, 2, 2, 2, 3, 0, 1, 3, 3, 2, 1, 1, 1, 1, 0, 4, 3, 0, 4, -6, 8, 8, 7};
    c2 = {0, 1, 3, 3, 2, 1, 4, 3, 0, 4, -6, 8, 8, 7};
    fout << (c1 == c2 ? "==" : "!=") << "  " << (c1 != c2 ? "!=" : "==") << "  " << (c2 != c1 ? "!=" : "==") << '\n';
    fout << (c1 >= c2 ? ">=" : "<") << "  " << (c1 > c2 ? ">" : "<=") << "  " << (c2 < c1 ? "<" : ">=") << '\n';
    
    c1.swap(c2);
    fout << (c1 == c2 ? "==" : "!=") << "  " << (c1 != c2 ? "!=" : "==") << "  " << (c2 == c1 ? "==" : "!=") << '\n';
    fout << (c1 >= c2 ? ">=" : "<") << "  " << (c1 > c2 ? ">" : "<=") << "  " << (c2 < c1 ? "<" : ">=") << '\n';

    // Test with <UT_CONTAINER_NAME<std::string>>

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<std::string>> cc1 = { {"C++", "Java"}, {"C#", "Clojure", "Lisp"}, {}, {"PHP"}, {"Rlang", "Golang"} };
    UT_CONTAINER_NAME<UT_CONTAINER_NAME<std::string>> cc2 = { {"C#", "Clojure", "Lisp"}, {}, {"PHP"}, {"Rlang", "Golang"}, {"C++", "Java"} };
    fout << (cc1 == cc2 ? "==" : "!=") << "  " << (cc1 != cc2 ? "!=" : "==") << "  " << (cc2 == cc1 ? "==" : "!=") << '\n';
    fout << (cc1 >= cc2 ? ">=" : "<") << "  " << (cc1 > cc2 ? ">" : "<=") << "  " << (cc2 < cc1 ? "<" : ">=") << '\n';

    cc1.swap(cc2);
    cc1.pop_front();
    fout << (cc1 >= cc2 ? ">=" : "<") << "  " << (cc1 > cc2 ? ">" : "<=") << "  " << (cc2 < cc1 ? "<" : ">=") << '\n';

    cc1 = { {"C#", "Clojure", "Lisp"}, {}, {"PHP"}, {"Rlang", "Golang"} };
    cc2 = { {"C#", "Clojure", "Lisp"}, {}, {"PHP"}, {"Rlang", "Golang"}, {"C++", "Java"} };
    fout << (cc1 == cc2 ? "==" : "!=") << "  " << (cc1 != cc2 ? "!=" : "==") << "  " << (cc2 == cc1 ? "==" : "!=") << '\n';
    fout << (cc1 >= cc2 ? ">=" : "<") << "  " << (cc1 > cc2 ? ">" : "<=") << "  " << (cc2 < cc1 ? "<" : ">=") << '\n';

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section comparing(
        "Comparing: <!=>",
        file_names[1],
        {
            {"operators <!=>", ut::t_comparing}
        }
    );

    comparing.run();

    return 0;
}
