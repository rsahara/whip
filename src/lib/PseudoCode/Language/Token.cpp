//
//  Token.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Token.hpp"

namespace Whip::PseudoCodeLanguage {

    const char* tokenTypeDescription(TokenType type) {
        switch (type) {
            case TokenType::Begin: return "beginning of pseudo code";
            case TokenType::End: return "end of pseudo code";
            case TokenType::Error: return "error";
            case TokenType::Word: return "word";
            case TokenType::ScopeSeparator: return "scope separator";
            case TokenType::FunctionCall: return "function call";
            case TokenType::ArgBegin: return "\"(\" function arg list begin";
            case TokenType::ArgEnd: return "\")\" function arg list end";
            case TokenType::StringBegin: return "string begin";
            case TokenType::StringContent: return "string content";
            case TokenType::StringEnd: return "string end";
            case TokenType::Number: return "number";
            case TokenType::ParenthesisOpen: return "\"(\" parenthesis";
            case TokenType::ParenthesisClose: return "\")\" parenthesis";
            case TokenType::ArgSeparator: return "\",\" argument separator";
            case TokenType::LogicalAnd: return "\"&&\" logical operator";
            case TokenType::LogicalOr: return "\"||\" logical operator";
            case TokenType::LogicalNegation: return "\"!\" logical operator";
            case TokenType::CompareGreater: return "\">\" comparison operator";
            case TokenType::CompareGreaterOrEqual: return "\">=\" comparison operator";
            case TokenType::CompareLess: return "\"<\" comparison operator";
            case TokenType::CompareLessOrEqual: return "\">=\" comparison operator";
            case TokenType::CompareEqual: return "\"==\" comparison operator";
            case TokenType::CompareNotEqual:  return "\"!=\" comparison operator";
            case TokenType::SequenceSeparator:  return "\";\" sequence separator";
        }
        return "(unexpected)";
    }
}
