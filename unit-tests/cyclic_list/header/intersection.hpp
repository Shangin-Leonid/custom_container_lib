/* void swap(cyclic_list & another) noexcept;

    void merge(cyclic_list & another); // TODO: noexcept(operator<)
    template <typename Compare_Fun_t>
    void merge(cyclic_list & another, Compare_Fun_t comparator); //TODO : noexcept(comporator)

    void splice_after(const_iterator pos, cyclic_list & another) noexcept;
    void splice_after(const_iterator pos, cyclic_list & another, const_iterator removable_el);
    void splice_after(const_iterator pos, cyclic_list & another,
                      const_iterator another_first, const_iterator another_last);*/

#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <utility>
#include <initializer_list>
#include <functional>
#include <string>
#include <vector>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"


namespace ut { /* 'ut' = unit-tests */


void t_swap(std::ofstream & fout)
{
    UT_CONTAINER_NAME<std::vector<int>> vc1 = { {1, 2}, {3, 4, 5}, {}, {6}, {7, 8, 9}, {10, 11, 12}, {13}, {0, 0, 0, 0, 0, 0}, {-1} };
    UT_CONTAINER_NAME<std::vector<int>> vc2 = vc1;
    vc2.reverse();
    vc2.pop_front();
    vc1.swap(vc2);
    print_container_of_containers(vc1, fout);
    print_container_of_containers(vc2, fout);

    vc1= {};
    vc1.swap(vc2);
    print_container_of_containers(vc1, fout);
    print_container_of_containers(vc2, fout);

    vc1 = {};
    vc1.swap(vc2);
    print_container_of_containers(vc1, fout);
    print_container_of_containers(vc2, fout);

}


/*void t_merge(std::ofstream & fout)
{
    
}


void t_splice_after(std::ofstream & fout)
{
    
}*/


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section intersection(
        "Intersection: 'swap', 'merge', 'splice_after'",
        file_names[1],
        {
            {"'swap()'",         ut::t_swap         }
            //{"'merge()'",        ut::t_merge        },
            //{"'splice_after()'", ut::t_splice_after }
        }
    );

    intersection.run();

    return 0;
}
