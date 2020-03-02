//
//  Logging.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Util_Logging_hpp
#define Whip_Util_Logging_hpp

#include <ostream>

namespace Whip {
    /// Output stream to use to log error messages.
    std::ostream& errorLog();
    /// Output stream to use to log some information.
    std::ostream& infoLog();
    /// Output stream to use to log debug messages.
    std::ostream& debugLog();

    /// Logs a line of error message.
    void logErrorLine(const char* format, ...);
    /// Logs a line of information.
    void logInfoLine(const char* format, ...);
    /// Logs a line of debug message.
    void logDebugLine(const char* format, ...);
}

#endif
