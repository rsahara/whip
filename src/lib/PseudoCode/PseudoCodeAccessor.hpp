//
//  PseudoCodeAccessor.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_PseudoCode_PseudoCodeAccessor_hpp
#define Whip_PseudoCode_PseudoCodeAccessor_hpp

#include <vector>

namespace Whip {
    struct PseudoCode;
    struct Expression;

    /// Helper class that provides the following features:
    ///  - Access children information.
    ///  - Perform a tree traversal on the expression tree and iterate expressions.
    struct PseudoCodeAccessor {
        PseudoCodeAccessor(PseudoCode& pseudoCode);
        PseudoCodeAccessor(const PseudoCode& pseudoCode);

        /// Returns an array of child expression IDs.
        /// The size of the array is equal to expression.childCount.
        const int* childExpressionIDs(int expressionID) const;

        /// Retuns the parent expression ID.
        int parentExpressionID(int expressionID) const;

        /// Returns the pseudo code.
        PseudoCode& content() { return pseudoCode; }
        /// Returns the pseudo code.
        const PseudoCode& content() const { return pseudoCode; }

    private:
        /// Creates the indices of child expressions.
        void createChildIndices(int& nextExpressionIndex);

    private:
        /// Indices of child expressions of each operations.
        std::vector<int> allChildIndices;
        /// Maps the index of an expression to the index of the first child expression in "allChildIndices". -1 means no child.
        std::vector<int> offsets;
        /// Maps the index of an expression to the index of the parent expression. -1 is used for the root expression.
        std::vector<int> parentIndices;

        /// Pseudo code to analyze.
        PseudoCode& pseudoCode;
    };
}

#endif
