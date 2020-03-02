//
//  SwiftDocumentWriter.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_SwiftDocumentWriter_hpp
#define Whip_SwiftDocumentWriter_hpp

#include "Document/DocumentWriter.hpp"

namespace Whip {
    /// Generates a state machine in the swift language.
    struct SwiftDocumentWriter: DocumentWriter {
        virtual bool writeToDirectory(const std::string& directoryPath,
                                      const std::string& basename,
                                      const Document& document,
                                      const std::vector<std::string>& parameters);
    };
}

#endif
