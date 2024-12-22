#pragma once

#include <iostream>

void assert_impl(bool cond, const char* cond_str, const char* file, int line) {
    if (!cond) {
        std::cerr << file << ":" << line << " TEST FAILED: condition '" << cond_str << "' is expected to be true\n";
        throw std::runtime_error("ASSERT FAILED");
    }
}

#define ASSERT(cond) \
    do { assert_impl((cond), #cond, __FILE__, __LINE__); } while(false)
