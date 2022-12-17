#pragma once

#include <fstream>
#include <string>
#include <chrono>
#include <ratio>

#include "testing_types.hpp"


namespace ut { /* 'ut' = unit-tests */


using rep_t = double;

using nanoseconds_t  = std::chrono::duration<rep_t, std::nano>;
using microseconds_t = std::chrono::duration<rep_t, std::micro>;
using milliseconds_t = std::chrono::duration<rep_t, std::milli>;
using seconds_t      = std::chrono::duration<rep_t, std::ratio<1, 1>>;
using minutes_t      = std::chrono::duration<rep_t, std::ratio<60>>;
using hours_t        = std::chrono::duration<rep_t, std::ratio<3600>>;

using clock = std::conditional< std::chrono::high_resolution_clock::is_steady,
                                std::chrono::high_resolution_clock,
                                std::chrono::steady_clock >::type;


/*
    Clocks the time that 'func(fout)' is working. 'func()' must has 'ctl::test_fun_t' type.
    Template parameter means the unit of time of clocking.
    Returns amount of ticks (corresponds to the unit of time in template parameter).
*/
template <typename Time_t = milliseconds_t>
std::chrono::duration<rep_t>::rep clock_perfomance(testing_fun_t func, std::ofstream & fout)
{
    const clock::time_point start_mark = clock::now();
    func(fout);
    const clock::time_point end_mark = clock::now();

    return std::chrono::duration_cast<Time_t>(end_mark - start_mark).count();
}


/* Convers tickes to std::string with number of tickes and a unit of time. */
template <typename Time_t = milliseconds_t>
std::string time_to_string(std::chrono::duration<rep_t>::rep reps)
{
    constexpr const char * unit_of_time = (std::is_same<Time_t, nanoseconds_t>::value  ? "ns"  :
                                          (std::is_same<Time_t, microseconds_t>::value ? "us"  :
                                          (std::is_same<Time_t, milliseconds_t>::value ? "ms"  :
                                          (std::is_same<Time_t, seconds_t>::value      ? "s"   :
                                          (std::is_same<Time_t, minutes_t>::value      ? "min" :
                                          (std::is_same<Time_t, hours_t>::value        ? "h"   : "conventional unit"))))));

    return std::to_string(reps) + " " + unit_of_time;
}


} /* namespace ut */
