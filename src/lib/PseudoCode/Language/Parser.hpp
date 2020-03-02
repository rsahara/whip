//
//  Parser.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_Language_Parser_hpp
#define Whip_PseudoCode_Language_Parser_hpp

#include <stack>

#include "Document/Document.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCode/Language/Token.hpp"

namespace Whip::PseudoCodeLanguage {
    /// Helper class that parses an array of tokens and creates a vector of expressions.
    /// The array must begin with a TokenType::Begin token and end with TokenType::End token.
    /// The expressions are parsed using the Shunting-yard algorithm.
    struct Parser {
        Parser(std::vector<Whip::Expression>& expressions, const Token* firstToken);

        /// Parses the provided tokens.
        bool parse();

    private:
        /// Parses any type of tokens that we can expect.
        bool parseTokens();
        /// Pops an operation from the stack.
        void popOperationStack();
        /// Pops operations from the stack, until a given token type is found.
        void popOperationStackUntilTokenType(TokenType tokenType);
        /// Pushes an operation to the stack.
        void pushOperationStack(const Token& token);

        /// Returns the expression type that describes a given token type.
        static Whip::ExpressionType expressionTypeForTokenType(TokenType tokenType);
        /// Returns the precedence for a token type.
        /// Returns 0 when it's not an operator that should be pushed/popped automatically by comparing the precedences.
        static int precedenceForTokenType(TokenType tokenType);

    private:
        /// Vector where we store the expressions we identified.
        std::vector<Whip::Expression>& expressions;
        /// First token of the input.
        const Token* firstToken;

        /// Current token that we're parsing in the input.
        const Token* token;

        typedef std::stack<const Token*> TokenStack;
        /// Stack of operations we'll use in the Shunting-yard algorithm.
        TokenStack operationStack;
    };
}

#endif
