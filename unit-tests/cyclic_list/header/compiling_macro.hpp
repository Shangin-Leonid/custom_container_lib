#pragma once

#include <iostream>
#include <utility>
#include <string>
#include <vector>

#include UT_INCLUDED_LIB_HEADER

template class UT_CONTAINER_NAME<int>;
template class UT_CONTAINER_NAME<int *>;
template class UT_CONTAINER_NAME<const int *>;

template class UT_CONTAINER_NAME<uint16_t>;
template class UT_CONTAINER_NAME<uint16_t *>;
template class UT_CONTAINER_NAME<const uint16_t *>;

template class UT_CONTAINER_NAME<uint32_t>;
template class UT_CONTAINER_NAME<uint32_t *>;
template class UT_CONTAINER_NAME<const uint32_t *>;

template class UT_CONTAINER_NAME<uint64_t>;
template class UT_CONTAINER_NAME<uint64_t *>;
template class UT_CONTAINER_NAME<const uint64_t *>;

template class UT_CONTAINER_NAME<std::pair<int, uint>>;
template class UT_CONTAINER_NAME<std::pair<uint, int>>;
template class UT_CONTAINER_NAME<std::pair<std::pair<float, bool>, double>>;

template class UT_CONTAINER_NAME<char>;
template class UT_CONTAINER_NAME<char *>;
template class UT_CONTAINER_NAME<const char *>;

template class UT_CONTAINER_NAME<double>;
template class UT_CONTAINER_NAME<double *>;
template class UT_CONTAINER_NAME<const double *>;

template class UT_CONTAINER_NAME<std::string>;

template class UT_CONTAINER_NAME<std::vector<std::string>>;

template class UT_CONTAINER_NAME<UT_CONTAINER_NAME<int>>;

template class UT_CONTAINER_NAME<UT_CONTAINER_NAME<std::string>>;
template class UT_CONTAINER_NAME<UT_CONTAINER_NAME<UT_CONTAINER_NAME<std::string>>>;


int main(void)
{
    std::cout << "Compiling tests: PASSED\n\n\n";

    return 0;
}
