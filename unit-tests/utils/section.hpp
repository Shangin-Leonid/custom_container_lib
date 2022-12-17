#pragma once

#include <utility>
#include <initializer_list>
#include <fstream>
#include <string>

#include "testing_types.hpp"
#include "../../containers/exception.hpp"


#define UT_HUGE_SEPARATE_CAPTION_STRING   "============================================="
#define UT_LITTLE_SEPARATE_CAPTION_STRING "---------------------------------------------"


namespace ut { /* 'ut' = unit-tests */



class Test_case final
{

  private:
    
    std::string   _case_name;
    testing_fun_t _pfun;

    void print_caption(std::ofstream & fout) const
    {
        fout << '\n' << UT_LITTLE_SEPARATE_CAPTION_STRING << '\n';
        fout << _case_name << '\n';
        fout << UT_LITTLE_SEPARATE_CAPTION_STRING << '\n';
    }

  public:
    
    Test_case() :
        _case_name(""), _pfun(nullptr)
    { }

    Test_case(const Test_case & origin) :
        _case_name(origin._case_name), _pfun(origin._pfun)
    { }

    Test_case(const std::string & case_name, testing_fun_t pfun) :
        _case_name(case_name), _pfun(pfun)
    { }

    Test_case(Test_case && rval_ref) :
        Test_case()
    {
        std::swap(_case_name, rval_ref._case_name);
        std::swap(_pfun, rval_ref._pfun);
    }

    ~Test_case() noexcept
    { }

    const Test_case & operator=(const Test_case & another)
    {
        _case_name = another._case_name;
        _pfun = another._pfun;
        
        return (*this);
    }

    void run(std::ofstream & fout) const
    {
        try{
            print_caption(fout);
            _pfun(fout);
        }
        catch(ctl::exception & exc){
            fout << exc.error_message() << '\n';
        }
        catch(...){
            fout << "Unexpected exception in 'ut::Test_case::run()'.\n";
        }
    }

};



class Section final
{

  private:

    std::string   _section_name;
    std::ofstream _fout;
    Test_case *   _cases;
    size_t        _ncases;

    void print_caption(void) noexcept
    {
        _fout << UT_HUGE_SEPARATE_CAPTION_STRING << '\n';
        _fout << _section_name << '\n';
        _fout << UT_HUGE_SEPARATE_CAPTION_STRING << "\n";
    }

  public:

    Section(const std::string & section_name, const std::string & fout_name, std::initializer_list<Test_case> case_ilist) :
        _section_name(section_name), _cases(nullptr), _ncases(0)
    {
        _fout.open(fout_name);
        if(!_fout.is_open())
            throw ctl::exception("Error during opening file: \"" + fout_name + "\"", "ut::Section::constructor()");

        for(auto it = case_ilist.begin(); it != case_ilist.end(); ++it)
            ++_ncases;
        try{
            _cases = new Test_case[_ncases];
            size_t i = 0;
            for(auto elem : case_ilist)
                _cases[i++] = std::move(elem);
        }
        catch(std::bad_alloc & ba_exc){
            throw ctl::exception("Memory allocating error (std::bad_alloc)", "ut::Section::constructor()");
        }
        catch(...){
            throw ctl::exception("Unexpected exception", "ut::Section::constructor()");
        }
    }

    ~Section() noexcept
    {
        delete[] _cases;
        _fout.close();
    }

    void run(void)
    {
        try{
            print_caption();
            for(size_t i = 0; i < _ncases; ++i)
                _cases[i].run(_fout);
        }
        catch(ctl::exception & exc){
            throw exc;
        }
        catch(...){
            throw ctl::exception("Unexpected exception", "ut::Section::run()");
        }
    }

};

} /* namespace ut */
