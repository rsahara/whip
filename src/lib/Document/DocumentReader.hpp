//
//  DocumentReader.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Document_DocumentReader_hpp
#define Whip_Document_DocumentReader_hpp

#include <string>

namespace Whip {
    struct Document;

    /// Protocol for reading the definition of a state machine and loading the information to a Document object.
    struct DocumentReader {
        virtual ~DocumentReader();
        
        /// Reads a string and loads the values to document.
        virtual bool read(Document& document, const char* inputText) = 0;
        
        /// Reads a file and loads the values to document.
        bool readFromFile(Document& document, const std::string& filename);
    };
}

#endif
