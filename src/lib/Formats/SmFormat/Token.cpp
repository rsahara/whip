//
//  Token.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Token.hpp"

namespace Whip::SmFormat {
    const char* tokenTypeDescription(TokenType type) {
        switch (type) {
            case TokenType::Begin: return "beginning of document";
            case TokenType::End: return "end of document";
            case TokenType::Error: return "error";
            case TokenType::Word: return "word";
            case TokenType::RawLine: return "raw line";
            case TokenType::Class: return "\"%class\" keyword";
            case TokenType::Start: return "\"%start\" keyword";
            case TokenType::Include: return "\"%include\" keyword";
            case TokenType::Package: return "\"%package\" keyword";
            case TokenType::Enum: return "\"%enum\" keyword";
            case TokenType::Map: return "\"%map\" keyword";
            case TokenType::MapDelimiter: return "\"%%\" map delimiter";
            case TokenType::BraceOpen: return "\"{\" brace open";
            case TokenType::BraceClose: return "\"}\" brace close";
            case TokenType::CondBegin: return "\"[\" condition begin";
            case TokenType::CondEnd: return "\"]\" condition end";
            case TokenType::RawCode: return "raw code";
            case TokenType::Entry: return "\"Entry\" keyword";
            case TokenType::Exit: return "\"Exit\" keyword";
        }
        return "(unexpected)";
    }
}
