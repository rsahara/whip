//
//  Scanner.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SmFormat_Scanner_hpp
#define Whip_Formats_SmFormat_Scanner_hpp

#include <vector>

#include "Formats/SmFormat/Token.hpp"

namespace Whip::SmFormat {
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
        /// Scans a line of comment (skips until EndOfLine/EndOfString).
        void scanLineComment();
        /// Scans white spaces.
        bool scanWhitespaces();
        /// Scans a line of text (until EndOfLine/EndOfString).
        bool scanRawLine();
        /// Scans a word.
        bool scanWord();
        /// Scans the code inside a guard condition.
        bool scanConditionCodeIfAny();
        /// Scans the definition of a block of actions executed on transition/state entry/state exit.
        bool scanBlockOfCode();
        /// Scans the definition of a transition in the current state.
        bool scanTransition();
        /// Scans the definition of a state, with Entry/Exit actions and transitions.
        bool scanState();
        /// Scans the definition of a map of states.
        bool scanMap();
        /// Scans any type of tokens that we can expect.
        bool scanTokens();

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
