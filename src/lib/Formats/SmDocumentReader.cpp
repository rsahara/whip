//
//  SmDocumentReader.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "SmDocumentReader.hpp"

#include <string>
#include <vector>

#include "Util/Debug.hpp"
#include "Util/Logging.hpp"

#include "SmFormat/Token.hpp"
#include "SmFormat/Scanner.hpp"
#include "SmFormat/Parser.hpp"

namespace Whip {
    using namespace SmFormat;

    bool SmDocumentReader::read(Document& document, const char* inputText) {
        Scanner scanner(inputText);
        if (!scanner.scan()) {
            errorLog() << "scan error." << std::endl;
            return false;
        }

        const Token* firstToken = &*scanner.result().begin();
        Parser parser(document, firstToken);
        if (!parser.parse()) {
            errorLog() << "parse error." << std::endl;
            return false;
        }
        return true;
    }
}
