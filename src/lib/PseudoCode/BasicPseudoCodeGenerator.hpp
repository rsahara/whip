//
//  BasicPseudoCodeGenerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_BasicPseudoCodeGenerator_hpp
#define Whip_PseudoCode_BasicPseudoCodeGenerator_hpp

#include <stack>
#include <ostream>
#include <string>

#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCode/PseudoCodeTraversal.hpp"

namespace Whip {
    /// A basic pseudo code generator that generates a human readable representation of pseudo codes.
    struct BasicPseudoCodeGenerator: PseudoCodeTraversal::Delegate {
        struct Writer;
        BasicPseudoCodeGenerator();
        BasicPseudoCodeGenerator(Writer& writer);
        virtual ~BasicPseudoCodeGenerator();

        void generate(const PseudoCode& pseudoCode, std::ostream& output);
        void generate(const PseudoCodeAccessor& pseudoCodeAccessor, std::ostream& output);

        /// Interface defining write functions called during the generation of a pseudo code.
        /// The base implementation is a basic human readable output.
        struct Writer {
            Writer();
            virtual ~Writer();

            virtual void writePreOrder(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeInOrder(const Expression& expression, int expressionID, int childIndex, std::ostream& output);
            virtual void writePostOrder(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeOpenParentheses(std::ostream& output);
            virtual void writeCloseParentheses(std::ostream& output);
            virtual void writeDelegateWord(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeCommonWord(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeNumber(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeString(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeSequenceSeparator(const Expression& expression, int expressionID, std::ostream& output);
            virtual void writeFunctionCall(const Expression& expression, int expressionID, std::ostream& output);
        };

    public:
        // PseudoCodeTraversal::Delegate
        virtual void traversePreOrder(Expression& expression, int expressionID);
        virtual void traverseInOrder(Expression& expression, int expressionID, int childIndex);
        virtual void traversePostOrder(Expression& expression, int expressionID);

    private:
        /// Priority of an expression type, to calculate required parentheses.
        int priorityForExpressionType(ExpressionType expressionType);

    private:
        /// Writes the actual code to the output.
        Writer& writer;
        /// Stack of parent expressions during the tree traversal, to add required parenthesis.
        std::stack<int> priorityStack;
        /// Output stream to store the result. Valid only inside the generate function.
        std::ostream* output;
    };
}
#endif
