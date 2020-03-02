//
//  Scanner.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_Language_Scanner_hpp
#define Whip_PseudoCode_Language_Scanner_hpp

#include <vector>

#include "PseudoCode/Language/Token.hpp"

namespace Whip::PseudoCodeLanguage {
    /// Helper class that scans a null terminated string.
    struct Scanner {
        Scanner(const char* inputText);

        /// Scans the provided text.
        /// Returns false if there is an error.
        /// firstLine will be used as the line number of the first line.
        bool scan(int firstLine = 1);

        /// Tokens that we scanned.
        /// The first token is a token of type TokenType::Begin.
        /// If the tokens were scanned successfully, the last token is of type TokenType::End.
        /// In case of error, the last token is of type Token::Error.
        const std::vector<Token>& result() const { return tokens; }

    private:
        /// Pushes a token that indicates an error.
        bool scanError(TokenType expectedType);
        /// Scans white spaces.
        bool scanWhitespaces();
        /// Scans a word.
        bool scanWord();
        /// Scans a number.
        bool scanNumber();
        /// Scans the content of a string including the end of string delimiter.
        bool scanString();
        /// Scans an expression.
        bool scanExpression();
        /// Scans tokens that can be after an expression.
        bool scanAfterExpression();
        // Scans the list of arguments of a function call.
        bool scanFunctionCallArguments(int functionCallTokenIndex);

    private:
        /// First character of the input text.
        const char* inputText;
        /// Vector of tokens we have scanned so far.
        std::vector<Token> tokens;
        /// Current character we're reading in the input text.
        const char* text;
        /// Current line.
        int line;
    };
}

#endif
