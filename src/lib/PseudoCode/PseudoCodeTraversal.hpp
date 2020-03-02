//
//  PseudoCodeTraversal.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_PseudoCodeTraversal_hpp
#define Whip_PseudoCode_PseudoCodeTraversal_hpp

#include <vector>

namespace Whip {
    struct Expression;
    struct PseudoCodeAccessor;

    /// Helper class that performs a tree traversal on the expression tree of pseudo codes.
    struct PseudoCodeTraversal {
        struct Delegate;
        PseudoCodeTraversal(PseudoCodeAccessor& pseudoCodeAccessor, Delegate& delegate);
        PseudoCodeTraversal(const PseudoCodeAccessor& pseudoCodeAccessor, Delegate& delegate);

        /// Performs the tree traversal on the provided expressions.
        void traverse();

        /// Interface defining functions called during a tree traversal of the expression tree of a pseudo code.
        struct Delegate {
            virtual ~Delegate();

            /// Function called for each expression, for the pre order pass (i.e. when entering the expression).
            /// If there are more than 2 children, childIndex is the index of the previous child.
            virtual void traversePreOrder(Expression& expression, int expressionID) = 0;

            /// Function called for each expression with more than 2 children,
            /// for the pre order pass (i.e. after a visit to child expressions).
            /// childIndex is the index of the previous child.
            virtual void traverseInOrder(Expression& expression, int expressionID, int childIndex) = 0;

            /// Function called for each expression, for the pre order pass (i.e. when leaving the expression).
            /// In expressions with more than 2 children, childIndex is the index of the previous child.
            virtual void traversePostOrder(Expression& expression, int expressionID) = 0;
        };

    private:
        /// Traverse the expressions in order of their appearance.
        void traverseExpressions(int expressionID) const;

    private:
        /// Helper that provides information on child expressions.
        PseudoCodeAccessor& pseudoCodeAccessor;
        /// Delegate that will receive the function calls at each expression visited.
        Delegate& delegate;
    };
}
#endif
