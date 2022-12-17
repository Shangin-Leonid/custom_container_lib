#pragma once

#include <iostream>
#include <fstream>

/* For test cases */
#include <utility>
#include <initializer_list>
#include <string>
#include <list>

#include UT_INCLUDED_LIB_HEADER

#include "../../utils/section.hpp"
#include "../../utils/log_printing.hpp"


namespace ut { /* 'ut' = unit-tests */


class Emplacable_t
{
  private:

    size_t _ui;
    char _c;
    std::string _str;

  public:
    
    Emplacable_t() :
        _ui(0), _c(' '), _str("")
    { }

    Emplacable_t(size_t ui, char c, const std::string & str) :
        _ui(ui), _c(c), _str(str)
    { }

    Emplacable_t(size_t ui, char c, std::string && str) :
        _ui(ui), _c(c), _str(str)
    { }

    ~Emplacable_t()
    { }

    friend std::ofstream & operator<<(std::ofstream & ofs, const Emplacable_t & obj);
};

std::ofstream & operator<<(std::ofstream & ofs, const Emplacable_t & obj)
{
    ofs << "{ " << obj._ui << ' ' << obj._c << ' ' << obj._str << " }";

    return ofs;
}



void t_insert_after(std::ofstream & fout)
{
    // Test with <int>

    UT_CONTAINER_NAME<int> ic = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(auto it = ic.begin(); *it != 5; ++it)
        it = ic.insert_after(it, 0);
    ut::print_container(ic, fout);

    ic = {33};
    ic.insert_after(ic.insert_after(ic.cbegin(), -5), 78);
    ut::print_container(ic, fout);

    ic = {0, 0, 0, 0, 0, 0};
    auto it = ic.cbegin();
    ++it, ++it, ++it;
    ic.insert_after(it, 7, 1);
    ut::print_container(ic, fout);

    ic = {0, 0, 0, 0, 0, 0};
    it = ic.cbegin();
    ++it, ++it, ++it;
    it = ic.insert_after(it, {-7, -6, -5, -4});
    ic.insert_after(it, 1, 5);
    ic.insert_after(it, 0, 555);
    ut::print_container(ic, fout);

    // Test with <std::string>

    UT_CONTAINER_NAME<std::string> sc = {"one", "two", "three", "four", "five", "six", "next is 7 ?"};
    std::string s1 = "empty3";
    std::string s2 = "fff";
    auto sit = sc.cbegin();
    sit++, ++sit;
    sit = sc.insert_after(sit, s1);
    ++sit;
    sc.insert_after(sit, std::move(s1));
    sit = sc.insert_after(sit, s2);
    ut::print_container(sc, fout);

    /*++sit;
    sc.insert_after(sit, s2.begin() + 3, s2.end());
    ut::print_container(sc, fout);*/

}


void t_emplace_after_front(std::ofstream & fout)
{
    // Test with <std::string>

    UT_CONTAINER_NAME<std::string> sc = {"one", "two", "three", "four", "five", "six"};
    std::string str = "test_str";
    auto sit = sc.cbegin();
    sit++, ++sit;
    sit = sc.emplace_after(sit, str);
    ++sit;
    sit = sc.emplace_after(sit, std::move(str));
    sc.emplace_after(sit, "c_str");
    sc.emplace_front("front_str");
    ut::print_container(sc, fout);

    // Test with <ut::Emplacable_t>
    
    UT_CONTAINER_NAME<Emplacable_t> ec = { {11, 'g', "str1"}, {22, 'e', "str2"}, {33, 'i', "str3"}, {44, 'w', "str4"} };
    str = "inserted";
    auto it = ec.cbegin();
    it++, ++it;
    it = ec.emplace_after(it, 2, '.', "inserted");
    ec.emplace_front(7, '-', "front");
    ut::print_container(ec, fout);

    // Test with empty <ut::Emplacable_t>

    UT_CONTAINER_NAME<Emplacable_t> empty_c;
    empty_c.emplace_front(1, '+', "first");
    ut::print_container(empty_c, fout);

}


void t_push_front(std::ofstream & fout)
{
    UT_CONTAINER_NAME<std::string> c;
    std::string s1 = "empty3";
    std::string s2 = "fff";
    c.push_front(s1);
    c.push_front(std::move(s2));
    c.push_front(std::move(s1));
    ut::print_container(c, fout);
    
}


} /* namespace ut */



int main(int n_files, char * file_names[])
{
    if(n_files != 2){
        std::cerr << "You must send exactly 1 file name for container output logs.\n";
        return -1;
    }

    ut::Section modifiers_adding(
        "Modifiers of adding: 'insert_after', 'emplace_after', 'emplace_front', 'push_front'",
        file_names[1],
        {
            {"'insert_after()'",                        ut::t_insert_after        },
            {"'emplace_after()' and 'emplace_front()'", ut::t_emplace_after_front },
            {"'push_front()'",                          ut::t_push_front          }
        }
    );

    modifiers_adding.run();

    return 0;
}
