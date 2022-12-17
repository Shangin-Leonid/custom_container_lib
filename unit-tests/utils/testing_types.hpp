#pragma once

#include <fstream>


namespace ut { /* 'ut' = unit-tests */


typedef void (*testing_fun_t) (std::ofstream & fout);


} /* namespace ut */
