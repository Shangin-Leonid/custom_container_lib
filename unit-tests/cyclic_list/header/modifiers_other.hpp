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


namespace ut { /* 'ut' = unit-tests */


void t_resize(std::ofstream & fout)
{
    UT_CONTAINER_NAME<int> ic = {0, 1, 2, 2, 2, 3, 0, 1, 3, 3, 2, 1, 1, 1, 1, 0, 4, 3, 0, 4, -6, 8, 8, 7};
    ic.resize(8);
    print_container(ic, fout);

    ic = {1, 1, 1, 1};
    ic.resize(9);
    print_container(ic, fout);

    ic = {1, 1, 1, 1};
    ic.resize(9, -1);
    print_container(ic, fout);

    ic = {};
    ic.resize(9, -1);
    print_container(ic, fout);

    ic.resize(0, 100);
    print_container(ic, fout);

    UT_CONTAINER_NAME<std::vector<int>> vc = { {1, 2}, {3, 4, 5}, {}, {6}, {7, 8, 9}, {10, 11, 12}, {13}, {0, 0, 0, 0, 0, 0}, {-1} };
    vc.resize(2);
    print_container_of_containers(vc, fout);

}


void t_reverse(std::ofstream & fout)
{
    UT_CONTAINER_NAME<int> ic = {0, 1, 2, 2, 2, 3, 0, 1, 3, 3, 2, 1, 1, 1, 1, 0, 4, 3, 0, 4, -6, 8, 8, 7};
    ic.reverse();
    print_container(ic, fout);

    ic = {100};
    ic.reverse();
    print_container(ic, fout);

    ic = {};
    ic.reverse();
    print_container(ic, fout);

    UT_CONTAINER_NAME<std::string> sc = {"one", "two", "three", "four", "five", "six"};
    sc.reverse();
    print_container(sc, fout);

    UT_CONTAINER_NAME<std::vector<int>> vc = { {1, 2}, {3, 4, 5}, {}, {6}, {7, 8, 9}, {10, 11, 12}, {13}, {0, 0, 0, 0, 0, 0}, {-1} };
    vc.reverse();
    print_container_of_containers(vc, fout);

}


void t_sort(std::ofstream & fout)
{
    UT_CONTAINER_NAME<int> ic = {0, 1, 2, 2, 2, 3, 0, 1, 3, 3, 2, 1, 1, 1, 1, 0, 4, 3, 0, 4, -6, 8, 8, 7};
    ic.sort();
    print_container(ic, fout);

    ic = {0, 1, -5, 2, -22, 3, 103, 1, 31, 78, 2, 56, -1, -34, 90, 707, 74, 38, 0, 984, -6, 82, 897, 7};
    ic.sort();
    print_container(ic, fout);

    ic = {100};
    ic.sort();
    print_container(ic, fout);

    ic = {};
    ic.sort();
    print_container(ic, fout);

    UT_CONTAINER_NAME<std::string> sc = {"one", "two", "three", "four", "five", "six", "etc"};
    sc.sort();
    print_container(ic, fout);

    UT_CONTAINER_NAME<std::vector<int>> vc = { {1, 2}, {3, 4, 5}, {}, {6}, {7, 8, 9}, {10, 11, 12}, {13}, {0, 0, 0, 0, 0, 0}, {-1} };
    vc.sort();
    print_container_of_containers(vc, fout);

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section modifiers_other(
        "Other modifiers: 'resize', 'reverse', 'sort'",
        file_names[1],
        {
            {"'resize()'",  ut::t_resize  },
            {"'reverse()'", ut::t_reverse },
            {"'sort()'",    ut::t_sort    }
        }
    );

    modifiers_other.run();

    return 0;
}