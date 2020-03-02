//
//  Debug.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Debug.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace Whip {
#if DEBUG
    int* null = nullptr;

    void assertFailed(const char* filename, int line, const char* function) {
        LOG("assertion failed\n\tFile: %s\n\tLine: %d in function: %s", filename, line, function);
    }
#else
    void internalError() {
        LOG("internal error");
        std::exit(1);
    }
#endif // DEBUG
}
