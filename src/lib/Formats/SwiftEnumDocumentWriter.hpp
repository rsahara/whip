//
//  SwiftEnumDocumentWriter.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_SwiftEnumDocumentWriter_hpp
#define Whip_SwiftEnumDocumentWriter_hpp

#include "Document/DocumentWriter.hpp"

namespace Whip {
    /// Generates a enum definition in the swift language.
    struct SwiftEnumDocumentWriter: DocumentWriter {
        virtual bool writeToDirectory(const std::string& directoryPath,
                                      const std::string& basename,
                                      const Document& document,
                                      const std::vector<std::string>& parameters);
    };
}

#endif
