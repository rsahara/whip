//
//  Text.cpp
//  Tester
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Text.hpp"

#include "Whip.hpp"

using namespace Whip;

int testLowerCamelCaseIdentifier(int argc, const char* argv[]) {
    if (argc != 3) {
        errorLog() << "arguments count." << std::endl;
        return 1;
    }
    std::string identifier(argv[2]);
    infoLog() << lowerCamelCaseIdentifier(identifier);
    return 0;
}

int testUpperCamelCaseIdentifier(int argc, const char* argv[]) {
    if (argc != 3) {
        errorLog() << "arguments count." << std::endl;
        return 1;
    }
    std::string identifier(argv[2]);
    infoLog() << upperCamelCaseIdentifier(identifier);
    return 0;
}

int testIdentifierCapitalizedWithUnderscores(int argc, const char* argv[]) {
    if (argc != 3) {
        errorLog() << "arguments count." << std::endl;
        return 1;
    }
    std::string identifier(argv[2]);
    infoLog() << identifierCapitalizedWithUnderscores(identifier);
    return 0;
}
