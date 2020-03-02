//
//  Scanner.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Scanner.hpp"

#include <sstream>

#include "Util/Debug.hpp"
#include "Util/Text.hpp"

namespace Whip::PseudoCodeLanguage {
    Scanner::Scanner(const char* inputText)
    : inputText(inputText)
    , tokens()
    , text(inputText)
    , line(1) {
    }

    bool Scanner::scan(int firstLine) {
        line = firstLine;
        text = inputText;

        tokens.clear();
        tokens.reserve(256);
        tokens.emplace_back(TokenType::Begin, line);

        scanWhitespaces();
        if (*text != 0 && !scanExpression()) {
            return false;
        }
        scanWhitespaces();
        while (*text != 0) {
            if (!scanAfterExpression()) {
                return false;
            }
        }
        tokens.emplace_back(TokenType::End, line);
        return true;
    }

    bool Scanner::scanError(TokenType expectedType) {
        const Token& lastToken = tokens.back();
        if (lastToken.type == TokenType::Error) {
            return false;
        }
        errorLog() << "line " << line << ": after: " << tokenTypeDescription(lastToken.type);
        if (!lastToken.text.empty()) {
            errorLog() << " \"" << lastToken.text << "\"";
        }
        errorLog() << ", expected: \"" << tokenTypeDescription(expectedType) << "\"." << std::endl;
        tokens.emplace_back(TokenType::Error, line);
        return false;
    }

    bool Scanner::scanWhitespaces() {
        const char* start = text;
        for (;;) {
            switch (*text) {
                case ' ':
                case '\t':
                    text++;
                    break;

                case '\n':
                    text++;
                    line++;
                    break;

                default:
                    return start != text;
            }
        }
    }

    bool Scanner::scanWord() {
        scanWhitespaces();
        const char* start = text;

        switch (*text) {
            case 'A' ... 'Z':
            case 'a' ... 'z':
            case '_':
                text++;
                break;
            default:
                return false;
        }

        for (;;) {
            switch (*text) {
                case 'A' ... 'Z':
                case 'a' ... 'z':
                case '0' ... '9':
                case '_':
                    text++;
                    break;
                default:
                    tokens.emplace_back(TokenType::Word, line, std::string_view(start, text - start));
                    return true;
            }
        }
    }

    bool Scanner::scanNumber() {
        scanWhitespaces();
        const char* start = text;

        switch (*text) {
            case '0' ... '9':
                text++;
                break;
            default:
                return false;
        }
        for (;;) {
            switch (*text) {
                case 'A' ... 'Z':
                case 'a' ... 'z':
                case '0' ... '9':
                case ',':
                case '.':
                    text++;
                    break;
                default:
                    tokens.emplace_back(TokenType::Number, line, std::string_view(start, text - start));
                    return true;
            }
        }
    }

    bool Scanner::scanString() {
        const char* start = text;
        for (;;) {
            switch (*text) {
                case '"':
                    tokens.emplace_back(TokenType::StringContent, line, std::string_view(start, text - start));
                    text++;
                    tokens.emplace_back(TokenType::StringEnd, line);
                    return true;

                case 0:
                case '\n':
                    return scanError(TokenType::StringEnd);

                default:
                    text++;
                    break;
            }
        }
    }

    bool Scanner::scanExpression() {
        scanWhitespaces();

        if (scanText(text, "!")) {
            tokens.emplace_back(TokenType::LogicalNegation, line, 1);
            return scanExpression();
        }

        if (scanText(text, "(")) {
            tokens.emplace_back(TokenType::ParenthesisOpen, line);
            if (!scanExpression()) {
                return false;
            }
            for (;;) {
                scanWhitespaces();
                if (scanText(text, ")")) {
                    tokens.emplace_back(TokenType::ParenthesisClose, line);
                    return true;
                }
                if (!scanAfterExpression()) {
                    return false;
                }
            }
        }

        if (scanText(text, "\"")) {
            tokens.emplace_back(TokenType::StringBegin, line);
            return scanString();
        }

        if (scanNumber()) {
            return true;
        }

        if (!scanWord()) {
            return scanError(TokenType::End);
        }

        // After a word
        for (;;) {
            scanWhitespaces();

            // Check if the word was a scope.
            if (scanText(text, ".")) {
                tokens.emplace_back(TokenType::ScopeSeparator, line, 2);
                if (!scanWord()) {
                    return false;
                }
                continue;
            }

            // Check if the word is a function call.
            if (scanText(text, "(")) {
                int functionCallTokenIndex = (int)tokens.size() - 1;
                Token& functionCallToken = tokens[functionCallTokenIndex];
                ASSERT(functionCallToken.type == TokenType::Word, "unexpected token");
                functionCallToken.type = TokenType::FunctionCall;
                tokens.emplace_back(TokenType::ArgBegin, line);
                return scanFunctionCallArguments(functionCallTokenIndex);
            }

            return true;
        }
    }

    bool Scanner::scanAfterExpression() {
        scanWhitespaces();
        if (scanText(text, "&&")) {
            tokens.emplace_back(TokenType::LogicalAnd, line, 2);
            return scanExpression();
        }
        if (scanText(text, "||")) {
            tokens.emplace_back(TokenType::LogicalOr, line, 2);
            return scanExpression();
        }
        if (scanText(text, "==")) {
            tokens.emplace_back(TokenType::CompareEqual, line, 2);
            return scanExpression();
        }
        if (scanText(text, "!=")) {
            tokens.emplace_back(TokenType::CompareNotEqual, line, 2);
            return scanExpression();
        }
        if (scanText(text, ">=")) {
            tokens.emplace_back(TokenType::CompareGreaterOrEqual, line, 2);
            return scanExpression();
        }
        if (scanText(text, "<=")) {
            tokens.emplace_back(TokenType::CompareGreaterOrEqual, line, 2);
            return scanExpression();
        }
        if (scanText(text, ">")) {
            tokens.emplace_back(TokenType::CompareGreater, line, 2);
            return scanExpression();
        }
        if (scanText(text, "<")) {
            tokens.emplace_back(TokenType::CompareLess, line, 2);
            return scanExpression();
        }
        if (scanText(text, ";")) {
            scanWhitespaces();
            if (*text == 0) {
                // Ignore the last sequence end.
                return true;
            }
            tokens.emplace_back(TokenType::SequenceSeparator, line, 2);
            return scanExpression();
        }

        return scanError(TokenType::End);
    }

    bool Scanner::scanFunctionCallArguments(int functionCallTokenIndex) {
        scanWhitespaces();
        if (scanText(text, ")")) {
            tokens.emplace_back(TokenType::ArgEnd, line);
            return true;
        }
        if (!scanExpression()) {
            return false;
        }
        tokens[functionCallTokenIndex].argumentCount++;
        for (;;) {
            scanWhitespaces();
            if (scanText(text, ")")) {
                tokens.emplace_back(TokenType::ArgEnd, line);
                return true;
            }
            if (scanText(text, ",")) {
                tokens.emplace_back(TokenType::ArgSeparator, line);
                if (!scanExpression()) {
                    return false;
                }
                tokens[functionCallTokenIndex].argumentCount++;
                continue;
            }
            if (!scanAfterExpression()) {
                return false;
            }
        }
    }
}
