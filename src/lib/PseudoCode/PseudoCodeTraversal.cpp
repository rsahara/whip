//
//  PseudoCodeTraversal.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "PseudoCodeTraversal.hpp"

#include <vector>

#include "Util/Debug.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCode/PseudoCodeAccessor.hpp"

#define ENABLE_TRAVERSELOG  0

#if ENABLE_TRAVERSELOG
#define TRAVERSELOG(format, ...) LOG("traverse: " format, ##__VA_ARGS__)
#else
#define TRAVERSELOG(format, ...) (void)0
#endif

namespace Whip {
    PseudoCodeTraversal::PseudoCodeTraversal(PseudoCodeAccessor& pseudoCodeAccessor, Delegate& delegate)
    : pseudoCodeAccessor(pseudoCodeAccessor)
    , delegate(delegate) {
    }

    PseudoCodeTraversal::PseudoCodeTraversal(const PseudoCodeAccessor& pseudoCodeAccessor, Delegate& delegate)
    : pseudoCodeAccessor(const_cast<PseudoCodeAccessor&>(pseudoCodeAccessor))
    , delegate(delegate) {
    }

    void PseudoCodeTraversal::traverse() {
        if (pseudoCodeAccessor.content().isEmpty()) {
            return;
        }

        // Traverse the expressions in order of their appearance.
        traverseExpressions(pseudoCodeAccessor.content().rootExpressionID());
    }

    void PseudoCodeTraversal::traverseExpressions(int expressionID) const {
        ASSERT(expressionID >= 0, "unexpected index");
        Expression& expression = pseudoCodeAccessor.content().expression(expressionID);
        TRAVERSELOG("PRE: %d", expressionID);
        delegate.traversePreOrder(expression, expressionID);

        if (expression.childCount > 0) {
            const int* childExpressionIDs = pseudoCodeAccessor.childExpressionIDs(expressionID);
            traverseExpressions(childExpressionIDs[0]);

            // From the second child, call at the in-order pass.
            for (int childIndex = 1; childIndex < expression.childCount; childIndex++) {
                TRAVERSELOG("IN: %d", expressionID);
                delegate.traverseInOrder(expression, expressionID, childIndex);
                traverseExpressions(childExpressionIDs[childIndex]);
            }
        }

        TRAVERSELOG("POST: %d", expressionID);
        delegate.traversePostOrder(expression, expressionID);
    }
}

namespace Whip {
    PseudoCodeTraversal::Delegate::~Delegate() {
    }
}
