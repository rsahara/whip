//
//  Token.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SmFormat_Token_hpp
#define Whip_Formats_SmFormat_Token_hpp

#include <string>

namespace Whip::SmFormat {
    enum class TokenType: int {
        Begin,
        End,
        Error,
        Word,
        RawLine,
        Class,
        Start,
        Include,
        Package,
        Enum,
        Map,
        MapDelimiter,
        BraceOpen,
        BraceClose,
        CondBegin,
        CondEnd,
        RawCode,
        Entry,
        Exit,
    };

    struct Token {
        inline Token() {}
        inline Token(const Token& token) { *this = token; }
        inline Token& operator=(const Token& token);
        inline Token(TokenType type, int line);
        inline Token(TokenType type, int line, const std::string_view& text);

        TokenType type;
        int line;

        /// Text corresponding to this token.
        std::string_view text;
    };

    /// Text representation of a token type.
    const char* tokenTypeDescription(TokenType type);
}

// Inline

namespace Whip::SmFormat {
    inline Token& Token::operator=(const Token& token) {
        type = token.type;
        line = token.line;
        text = token.text;
        return *this;
    }

    inline Token::Token(TokenType type, int line)
    : type(type)
    , line(line)
    , text() {
    }

    inline Token::Token(TokenType type, int line, const std::string_view& text)
    : type(type)
    , line(line)
    , text(text) {
    }
}

#endif
