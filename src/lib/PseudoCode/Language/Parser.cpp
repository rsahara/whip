//
//  Parser.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Parser.hpp"

#include "Util/Debug.hpp"

#define ENABLE_PARSELOG  0

#if ENABLE_PARSELOG
#define PARSELOG(format, ...) LOG("parse: " format, ##__VA_ARGS__)
#else
#define PARSELOG(format, ...) (void)0
#endif

namespace Whip::PseudoCodeLanguage {
    Parser::Parser(std::vector<Expression>& expressions, const Token* firstToken)
    : expressions(expressions)
    , firstToken(firstToken)
    , token(firstToken)
    , operationStack() {
    }

    bool Parser::parse() {
        token = firstToken;
        expressions.clear();
        operationStack = TokenStack();

        while (parseTokens());

        return token->type == TokenType::End;
    }

    bool Parser::parseTokens() {
        switch (token->type) {
            case TokenType::Error:
            case TokenType::StringContent:
            case TokenType::StringEnd:
            case TokenType::ArgBegin:
                ASSERT(false, "unexpected token");
                return false;

            case TokenType::Begin:
                pushOperationStack(*token);
                token++;
                break;
            case TokenType::End:
                popOperationStackUntilTokenType(TokenType::Begin);
                popOperationStack();
                return false;
            case TokenType::StringBegin: {
                token++;
                std::string tokenText(token->text);
                ASSERT(token->type == TokenType::StringContent, "unexpected token");
                expressions.push_back(Expression(ExpressionType::String, tokenText));
                PARSELOG("> output: %s [%s]", tokenTypeDescription(token->type), tokenText.c_str());
                token += 2; // Skip StringEnd
            }
                break;
            case TokenType::Number: {
                std::string tokenText(token->text);
                expressions.push_back(Expression(ExpressionType::Number, tokenText));
                PARSELOG("> output: %s [%s]", tokenTypeDescription(token->type), tokenText.c_str());
                token++;
            }
                break;

            case TokenType::Word: {
                std::string tokenText(token->text);
                expressions.push_back(Expression(ExpressionType::Word, tokenText));
                PARSELOG("> output: %s [%s]", tokenTypeDescription(token->type), tokenText.c_str());
                token++;
            }
                break;
            case TokenType::FunctionCall:
                pushOperationStack(*token);
                token++;
                ASSERT(token->type == TokenType::ArgBegin, "unexpected token");
                pushOperationStack(*token);
                token++;
                break;
            case TokenType::ParenthesisOpen:
            case TokenType::LogicalAnd:
            case TokenType::LogicalOr:
            case TokenType::LogicalNegation:
            case TokenType::CompareGreater:
            case TokenType::CompareGreaterOrEqual:
            case TokenType::CompareLess:
            case TokenType::CompareLessOrEqual:
            case TokenType::CompareEqual:
            case TokenType::CompareNotEqual:
            case TokenType::SequenceSeparator:
            case TokenType::ScopeSeparator:
                pushOperationStack(*token);
                token++;
                break;

            case TokenType::ParenthesisClose:
                popOperationStackUntilTokenType(TokenType::ParenthesisOpen);
                popOperationStack();
                token++;
                break;
            case TokenType::ArgSeparator:
                popOperationStackUntilTokenType(TokenType::ArgBegin);
                token++;
                break;

            case TokenType::ArgEnd:
                popOperationStackUntilTokenType(TokenType::ArgBegin);
                popOperationStack();
                // Pops TokenType::FunctionCall
                popOperationStack();
                token++;
                break;
        }
        return true;
    }

    void Parser::popOperationStack() {
        const Token& token = *operationStack.top();
        ExpressionType expressionType = expressionTypeForTokenType(token.type);
        std::string tokenText(token.text);
        if (expressionType != ExpressionType::Invalid) {
            PARSELOG("> output: %s [%s][%d]", tokenTypeDescription(token.type), tokenText.c_str(), token.argumentCount);
            expressions.push_back(Expression(expressionType, tokenText, token.argumentCount));
        }

        PARSELOG("- pop: %s [%s]", tokenTypeDescription(token.type), token.text.c_str());
        operationStack.pop();
    }

    void Parser::popOperationStackUntilTokenType(TokenType tokenType) {
        PARSELOG("- (popalluntil: %s)", tokenTypeDescription(tokenType));
        ASSERT(!operationStack.empty(), "empty stack looking for %d", tokenType);
        while (operationStack.top()->type != tokenType) {
            popOperationStack();
            ASSERT(!operationStack.empty(), "empty stack looking for %d", tokenType);
        }
        PARSELOG("- (popalluntil done)");
    }

    void Parser::pushOperationStack(const Token& token) {
        int precedence = precedenceForTokenType(token.type);
        if (precedence != 0) {
            // Pop operations with a lower or equal precedence.
            PARSELOG("+ (going to push: %s [%s])", tokenTypeDescription(token.type), token.text.c_str());
            while (!operationStack.empty() && precedenceForTokenType(operationStack.top()->type) >= precedence) {
                popOperationStack();
            }
        }
        PARSELOG("+ push: %s [%s]", tokenTypeDescription(token.type), token.text.c_str());
        operationStack.push(&token);
    }

    ExpressionType Parser::expressionTypeForTokenType(TokenType tokenType) {
        switch (tokenType) {
            case TokenType::Begin:
            case TokenType::End:
            case TokenType::Error:
            case TokenType::StringBegin:
            case TokenType::StringEnd:
            case TokenType::ArgBegin:
            case TokenType::ArgEnd:
            case TokenType::ParenthesisOpen:
            case TokenType::ParenthesisClose:
            case TokenType::ArgSeparator:
                return ExpressionType::Invalid;

            case TokenType::Word: return ExpressionType::Word;
            case TokenType::Number: return ExpressionType::Number;
            case TokenType::FunctionCall: return ExpressionType::FunctionCall;
            case TokenType::StringContent: return ExpressionType::String;
            case TokenType::LogicalAnd: return ExpressionType::LogicalAnd;
            case TokenType::LogicalOr: return ExpressionType::LogicalOr;
            case TokenType::LogicalNegation: return ExpressionType::LogicalNegation;
            case TokenType::CompareGreater: return ExpressionType::CompareGreater;
            case TokenType::CompareGreaterOrEqual: return ExpressionType::CompareGreaterOrEqual;
            case TokenType::CompareLess: return ExpressionType::CompareLess;
            case TokenType::CompareLessOrEqual: return ExpressionType::CompareLessOrEqual;
            case TokenType::CompareEqual: return ExpressionType::CompareEqual;
            case TokenType::CompareNotEqual: return ExpressionType::CompareNotEqual;
            case TokenType::SequenceSeparator: return ExpressionType::SequenceSeparator;
            case TokenType::ScopeSeparator: return ExpressionType::Scope;
        }
        return ExpressionType::Invalid;
    }

    int Parser::precedenceForTokenType(TokenType tokenType) {
        switch (tokenType) {
            case TokenType::End:
            case TokenType::Error:
            case TokenType::StringBegin:
            case TokenType::StringEnd:
            case TokenType::ArgEnd:
            case TokenType::ParenthesisClose:
            case TokenType::ArgSeparator:
            case TokenType::Word:
            case TokenType::Number:
            case TokenType::StringContent:
                // Not an operator that should be pushed.
                return 0;

            case TokenType::Begin:
            case TokenType::ParenthesisOpen:
            case TokenType::ArgBegin:
                // Delimiters that should not be popped by comparing the precedences.
                return 0;

            case TokenType::SequenceSeparator:
                return 1;
            case TokenType::LogicalOr:
                // "a; b || c" is equivalent to "a; (b || c)"
                return 2;
            case TokenType::LogicalAnd:
                // "a || b && c" is equivalent to "a || (b && c)"
                return 3;
            case TokenType::CompareGreater:
            case TokenType::CompareGreaterOrEqual:
            case TokenType::CompareLess:
            case TokenType::CompareLessOrEqual:
            case TokenType::CompareEqual:
            case TokenType::CompareNotEqual:
                // "a && b == c != d" is equivalent to "a && ((b == c) != d)"
                return 4;
            case TokenType::LogicalNegation:
                // "!a == b" is equivalent to "(!a) == b"
                return 5;
            case TokenType::ScopeSeparator:
                return 6;
            case TokenType::FunctionCall:
                return 7;
        }
        return 0;
    }
}
