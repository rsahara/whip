//
//  TextUtil.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Text.hpp"

#include <cctype>

namespace Whip {
    void replaceAllOccurences(std::string& text, const std::string& from, const std::string& to) {
        size_t pos = text.find(from, 0);
        while (pos != std::string::npos) {
            text.replace(pos, from.size(), to);
            pos += to.length();
            pos = text.find(from, pos);
        }
    }
}
namespace Whip {
    bool scanText(const char*& inputText, const char* testText) {
        for (const char* ptr = inputText; *ptr == *testText;) {
            testText++;
            ptr++;
            if (*testText == 0) {
                inputText = ptr;
                return true;
            }
        }
        return false;
    }
}

namespace Whip {
    const char* indentation(int depth) {
        // Buffer of spaces with 8 indentations. Should be enough.
        static const char spaceArray[] = "                                ";
        static const int spaceArraySize = sizeof(spaceArray) - 1;

        const char* result = spaceArray + (spaceArraySize - 4 * depth);
        if (result < spaceArray) {
            // Give up with the max indentations.
            return spaceArray;
        }
        return result;
    }

    std::string lowerCamelCaseIdentifier(const std::string& identifier) {
        // Allow only alphanumeric characters.
        std::string result;
        bool beginningOfWord = false;
        for (char character: identifier) {
            if (isalnum(character)) {
                if (beginningOfWord) {
                    result += toupper(character);
                    beginningOfWord = false;
                }
                else {
                    result += character;
                }
            }
            else {
                beginningOfWord = true;
            }
        }

        // If the first character is not upper cased, we're done.
        if (result.empty() || !isupper(result[0])) {
            return result;
        }

        // Otherwise, the first characters must be lower cased, until we find the first lower cased character.
        // Ex.: "HTTPQuery" -> "httpQuery".
        result[0] = tolower(result[0]);
        int endIndex = (int)result.size() - 1;
        int index = 1;
        for (; index <= endIndex && isupper(result[index]); index++) {
            result[index - 1] = tolower(result[index - 1]);
        }
        if (index == (int)result.size()) {
            result[index - 1] = tolower(result[index - 1]);
        }
        return result;
    }

    std::string upperCamelCaseIdentifier(const std::string& identifier) {
        // Allow only alphanumeric characters.
        std::string result;
        bool beginningOfWord = false;
        for (char character: identifier) {
            if (isalnum(character)) {
                if (beginningOfWord) {
                    result += toupper(character);
                    beginningOfWord = false;
                }
                else {
                    result += character;
                }
            }
            else {
                beginningOfWord = true;
            }
        }

        // The first character has to be upper cased.
        if (result.empty() || isupper(result[0])) {
            return result;
        }
        result[0] = toupper(result[0]);
        return result;
    }

    std::string identifierCapitalizedWithUnderscores(const std::string& identifier) {
        // Allow only alphanumeric characters.
        std::string temp(identifier);
        bool beginningOfWord = false;
        for (char& character: temp) {
            if (isalnum(character)) {
                if (beginningOfWord) {
                    character = toupper(character);
                    beginningOfWord = false;
                }
            }
            else {
                beginningOfWord = true;
            }
        }

        if (temp.empty()) {
            return temp;
        }

        // The first character is always upper cased.
        std::string result;
        bool isUpper = isupper(temp[0]);
        result += toupper(temp[0]);
        if (temp.size() == 1) {
            return result;
        }

        if (temp.size() >= 2) {
            bool nextIsUpper = isupper(temp[1]);
            if (!isUpper && nextIsUpper) {
                result += '_';
            }
        }

        // Detect case changes and insert '_' when needed.
        // Ex.: "HTTPQuery" -> "HTTP_QUERY"
        // Ex.: "httpQuery" -> "HTTP_QUERY"
        // Ex.: "doSomething" -> "DO_SOMETHING"
        int endIndex = (int)temp.size() - 1;
        isUpper = isupper(temp[1]);
        bool skipNext = false;
        for (int index = 1; index < endIndex; index++) {
            bool nextIsUpper = isupper(temp[index + 1]);
            if (isUpper != nextIsUpper) {
                if (isUpper) {
                    if (skipNext) {
                        skipNext = false;
                        result += toupper(temp[index]);
                    } else {
                        result += '_';
                        result += toupper(temp[index]);
                    }
                }
                else {
                    result += toupper(temp[index]);
                    result += '_';
                    skipNext = true;
                }
            }
            else {
                skipNext = false;
                result += toupper(temp[index]);
            }
            isUpper = nextIsUpper;
        }

        // The last character is always upper cased.
        result += toupper(temp[endIndex]);
        return result;
    }
}
