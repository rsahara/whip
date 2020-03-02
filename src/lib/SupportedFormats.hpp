//
//  SupportedFormats.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_SupportedFormats_hpp
#define Whip_SupportedFormats_hpp

#include <string>

namespace Whip {
    struct DocumentWriter;
    struct DocumentReader;

    /// Retrieves a document writer that supports writing in a given format.
    /// Supported formats:
    ///   - swift: swift files that implement the state machine document.
    ///   - dotgraph: graph representation of the state machine document for graphviz (dot file).
    DocumentWriter* documentWriterByFormatName(const std::string& formatName);

    /// Retrieves a document reader that supports a given file extension.
    /// Supported file extension:
    ///   - sm: state machine specification inpired from SMC (State Machine Compiler).
    DocumentReader* documentReaderByFileExtension(const std::string& fileExtension);
}

#endif
