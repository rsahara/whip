//
//  Console.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Logging.hpp"

#include <cstdarg>
#include <cstdio>
#include <iostream>

namespace Whip {
    std::ostream& errorLog() {
        return std::cerr;
    }

    std::ostream& infoLog() {
        return std::cout;
    }

    std::ostream& debugLog() {
        return std::cout;
    }

    void logErrorLine(const char* format, ...) {
        fprintf(stderr, "error: ");
        va_list args;
        va_start(args, format);
        vfprintf(stderr, format, args);
        va_end(args);
        fprintf(stderr, "\n");
        fflush(stderr);
    }
    
    void logInfoLine(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fprintf(stdout, "\n");
        fflush(stdout);
    }
    
    void logDebugLine(const char* format, ...) {
        fprintf(stdout, "debug: ");
        va_list args;
        va_start(args, format);
        vfprintf(stdout, format, args);
        va_end(args);
        fprintf(stdout, "\n");
        fflush(stdout);
    }
}
