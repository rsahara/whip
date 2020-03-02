//
//  PseudoCodeAccessor.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "PseudoCodeAccessor.hpp"

#include "Util/Debug.hpp"
#include "PseudoCode/PseudoCode.hpp"

namespace Whip {
    PseudoCodeAccessor::PseudoCodeAccessor(PseudoCode& pseudoCode)
    : allChildIndices()
    , offsets(pseudoCode.expressionCount(), -1)
    , parentIndices(pseudoCode.expressionCount(), -1)
    , pseudoCode(pseudoCode) {
        allChildIndices.reserve(pseudoCode.expressionCount());
        int nextExpressionID = pseudoCode.rootExpressionID();
        if (nextExpressionID > 0) {
            createChildIndices(nextExpressionID);
        }
    }

    PseudoCodeAccessor::PseudoCodeAccessor(const PseudoCode& pseudoCode)
    : allChildIndices()
    , offsets(pseudoCode.expressionCount(), -1)
    , parentIndices(pseudoCode.expressionCount(), -1)
    , pseudoCode(const_cast<PseudoCode&>(pseudoCode)) {
        allChildIndices.reserve(pseudoCode.expressionCount());
        int nextExpressionID = pseudoCode.rootExpressionID();
        if (nextExpressionID > 0) {
            createChildIndices(nextExpressionID);
        }
    }

    const int* PseudoCodeAccessor::childExpressionIDs(int expressionID) const {
        ASSERT(expressionID < offsets.size(), "invalid ID");
        int firstChildOffset = offsets[expressionID];
        if (firstChildOffset == -1)
            return nullptr;
        return &*allChildIndices.begin() + offsets[expressionID];
    }

    int PseudoCodeAccessor::parentExpressionID(int expressionID) const {
        ASSERT(expressionID < parentIndices.size(), "invalid ID");
        return parentIndices[expressionID];
    }

    void PseudoCodeAccessor::createChildIndices(int& nextExpressionID) {
        int expressionID = nextExpressionID;
        const Expression& expression = pseudoCode.expression(expressionID);
        nextExpressionID--;

        // Check the child expressions.
        if (expression.childCount > 0) {
            std::vector<int> tempChilds;
            tempChilds.reserve(expression.childCount);
            for (int childIndex = 0; childIndex < expression.childCount; childIndex++) {
                tempChilds.emplace_back(nextExpressionID);
                createChildIndices(nextExpressionID);
            }
            ASSERT(tempChilds.size() == expression.childCount, "number of child expressions not matching");
            offsets[expressionID] = (int)allChildIndices.size();
            for (auto it = tempChilds.rbegin(); it != tempChilds.rend(); it++) {
                int childExpressionID = *it;
                allChildIndices.emplace_back(childExpressionID);
                parentIndices[childExpressionID] = expressionID;
            }
        }
    }
}
