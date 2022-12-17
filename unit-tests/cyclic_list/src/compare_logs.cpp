#include <fstream>
#include <iostream>

#include "../../utils/compare_files.hpp"

using namespace ut;

int main(int n_files, char * file_names[])
{
    if(n_files != 3){
        std::cerr << "You must send exactly 2 file names: for 'std' and 'ctl' containers output logs.\n";
        return -1;
    }

    std::ifstream f1,
                 f2;
    f1.open(file_names[1]);
    f2.open(file_names[2]);
    if(!f1.is_open() || !f2.is_open())
        std::cerr << "At least one of input files (\"" << file_names[1] << "\" or \"" << file_names[2] << "\") is closed.\n";

    std::cout << (are_same_files(f1, f2) ? "PASSED" : "FAILED");

    return 0;
}