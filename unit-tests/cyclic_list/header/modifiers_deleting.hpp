#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <utility>
#include <initializer_list>
#include <functional>
#include <string>
#include <list>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"


namespace ut { /* 'ut' = unit-tests */


void t_clear(std::ofstream & fout)
{
    // Test with <int>
    
    UT_CONTAINER_NAME<int> ic = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ic.clear();
    if(ic.empty()){
        ic.clear();
        fout << "empty\n";
    }
    else
        fout << "non-empty\n";
    print_container(ic, fout);

    ic.insert_after(ic.cbefore_begin(), {1, 2, 3});
    print_container(ic, fout);

    // Test with <std::list<std::string>>

    UT_CONTAINER_NAME<std::list<std::string>> lsc = { {"one", "two"}, {"three", "four", "five"}, {}, {"six"}, {"next_is_7_?"} };
    lsc.clear();
    if(lsc.empty()){
        lsc.clear();
        lsc.clear();
        fout << "empty\n";
    }
    else
        fout << "non-empty\n";
    print_container_of_containers(lsc, fout);

    lsc.insert_after(lsc.cbefore_begin(), {"C++", "Java", "PHP"});
    print_container_of_containers(lsc, fout);

}


void t_erase_after(std::ofstream & fout)
{
    // Test with <int>
    
    UT_CONTAINER_NAME<int> ic = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    auto it1 = ic.begin();
    for(int i = 0; i < 8; ++i)
        ++it1;
    it1 = ic.erase_after(ic.begin(), it1);
    print_container(ic, fout);
    fout << *it1 << '\n';
    ic.erase_after(it1, ic.end());
    print_container(ic, fout);

    // Test with <UT_CONTAINER_NAME<std::string>>

    UT_CONTAINER_NAME<UT_CONTAINER_NAME<std::string>> lsc = { {"C++", "Java"}, {"C#", "Clojure", "Lisp"}, {}, {"PHP"}, {"Rlang", "Golang"} };
    auto it2 = lsc.cbegin();
    ++it2;
    lsc.erase_after(lsc.erase_after(it2));
    print_container_of_containers(lsc, fout);

}


void t_pop_front(std::ofstream & fout)
{
    // Test with <int>
    
    UT_CONTAINER_NAME<int> c = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    c.pop_front();
    c.pop_front();
    c.pop_front();
    c.pop_front();
    print_container(c, fout);

}


void t_removes(std::ofstream & fout)
{
    // Test with <std::string>
    
    UT_CONTAINER_NAME<std::string> sc = {"C++", "Java", "C++", "C++", "C++", "C#", "Clojure", "Lisp", "PHP", "C+", "Rlang", "Golang", "C++C++", "C++"};
    sc.remove("C++");
    print_container(sc, fout);

    sc = {"C++", "Java", "C++", "C++", "C++", "C#", "Clojure", "Lisp", "PHP", "C-+", "Rlang", "Golang", "C+C", "C++"};
    sc.remove_if([](const std::string & str){return (str.find("C+") == std::string::npos ? false : true);});
    print_container(sc, fout);

}


void t_unique(std::ofstream & fout)
{
    // Test with <int>
    
    UT_CONTAINER_NAME<int> ic = {0, 1, 2, 2, 2, 3, 0, 1, 3, 3, 2, 1, 1, 1, 1, 0, 4, 3, 0, 4, -6, 8, 8, 7};
    ic.unique();
    print_container(ic, fout);

    ic = {1, 1, 1, 1, 1, 1};
    ic.unique();
    print_container(ic, fout);

    ic = {1, 2, 3, 4, 5, 6};
    ic.unique();
    print_container(ic, fout);

    ic = {0, 1, 2, 3, 4, 5};
    ic.unique(std::less<int>{});
    print_container(ic, fout);

    ic = {0, 1, 2, 5, 2, 3, 0, 1, 3, 3, 2, 1, 1, 4, 1, 0, 4, 3, 0, 4, -6, 8, 8, 7};
    ic.unique(std::less<int>{});
    print_container(ic, fout);

    ic = {5, 4, 3, 2, 1, 0};
    ic.unique(std::less<int>{});
    print_container(ic, fout);

}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section modifiers_deleting(
        "Modifiers of deleting: 'clear', 'erase_after', 'pop_front', 'remove(_if)', 'unique'",
        file_names[1],
        {
            {"'clear()'",                               ut::t_clear         },
            {"'emplace_after()' and 'emplace_front()'", ut::t_erase_after   },
            {"'pop_front()'",                           ut::t_pop_front     },
            {"'remove(_if)()'",                         ut::t_removes       },
            {"'unique()'",                              ut::t_unique        }
        }
    );

    modifiers_deleting.run();

    return 0;
}
