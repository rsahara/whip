//
//  Token.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_Language_Token_hpp
#define Whip_PseudoCode_Language_Token_hpp

#include <string_view>

namespace Whip::PseudoCodeLanguage {
    enum class TokenType {
        Begin,
        End,
        Error,
        Word,
        ScopeSeparator,
        FunctionCall,
        ArgBegin,
        ArgEnd,
        StringContent,
        StringBegin,
        StringEnd,
        Number,
        ParenthesisClose,
        ParenthesisOpen,
        ArgSeparator,
        LogicalAnd,
        LogicalOr,
        LogicalNegation,
        CompareGreater,
        CompareGreaterOrEqual,
        CompareLess,
        CompareLessOrEqual,
        CompareEqual,
        CompareNotEqual,
        SequenceSeparator,
    };

    struct Token {
        inline Token() {}
        inline Token(const Token& token) { *this = token; }
        inline Token& operator=(const Token& token);
        inline Token(TokenType type, int line, int argumentCount = 0);
        inline Token(TokenType type, int line, const std::string_view& text);

        TokenType type;
        int line;

        /// Number of arguments a function or operator takes.
        int argumentCount;
        /// Text corresponding to this token.
        std::string_view text;
    };

    /// Text representation of a token type.
    const char* tokenTypeDescription(TokenType type);
}

// Inline

namespace Whip::PseudoCodeLanguage {
    inline Token& Token::operator=(const Token& token) {
        type = token.type;
        line = token.line;
        argumentCount = token.argumentCount;
        text = token.text;
        return *this;
    }

    inline Token::Token(TokenType type, int line, int argumentCount)
    : type(type)
    , line(line)
    , argumentCount(argumentCount)
    , text() {
    }

    inline Token::Token(TokenType type, int line, const std::string_view& text)
    : type(type)
    , line(line)
    , argumentCount(0)
    , text(text) {
    }
}

#endif
