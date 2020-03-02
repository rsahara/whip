//
//  Scan.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Scanner.hpp"

#include <sstream>

#include "Util/Logging.hpp"
#include "Util/Text.hpp"

namespace Whip::SmFormat {
    Scanner::Scanner(const char* inputText)
    : inputText(inputText), tokens() , text(inputText), line(1) {
    }

    bool Scanner::scan(int firstLine) {
        tokens.clear();
        text = inputText;
        line = firstLine;

        tokens.emplace_back(TokenType::Begin, line);
        while (*text && scanTokens());
        if (tokens.back().type == TokenType::Error) {
            return false;
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
        errorLog() << ", expected: " << tokenTypeDescription(expectedType) << "." << std::endl;
        tokens.emplace_back(TokenType::Error, line);
        return false;
    }
    
    void Scanner::scanLineComment() {
        for (;;) {
            switch (*text) {
                case 0:
                    return;
                case '\n':
                    text++;
                    line++;
                    return;
            }
            text++;
        }
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
                    
                case '/':
                    if (text[1] == '/') {
                        text += 2;
                        scanLineComment();
                    }
                    break;
                    
                default:
                    return start != text;
            }
        }
    }

    bool Scanner::scanRawLine() {
        scanWhitespaces();
        const char* start = text;
        for (;;) {
            switch (*text) {
                case 0:
                    tokens.emplace_back(TokenType::RawLine, line, std::string_view(start, text - start));
                    return true;
                    
                case '\n':
                    tokens.emplace_back(TokenType::RawLine, line, std::string_view(start, text - start));
                    text++;
                    line++;
                    return true;
            }
            text++;
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
                return scanError(TokenType::Word);
        }
        
        for (;;) {
            switch (*text) {
                case 'A' ... 'Z':
                case 'a' ... 'z':
                case '0' ... '9':
                case '_':
                case ':':
                case '.':
                    text++;
                    break;
                default:
                    tokens.emplace_back(TokenType::Word, line, std::string_view(start, text - start));
                    return true;
            }
        }
    }
    
    bool Scanner::scanConditionCodeIfAny() {
        const char* start = text;
        for (;;) {
            switch (*text) {
                case '\n':
                    line++;
                    break;

                case ']':
                case 0:
                    if (start == text) {
                        return false;
                    }
                    tokens.emplace_back(TokenType::RawCode, line, std::string_view(start, text - start));
                    return true;
            }
            text++;
        }
        return false;
    }
    
    bool Scanner::scanBlockOfCode() {
        scanWhitespaces();
        if (!scanText(text, "{")) {
            return scanError(TokenType::BraceOpen);
        }
        tokens.emplace_back(TokenType::BraceOpen, line);

        const char* start = text;
        int startLine = line;
        for (;;) {
            switch (*text) {
                case '\n':
                    line++;
                    break;
                case 0:
                    return scanError(TokenType::BraceClose);
                case '}':
                    tokens.emplace_back(TokenType::RawCode, startLine, std::string_view(start, text - start));
                    text++;
                    tokens.emplace_back(TokenType::BraceClose, line);
                    return true;
            }
            text++;
        }
    }
    
    bool Scanner::scanTransition() {
        if (!scanWord()) {
            return false;
        }
        
        scanWhitespaces();
        if (scanText(text, "[")) {
            tokens.emplace_back(TokenType::CondBegin, line);
            scanConditionCodeIfAny();
            scanWhitespaces();
            if (!scanText(text, "]")) {
                return scanError(TokenType::CondEnd);
            }
            tokens.emplace_back(TokenType::CondEnd, line);
        }
        
        if (!scanWord()) {
            return false;
        }
        
        return scanBlockOfCode();
    }
    
    bool Scanner::scanState() {
        if (!scanWord())
            return false;
        
        scanWhitespaces();
        if (scanText(text, "Entry")) {
            tokens.emplace_back(TokenType::Entry, line);
            if (!scanBlockOfCode()) {
                return false;
            }
            scanWhitespaces();
        }
        
        if (scanText(text, "Exit")) {
            tokens.emplace_back(TokenType::Exit, line);
            if (!scanBlockOfCode()) {
                return false;
            }
            scanWhitespaces();
        }
        
        if (!scanText(text, "{")) {
            return scanError(TokenType::BraceOpen);
        }
        tokens.emplace_back(TokenType::BraceOpen, line);
        
        for (;;) {
            scanWhitespaces();
            if (scanText(text, "}")) {
                tokens.emplace_back(TokenType::BraceClose, line);
                return true;
            }
            if (!scanTransition()) {
                return false;
            }
        }
    }

    bool Scanner::scanMap() {
        scanWhitespaces();
        if (!scanText(text, "%%")) {
            return scanError(TokenType::MapDelimiter);
        }
        tokens.emplace_back(TokenType::MapDelimiter, line);
        
        if (!scanState()) {
            return false;
        }
        
        for (;;) {
            scanWhitespaces();
            if (scanText(text, "%%")) {
                tokens.emplace_back(TokenType::MapDelimiter, line);
                return true;
            }
            if (!scanState()) {
                return false;
            }
        }
    }

    bool Scanner::scanTokens() {
        if (scanWhitespaces()) {
            return true;
        }
        if (scanText(text, "%start")) {
            tokens.emplace_back(TokenType::Start, line);
            return scanWord();
        }
        if (scanText(text, "%class")) {
            tokens.emplace_back(TokenType::Class, line);
            return scanWord();
        }
        if (scanText(text, "%include")) {
            tokens.emplace_back(TokenType::Include, line);
            return scanRawLine();
        }
        if (scanText(text, "%package")) {
            tokens.emplace_back(TokenType::Package, line);
            return scanRawLine();
        }
        if (scanText(text, "%enum")) {
            tokens.emplace_back(TokenType::Enum, line);
            return scanWord();
        }
        if (scanText(text, "%map")) {
            tokens.emplace_back(TokenType::Map, line);
            return scanWord() && scanMap();
        }
        if (*text != 0) {
            return scanError(TokenType::End);
        }
        
        return false;
    }
}
