//
//  EnumGenerator.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "EnumGenerator.hpp"

#include <vector>

#include "PseudoCode/PseudoCode.hpp"
#include "Document/Document.hpp"
#include "PseudoCode/PseudoCodeAccessor.hpp"
#include "Util/Text.hpp"

namespace Whip::SwiftFormat {
    EnumGenerator::EnumGenerator(const Document& document, std::ostream& output, const std::string& enumName)
    : document(document)
    , output(output)
    , enumName(enumName) {
    }

    bool EnumGenerator::generate() {
        enumValuesSet.clear();
        enumValues.clear();

        for (std::vector<PseudoCode>::const_iterator it = document.pseudoCodes.begin();
             it != document.pseudoCodes.end();
             it++) {
            const PseudoCode& pseudoCode = *it;
            PseudoCodeAccessor pseudoCodeAccessor(pseudoCode);
            this->pseudoCodeAccessor = &pseudoCodeAccessor;
            PseudoCodeTraversal pseudoCodeTraversal(pseudoCodeAccessor, *this);
            pseudoCodeTraversal.traverse();
        }

        output << "enum " << enumName << " {" << std::endl;
        for (std::vector<std::string>::iterator it = enumValues.begin();
             it != enumValues.end();
             it++) {
            output << indentation(1) << "case " << *it << std::endl;
        }
        output << "}" << std::endl;

        return true;
    }

    void EnumGenerator::traversePreOrder(Expression& expression, int expressionID) {
        PseudoCodeAccessor& pseudoCodeAccessor = *this->pseudoCodeAccessor;
        // Check if it's the value of the enum we're looking for.
        if (expression.type != ExpressionType::Word) {
            return;
        }
        int parentExpressionID = pseudoCodeAccessor.parentExpressionID(expressionID);
        if (parentExpressionID == invalidExpressionID) {
            return;
        }
        const Expression& parentExpression = pseudoCodeAccessor.content().expression(parentExpressionID);
        if (parentExpression.type != ExpressionType::Scope) {
            return;
        }
        int leftExpressionID = pseudoCodeAccessor.childExpressionIDs(parentExpressionID)[0];
        if (leftExpressionID == expressionID) {
            return;
        }
        const Expression& leftExpression = pseudoCodeAccessor.content().expression(leftExpressionID);
        if (enumName != leftExpression.value) {
            return;
        }
        std::string enumValue = lowerCamelCaseIdentifier(expression.value);
        if (enumValuesSet.find(enumValue) != enumValuesSet.end()) {
            // Already exists.
            return;
        }
        enumValuesSet.emplace(enumValue);
        enumValues.emplace_back(enumValue);
    }

    void EnumGenerator::traverseInOrder(Expression& expression, int expressionID, int childIndex) {
    }

    void EnumGenerator::traversePostOrder(Expression& expression, int expressionID) {
    }
}
