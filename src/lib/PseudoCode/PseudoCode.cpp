//
//  PseudoCode.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "PseudoCode.hpp"

#include "Util/Logging.hpp"

#include "PseudoCode.hpp"
#include "Language/Scanner.hpp"
#include "Language/Parser.hpp"

namespace Whip {
    using namespace PseudoCodeLanguage;
    const std::string placeholderForDelegateWord("__DELEGATE__");
    const int invalidExpressionID = -1;

    bool PseudoCode::read(const char* inputText, int firstLine) {
        Scanner scanner(inputText);
        if (!scanner.scan(firstLine)) {
            return false;
        }

        const Token* firstToken = &*scanner.result().begin();
        Parser parser(expressions, firstToken);
        if (!parser.parse()) {
            return false;
        }

        return true;
    }
}
