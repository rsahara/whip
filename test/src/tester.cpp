//
//  tester.cpp
//  Tester
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include <string_view>
#include <iostream>

#include "Util/Logging.hpp"

#include "tester/Text.hpp"

using namespace Whip;

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        errorLog() << "no arguments." << std::endl;
        return 1;
    }

    std::string_view firstArgument(argv[1]);
    if (firstArgument == "lowerCamelCaseIdentifier") {
        return testLowerCamelCaseIdentifier(argc, argv);
    }
    else if (firstArgument == "upperCamelCaseIdentifier") {
        return testUpperCamelCaseIdentifier(argc, argv);
    }
    else if (firstArgument == "identifierCapitalizedWithUnderscores") {
        return testIdentifierCapitalizedWithUnderscores(argc, argv);
    }

    // Unknown test.
    errorLog() << "unknown test." << std::endl;
    return 1;
}
