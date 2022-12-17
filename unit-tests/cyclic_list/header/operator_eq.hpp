#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <utility>
#include <initializer_list>
#include <string>
#include <deque>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"


namespace ut { /* 'ut' = unit-tests */


void t_operator_eq(std::ofstream & fout)
{
    // TEST 'OPERATOR=()' (copy and move)

    //-------------------------------------------------------------------------------------------------
    // Test with 'int'

    UT_CONTAINER_NAME<int> ic1 = {2, 575, 6, 8, -393, 0, 6, -809, -45, 23, 13},
                ic2 = {98, 50, -43, 2, -9, -47, 32},
                ic3,
                ic4;

    ic3 = ic1;
    ut::print_container(ic3, fout);
    ic1 = ic4;
    ut::print_container(ic1, fout);
    ic4 = std::move(ic2);
    ut::print_container(ic2, fout);
    ut::print_container(ic4, fout);
    ic3 = std::move(ic1);
    ut::print_container(ic3, fout);

    ic1 = {1, 2, 3, 4, 5, 0};
    ut::print_container(ic1, fout);
    ic1 = {1, 2, 0};
    ut::print_container(ic1, fout);
    ic1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    ut::print_container(ic1, fout);
    ic1 = {};
    ut::print_container(ic1, fout);
    ic1 = {1, 2};
    ut::print_container(ic1, fout);

    //-------------------------------------------------------------------------------------------------
    // Test with 'std::deque<std::string>'

    UT_CONTAINER_NAME<std::deque<std::string>> dc1;

    dc1 = { {"one", "two"}, {"three", "four", "five"}, {}, {"six"}, {"next is 7 ?"} };
    ut::print_container_of_containers(dc1, fout);

    dc1 = {{}};
    ut::print_container_of_containers(dc1, fout);

    dc1 = { {"one", "two", "three", "four", "five"}, {"six"}, {"next is 7 ?"} };
    ut::print_container_of_containers(dc1, fout);

    //-------------------------------------------------------------------------------------------------
    // Test with 'UT_CONTAINER_NAME<char>'

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<char>> cc1,
                         cc2,
                         cc3;

    cc1 = {{'a', 'b', 'c', 'd', 'e'}, {}, {'t', 'r', 'i'}, {'j', 'j', 'j', 'j'}, {'2', '6'}, {'7'}, {'w', 'w', 'w'}};
    ut::print_container_of_containers(cc1, fout);
    cc2 = {{}, {}, {}};
    ut::print_container_of_containers(cc2, fout);
    cc3 = cc1;
    ut::print_container_of_containers(cc3, fout);
    cc2 = std::move(cc1);
    ut::print_container_of_containers(cc2, fout);

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section operator_eq_section(
        "'operator=()' (copy and move)",
        file_names[1],
        {
            {"'operator=()' (copy and move)", ut::t_operator_eq}
        }
    );

    operator_eq_section.run();

    return 0;
}
