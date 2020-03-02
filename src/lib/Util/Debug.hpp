//
//  Debug.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Util_Debug_hpp
#define Whip_Util_Debug_hpp

#include "Util/Logging.hpp"

#if DEBUG

/// Assert that lets us debug easier on Xcode.
#define ASSERT(test, format, ...) \
do { if (!(test)) { Whip::logDebugLine(format, ##__VA_ARGS__); Whip::assertFailed(__FILE__, __LINE__, __FUNCTION__); *Whip::null = 0; } } while (0)

/// Debug log.
#define LOG(format, ...) Whip::logDebugLine(format, ##__VA_ARGS__)

#else // DEBUG

#define ASSERT(test, format, ...)

#define LOG(format, ...) (void)0

#endif

namespace Whip {
#if DEBUG
    extern int* null;
    void assertFailed(const char* filename, int line, const char* function);
#else
    void internalError();
#endif // DEBUG
}


#endif
