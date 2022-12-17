/* As you can guess, this section test not only constructors, but destructor too (it 
    is expected that you run them under valgrind or similar utility */

#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <utility>
#include <initializer_list>
#include <string>
#include <vector>
#include <list>
#include <array>
#include <complex>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"


#define UT_DEFAULT_CONTAINER_SIZE 20


namespace ut { /* 'ut' = unit-tests */


void t_default_constructor(std::ofstream & fout)
{

    UT_CONTAINER_NAME<int>         c1;
    print_container(c1, fout);

    UT_CONTAINER_NAME<std::string> c2;
    print_container(c2, fout);

    const UT_CONTAINER_NAME<std::string> c3;
    print_container(c3, fout);

    UT_CONTAINER_NAME<std::vector<std::string>>  c4;
    print_container_of_containers(c4, fout);

    UT_CONTAINER_NAME<std::array<double, 10>>    c5;
    print_container_of_containers(c5, fout);

}


void t_size_constructor(std::ofstream & fout)
{
    // TEST WITH INT

    size_t sz = UT_DEFAULT_CONTAINER_SIZE;

    const UT_CONTAINER_NAME<int> ic1 = UT_CONTAINER_NAME<int>();
    print_container(ic1, fout);

    auto ic2 = UT_CONTAINER_NAME<int>(sz);
    print_container(ic2, fout);

    const UT_CONTAINER_NAME<int> ic3(sz);
    print_container(ic3, fout);

    UT_CONTAINER_NAME<int> ic4 = UT_CONTAINER_NAME<int>(0);
    print_container(ic4, fout);

    int ivalue = 8;;
    UT_CONTAINER_NAME<int> ic5 = UT_CONTAINER_NAME<int>(sz, ivalue);
    print_container(ic5, fout);

    // TEST WITH STD::STRING

    const UT_CONTAINER_NAME<std::string> sc1 = UT_CONTAINER_NAME<std::string>();
    print_container(sc1, fout);

    auto sc2 = UT_CONTAINER_NAME<std::string>(sz);
    print_container(sc2, fout);

    const UT_CONTAINER_NAME<std::string> sc3(sz);
    print_container(sc3, fout);

    UT_CONTAINER_NAME<std::string> sc4 = UT_CONTAINER_NAME<std::string>(0);
    print_container(sc4, fout);

    std::string svalue = "abc";
    UT_CONTAINER_NAME<std::string> sc5 = UT_CONTAINER_NAME<std::string>(sz, svalue);
    print_container(sc5, fout);

    // TEST WITH SOME CONTAINER

    UT_CONTAINER_NAME<std::array<char, 6>> cc1(sz);
    print_container_of_containers(cc1, fout);

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int *>> cc2(sz);
    print_container_of_containers(cc2, fout);

}


void t_initializer_constructor(std::ofstream & fout)
{

    UT_CONTAINER_NAME<double> c1 = {};
    print_container(c1, fout);

    UT_CONTAINER_NAME<int> c2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    print_container(c2, fout);

    UT_CONTAINER_NAME<std::string> c3 = {"one", "two", "three", "four", "five", "six", "next is 7 ?"};
    print_container(c3, fout);

    UT_CONTAINER_NAME<std::complex<double>> c4 = { {0., 0.}, {-2., 4.}, {17., 97.}, {6. -15.}, {5.56, 7.62} };
    print_container(c4, fout);

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>> c5 = { {1, 2}, {3, 4, 5, 6}, {7}, {8, 9, 10, 11 ,12}, {}, {13, 14, 15} };
    print_container_of_containers(c5, fout);

    UT_CONTAINER_NAME<std::vector<std::string>> c6 = { {"one", "two"}, {"three", "four", "five"}, {}, {"six"}, {"next is 7 ?"} };
    print_container_of_containers(c6, fout);

}


void t_iterator_range_constructor(std::ofstream & fout)
{

    std::list<int> l1;
    UT_CONTAINER_NAME<int>    c1(l1.begin(), l1.end());
    print_container(c1, fout);

    std::list<int> l2 = {1};
    UT_CONTAINER_NAME<int>    c2(l2.begin(), l2.end());
    print_container(c2, fout);

    std::list<int> l3 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    UT_CONTAINER_NAME<int>    c3(l3.begin(), l3.end());
    print_container(c3, fout);

    std::list<std::string> l_4_5_6 = {"one", "two", "three", "four", "five", "six", "seven", "next is 8 ?"};
    UT_CONTAINER_NAME<std::string>    c4(l_4_5_6.begin(), l_4_5_6.end());
    print_container(c4, fout);
    
    auto it = l_4_5_6.begin();
    ++it;
    ++it;
    ++it;
    ++it;
    UT_CONTAINER_NAME<std::string> c5(it, l_4_5_6.end());
    print_container(c5, fout);

    UT_CONTAINER_NAME<std::string> c6(++l_4_5_6.begin(), it);
    print_container(c6, fout);

    UT_CONTAINER_NAME<std::vector<int>> c7 = { {1, 2}, {3, 4, 5}, {}, {6}, {7, 8, 9}, {10, 11, 12}, {13}, {0, 0, 0, 0, 0, 0}, {-1} };
    auto first = c7.begin();
    auto last = first;
    ++first;
    ++(++(++(++(last))));
    UT_CONTAINER_NAME<std::vector<int>> c8(first, last);
    print_container_of_containers(c8, fout);

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>> c9 = { {1, 2}, {3, 4, 5, 6}, {7}, {8, 9, 10, 11 ,12}, {}, {13, 14, 15} };
    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>> c10(c9.begin(), c9.end());
    print_container_of_containers(c10, fout);

}


void t_copy_and_move_constructors(std::ofstream & fout)
{
    //-------------------------------------------------------------------------------------------------

    UT_CONTAINER_NAME<int *> c1 = {};
    UT_CONTAINER_NAME<int *> copy_c1 = c1;
    print_container(copy_c1, fout);
    UT_CONTAINER_NAME<int *> move_c1 = std::move(copy_c1);
    print_container(move_c1, fout);
    print_container(c1, fout);

    //-------------------------------------------------------------------------------------------------

    UT_CONTAINER_NAME<int> c2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
    UT_CONTAINER_NAME<int> copy_c2 = c2;
    print_container(copy_c2, fout);
    UT_CONTAINER_NAME<int> move_c2 = std::move(c2);
    print_container(move_c2, fout);
    print_container(c2, fout);

    //-------------------------------------------------------------------------------------------------

    UT_CONTAINER_NAME<std::string> c3 = {"one", "two", "three", "four", "five", "six", "next is 7 ?"};
    UT_CONTAINER_NAME<std::string> copy_c3 = c3;
    print_container(copy_c3, fout);
    UT_CONTAINER_NAME<std::string> move_c3 = std::move(c3);
    print_container(move_c3, fout);
    print_container(c3, fout);

    //-------------------------------------------------------------------------------------------------

    UT_CONTAINER_NAME<std::complex<int>> c4 = { {0, 0}, {-2, 4}, {17, 97}, {6, -15}, {556, 762} };
    UT_CONTAINER_NAME<std::complex<int>> copy_c4 = c4;
    print_container(copy_c4, fout);
    UT_CONTAINER_NAME<std::complex<int>> move_c4 = std::move(copy_c4);
    print_container(move_c4, fout);
    print_container(c4, fout);

    //-------------------------------------------------------------------------------------------------

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>> c5 = { {1, 2}, {3, 4, 5, 6}, {7}, {8, 9, 10, 11 ,12}, {}, {13, 14, 15} };
    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>> copy_c5 = c5;
    print_container_of_containers(copy_c5, fout);
    UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>> move_c5 = std::move(copy_c5);
    print_container_of_containers(move_c5, fout);
    print_container_of_containers(c5, fout);

    //-------------------------------------------------------------------------------------------------

    UT_CONTAINER_NAME<std::vector<std::string>> c6 = { {"one", "two"}, {"three", "four", "five"}, {}, {"six"}, {"next is 7 ?"} };
    UT_CONTAINER_NAME<std::vector<std::string>> copy_c6 = c6;
    print_container_of_containers(copy_c6, fout);
    UT_CONTAINER_NAME<std::vector<std::string>> move_c6 = std::move(c6);
    print_container_of_containers(move_c6, fout);
    print_container_of_containers(c6, fout);

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section constructor_section(
        "Constructors",
        file_names[1],
        {
            {"Default constructor",         ut::t_default_constructor       },
            {"Size constructor",            ut::t_size_constructor          },
            {"Initializer constructor",     ut::t_initializer_constructor   },
            {"Iterator range constructor",  ut::t_iterator_range_constructor},
            {"Copy and move constructors",  ut::t_copy_and_move_constructors}
        }
    );

    constructor_section.run();

    return 0;
}
