//
//  Text.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Util_Text_hpp
#define Whip_Util_Text_hpp

#include <string>

// String features.
namespace Whip {
    /// Replace all occurences of `from` to `to` in `text` and returns the resulting string.
    void replaceAllOccurences(std::string& text, const std::string& from, const std::string& to);
}

// Scanner features.
namespace Whip {
    /// Checks if inputText starts with testText.
    /// If so, inputText is moved to the character after the text that matched testText.
    bool scanText(const char*& inputText, const char* testText);
}

// Code generation features.
namespace Whip {
    /// Returns a null terminated string representing the indentation of a line.
    const char* indentation(int depth);
    /// Sanitizes a camel case identifier and returns it in Lower Camel Case.
    std::string lowerCamelCaseIdentifier(const std::string& identifier);
    /// Sanitizes a camel case identifier and returns it in Upper Camel Case.
    std::string upperCamelCaseIdentifier(const std::string& identifier);
    /// Sanitizes a camel case identifier and returns it in capitalized with underscores.
    std::string identifierCapitalizedWithUnderscores(const std::string& identifier);
}

#endif
