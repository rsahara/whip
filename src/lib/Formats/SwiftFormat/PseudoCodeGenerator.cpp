//
//  PseudoCodeGenerator.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "PseudoCodeGenerator.hpp"

#include "Document/Document.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCode/PseudoCodeAccessor.hpp"
#include "Util/Text.hpp"

namespace Whip::SwiftFormat {
    PseudoCodeGenerator::PseudoCodeGenerator(const Document& document) {
        for (auto it = document.metas.begin(); it != document.metas.end(); it++) {
            const MetaData& metaData = *it;
            if (metaData.key == MetaDataKey::treatAsEnum) {
                enumSet.emplace(metaData.value);
            }
        }
    }
    
    PseudoCodeGenerator::~PseudoCodeGenerator() {
    }

    void PseudoCodeGenerator::generate(const PseudoCode& pseudoCode, std::ostream& output) {
        PseudoCodeAccessor pseudoCodeAccessor(pseudoCode);
        this->pseudoCodeAccessor = &pseudoCodeAccessor;

        BasicPseudoCodeGenerator basicPseudoCodeGenerator(*this);
        basicPseudoCodeGenerator.generate(pseudoCodeAccessor, output);
    }

    void PseudoCodeGenerator::writeCommonWord(const Expression& expression, int expressionID, std::ostream& output) {
        // Check if it's the value of an enum.
        PseudoCodeAccessor& pseudoCodeAccessor = *this->pseudoCodeAccessor;
        int parentExpressionID = pseudoCodeAccessor.parentExpressionID(expressionID);
        if (parentExpressionID != invalidExpressionID) {
            const Expression& parentExpression = pseudoCodeAccessor.content().expression(parentExpressionID);
            if (parentExpression.type == ExpressionType::Scope) {
                int leftExpressionID = pseudoCodeAccessor.childExpressionIDs(parentExpressionID)[0];
                if (leftExpressionID != expressionID) {
                    const Expression& leftExpression = pseudoCodeAccessor.content().expression(leftExpressionID);
                    if (enumSet.find(leftExpression.value) != enumSet.end()) {
                        output << lowerCamelCaseIdentifier(expression.value);
                        return;
                    }
                }
            }
        }

        output << expression.value;
    }

    void PseudoCodeGenerator::writeDelegateWord(const Expression& expression, int expressionID, std::ostream& output) {
        output << "implementations";
    }

    void PseudoCodeGenerator::writeFunctionCall(const Expression& expression, int expressionID, std::ostream& output) {
        PseudoCodeAccessor& pseudoCodeAccessor = *this->pseudoCodeAccessor;
        // Add the delegate word if it is a simple function call.
        int parentExpressionID = pseudoCodeAccessor.parentExpressionID(expressionID);
        if (parentExpressionID == invalidExpressionID
            || pseudoCodeAccessor.content().expression(parentExpressionID).type != ExpressionType::Scope) {
            output << "implementations.";
        }
        output << expression.value << "(";
    }
}
