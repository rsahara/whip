//
//  SmDocumentReader.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_SmDocumentReader_hpp
#define Whip_SmDocumentReader_hpp

#include "Document/DocumentReader.hpp"

namespace Whip {
    /// Reads a state machine specification file.
    struct SmDocumentReader: DocumentReader {
        virtual bool read(Document& document, const char* inputText);
    };
}

#endif
