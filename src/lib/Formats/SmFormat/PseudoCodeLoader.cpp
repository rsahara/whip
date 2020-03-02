//
//  PseudoCodeLoader.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "PseudoCodeLoader.hpp"

#include "Document/Document.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCode/PseudoCodeAccessor.hpp"

namespace Whip::SmFormat {
    const std::string placeholderForContextVariable = "ctxt";

    PseudoCodeLoader::PseudoCodeLoader(PseudoCode& pseudoCode, const char* inputText)
    : pseudoCode(pseudoCode)
    , inputText(inputText)
    , accessor(nullptr) {
    }

    PseudoCodeLoader::~PseudoCodeLoader() {
    }

    bool PseudoCodeLoader::load(int firstLine) {
        if (!pseudoCode.read(inputText, firstLine)) {
            return false;
        }

        PseudoCodeAccessor pseudoCodeAccessor(pseudoCode);
        accessor = &pseudoCodeAccessor;

        PseudoCodeTraversal pseudoCodeTraversal(pseudoCodeAccessor, *this);
        pseudoCodeTraversal.traverse();

        return true;
    }

    void PseudoCodeLoader::traversePreOrder(Expression& expression, int expressionID) {
        if (expression.type == ExpressionType::Word
            && expression.value == placeholderForContextVariable) {
            expression.value = placeholderForDelegateWord;
        }
    }

    void PseudoCodeLoader::traverseInOrder(Expression& expression, int expressionID, int childIndex) {
        // Nothing to do.
    }

    void PseudoCodeLoader::traversePostOrder(Expression& expression, int expressionID) {
        // Nothing to do.
    }
}

