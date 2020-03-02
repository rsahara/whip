//
//  PseudoCode.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_PseudoCode_hpp
#define Whip_PseudoCode_PseudoCode_hpp

#include <vector>
#include <string>

#include "Util/Debug.hpp"

namespace Whip {
    /// Expression ID used to represent an invalid expression ID.
    extern const int invalidExpressionID;

    enum class ExpressionType {
        Invalid,

        // Primitives
        Word,
        String,
        Number,

        // Operations
        LogicalOr,
        LogicalAnd,
        LogicalNegation,
        CompareGreater,
        CompareGreaterOrEqual,
        CompareLess,
        CompareLessOrEqual,
        CompareEqual,
        CompareNotEqual,
        SequenceSeparator,
        FunctionCall,
        Scope,
    };

    struct Expression {
        inline Expression() {}
        inline Expression(ExpressionType type, const std::string& value, int childCount = 0);
        inline Expression(const Expression& expression) { *this = expression; }
        inline Expression& operator=(const Expression& expression);

        /// Type of expression.
        /// If it is a primitive, value is set and childCount is 0.
        /// Otherwise, childCount > 0.
        ExpressionType type;

        /// Value of this expression, if it's a primitive.
        std::string value;

        /// Number of children/arguments this expression takes, 0 for primitives.
        int childCount;
    };

    /// Expression tree.
    struct PseudoCode {
        /// Reads pseudo code text and saves the content to a PseudoCode object.
        /// Returns false if there is an error.
        bool read(const char* inputText, int firstLine = 1);

        /// Whether the expression tree is empty.
        inline bool isEmpty() const { return expressions.empty(); }
        /// ID of the root expression. invalidExpressionID if the expression tree is empty.
        inline int rootExpressionID() const { return isEmpty() ? invalidExpressionID : expressionCount() - 1; }
        /// Returns the reference to an expression by its ID.
        inline Expression& expression(int expressionID);
        /// Returns the reference to an expression by its ID.
        inline const Expression& expression(int expressionID) const;
        /// Number of expressions in the expression tree.
        inline int expressionCount() const { return (int)expressions.size(); }

    private:
        typedef std::vector<Expression> ExpressionVector;
        /// Vector of expressions.
        /// (Expression trees are represented in the Reverse Polish Notation so we can use vectors as container.)
        ExpressionVector expressions;
    };

    /// Placeholder that represents the variable name of the state machine delegate.
    extern const std::string placeholderForDelegateWord;
}

// Inline

namespace Whip {
    inline Expression::Expression(ExpressionType type, const std::string& value, int childCount)
    : type(type)
    , value(value)
    , childCount(childCount) {
    }

    inline Expression& Expression::operator=(const Expression& expression) {
        type = expression.type;
        value = expression.value;
        childCount = expression.childCount;
        return *this;
    }

    inline Expression& PseudoCode::expression(int expressionID) {
        ASSERT(expressionID >= 0 && expressionID < expressionCount(), "invalid ID");
        return expressions[expressionID];
    }
    inline const Expression& PseudoCode::expression(int expressionID) const {
        ASSERT(expressionID >= 0 && expressionID < expressionCount(), "invalid ID");
        return expressions[expressionID];
    }
}

#endif
