#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <initializer_list>
#include <vector>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"



namespace ut { /* 'ut' = unit-tests */


void t_assign(std::ofstream & fout)
{
    // TEST 'ASSIGN' (copy and move)

    UT_CONTAINER_NAME<int> ic1 = {2, 575, 6, 8, -393, 0, 0, 0, 0, 6, -809, -45, 23, 13};
    ut::print_container(ic1, fout);

    /*ic1.assign(5, -223);
    ut::print_container(ic1, fout);

    ic1.assign(0, 6008);
    ut::print_container(ic1, fout);*/

    ic1.assign({10, 9, 8, 7, 6, 5, 4, 3, 2, -1});
    ut::print_container(ic1, fout);

    ic1.assign({});
    ut::print_container(ic1, fout);

    std::vector<int> vec = {6, -9, 6, -9, 6, -9, 6, 33, 56, -88, -23, 56, 80, -4, -3, 0, 17, 54, 76, -654};
    auto first = vec.begin() + 3;
    auto last = vec.begin() + 16;
    ic1.assign(vec.begin(), vec.begin());
    ut::print_container(ic1, fout);
    ic1.assign(first, last);
    ut::print_container(ic1, fout);

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section assign_section(
        "Assign",
        file_names[1],
        {
            {"'assign()'", ut::t_assign}
        }
    );

    assign_section.run();

    return 0;
}
