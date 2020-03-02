//
//  BasicPseudoCodeGenerator.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "BasicPseudoCodeGenerator.hpp"
#include "PseudoCode/PseudoCodeAccessor.hpp"
#include "Util/Text.hpp"

namespace Whip {
    static BasicPseudoCodeGenerator::Writer basicWriter;

    BasicPseudoCodeGenerator::BasicPseudoCodeGenerator(Writer& writer)
    : writer(writer) {
    }

    BasicPseudoCodeGenerator::BasicPseudoCodeGenerator()
    : writer(basicWriter) {
    }

    BasicPseudoCodeGenerator::~BasicPseudoCodeGenerator() {
    }

    void BasicPseudoCodeGenerator::generate(const PseudoCode& pseudoCode, std::ostream& output) {
        this->output = &output;
        priorityStack = std::stack<int>();
        priorityStack.push(0);

        PseudoCodeAccessor pseudoCodeAccessor(pseudoCode);
        PseudoCodeTraversal pseudoCodeTraversal(pseudoCodeAccessor, *this);
        pseudoCodeTraversal.traverse();
    }

    void BasicPseudoCodeGenerator::generate(const PseudoCodeAccessor& pseudoCodeAccessor, std::ostream& output) {
        this->output = &output;
        priorityStack = std::stack<int>();
        priorityStack.push(0);

        PseudoCodeTraversal pseudoCodeTraversal(pseudoCodeAccessor, *this);
        pseudoCodeTraversal.traverse();
    }

    void BasicPseudoCodeGenerator::traversePreOrder(Expression& expression, int expressionID) {
        // Consider to open parentheses by checking the parent expression's priority.
        int expressionPriority = priorityForExpressionType(expression.type);
        int parentExpressionPriority = priorityStack.top();

        // Function calls are special because they are protected by delimiters (parenthesis and comma).
        // If parent expression has a higher priority, add a parenthesis to keep the semantics.
        if (expression.type != ExpressionType::FunctionCall && parentExpressionPriority > expressionPriority) {
            writer.writeOpenParentheses(*output);
        }
        priorityStack.push(expressionPriority);

        writer.writePreOrder(expression, expressionID, *output);
    }

    void BasicPseudoCodeGenerator::traverseInOrder(Expression& expression, int expressionID, int childIndex) {
        writer.writeInOrder(expression, expressionID, childIndex, *output);
    }

    void BasicPseudoCodeGenerator::traversePostOrder(Expression& expression, int expressionID) {
        writer.writePostOrder(expression, expressionID, *output);

        // Consider to close parentheses by checking the parent expression's priority.
        priorityStack.pop();
        int expressionPriority = priorityForExpressionType(expression.type);
        int parentExpressionPriority = priorityStack.top();
        if (expression.type != ExpressionType::FunctionCall && parentExpressionPriority > expressionPriority) {
            writer.writeCloseParentheses(*output);
        }
    }

    int BasicPseudoCodeGenerator::priorityForExpressionType(ExpressionType expressionType) {
        switch (expressionType) {
            case ExpressionType::Invalid:
            case ExpressionType::SequenceSeparator:
                return 1;
            case ExpressionType::LogicalOr:
                return 2;
            case ExpressionType::LogicalAnd:
                return 3;
            case ExpressionType::CompareGreater:
            case ExpressionType::CompareGreaterOrEqual:
            case ExpressionType::CompareLess:
            case ExpressionType::CompareLessOrEqual:
            case ExpressionType::CompareEqual:
            case ExpressionType::CompareNotEqual:
                return 4;
            case ExpressionType::LogicalNegation:
                return 5;
            case ExpressionType::FunctionCall:
                return 6;
            case ExpressionType::Scope:
                return 7;
            case ExpressionType::Word:
            case ExpressionType::Number:
            case ExpressionType::String:
                return 8;
        }
        return 1;
    }
}

namespace Whip {
    BasicPseudoCodeGenerator::Writer::Writer() {
    }

    BasicPseudoCodeGenerator::Writer::~Writer() {
    }

    void BasicPseudoCodeGenerator::Writer::writePreOrder(const Expression& expression, int expressionID, std::ostream& output) {
        switch (expression.type) {
            case ExpressionType::Invalid:
            case ExpressionType::LogicalOr:
            case ExpressionType::LogicalAnd:
            case ExpressionType::CompareGreater:
            case ExpressionType::CompareGreaterOrEqual:
            case ExpressionType::CompareLess:
            case ExpressionType::CompareLessOrEqual:
            case ExpressionType::CompareEqual:
            case ExpressionType::CompareNotEqual:
            case ExpressionType::Scope:
            case ExpressionType::SequenceSeparator:
                // Nothing to write for pre-order pass.
                break;
            case ExpressionType::Word:
                if (expression.value == placeholderForDelegateWord) {
                    writeDelegateWord(expression, expressionID, output);
                }
                else {
                    writeCommonWord(expression, expressionID, output);
                }
                break;
            case ExpressionType::Number:
                writeNumber(expression, expressionID, output);
                break;
            case ExpressionType::String:
                writeString(expression, expressionID, output);
                break;
            case ExpressionType::FunctionCall:
                writeFunctionCall(expression, expressionID, output);
                break;
            case ExpressionType::LogicalNegation:
                output << "!";
                break;
        }
    }

    void BasicPseudoCodeGenerator::Writer::writeInOrder(const Expression& expression, int expressionID, int childIndex, std::ostream& output) {
        switch (expression.type) {
            case ExpressionType::Invalid:
            case ExpressionType::Word:
            case ExpressionType::String:
            case ExpressionType::Number:
            case ExpressionType::LogicalNegation:
                // Nothing to write for in-order pass.
                break;
            case ExpressionType::LogicalOr:
                output << " || ";
                break;
            case ExpressionType::LogicalAnd:
                output << " && ";
                break;
            case ExpressionType::CompareGreater:
                output << " > ";
                break;
            case ExpressionType::CompareGreaterOrEqual:
                output << " >= ";
                break;
            case ExpressionType::CompareLess:
                output << " < ";
                break;
            case ExpressionType::CompareLessOrEqual:
                output << " <= ";
                break;
            case ExpressionType::CompareEqual:
                output << " == ";
                break;
            case ExpressionType::CompareNotEqual:
                output << " != ";
                break;
            case ExpressionType::SequenceSeparator:
                writeSequenceSeparator(expression, expressionID, output);
                break;
            case ExpressionType::FunctionCall:
                output << ", ";
                break;
            case ExpressionType::Scope:
                output << ".";
                break;
        }
    }

    void BasicPseudoCodeGenerator::Writer::writePostOrder(const Expression& expression, int expressionID, std::ostream& output) {
        switch (expression.type) {
            case ExpressionType::Invalid:
            case ExpressionType::Word:
            case ExpressionType::String:
            case ExpressionType::Number:
            case ExpressionType::LogicalNegation:
            case ExpressionType::LogicalOr:
            case ExpressionType::LogicalAnd:
            case ExpressionType::CompareGreater:
            case ExpressionType::CompareGreaterOrEqual:
            case ExpressionType::CompareLess:
            case ExpressionType::CompareLessOrEqual:
            case ExpressionType::CompareEqual:
            case ExpressionType::CompareNotEqual:
            case ExpressionType::SequenceSeparator:
            case ExpressionType::Scope:
                // Nothing to write for post-order pass.
                break;
            case ExpressionType::FunctionCall:
                output << ")";
                break;
        }
    }

    void BasicPseudoCodeGenerator::Writer::writeOpenParentheses(std::ostream& output) {
        output << "(";
    }

    void BasicPseudoCodeGenerator::Writer::writeCloseParentheses(std::ostream& output) {
        output << ")";
    }

    void BasicPseudoCodeGenerator::Writer::writeCommonWord(const Expression& expression, int expressionID, std::ostream& output) {
        output << expression.value;
    }

    void BasicPseudoCodeGenerator::Writer::writeDelegateWord(const Expression& expression, int expressionID, std::ostream& output) {
        output << "dlg";
    }

    void BasicPseudoCodeGenerator::Writer::writeNumber(const Expression& expression, int expressionID, std::ostream& output) {
        output << expression.value;
    }

    void BasicPseudoCodeGenerator::Writer::writeString(const Expression& expression, int expressionID, std::ostream& output) {
        output << '\"' << expression.value << '\"';
    }

    void BasicPseudoCodeGenerator::Writer::writeSequenceSeparator(const Expression& expression, int expressionID, std::ostream& output) {
        output << "; ";
    }

    void BasicPseudoCodeGenerator::Writer::writeFunctionCall(const Expression& expression, int expressionID, std::ostream& output) {
        output << expression.value << "(";
    }
}
