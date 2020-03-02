//
//  Document.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Document.hpp"

namespace Whip {
    const int invalidStateID = -1;
    const int invalidPseudoCodeID = -1;

    namespace MetaDataKey {
        const std::string defineDelegateClass("defineDelegateClass");
        const std::string moduleToImport("moduleToImport");
        const std::string definePackage("definePackage");
        const std::string defineInitialState("defineInitialState");
        const std::string treatAsEnum("treatAsEnum");
    }

    Document::Document() {
        metas.reserve(32);
        states.reserve(256);
        transitions.reserve(256);
        pseudoCodes.reserve(256);
    }
}
