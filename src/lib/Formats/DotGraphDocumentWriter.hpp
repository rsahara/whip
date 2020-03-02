//
//  DotGraphDocumentWriter.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_DotGraphDocumentWriter_hpp
#define Whip_DotGraphDocumentWriter_hpp

#include "Document/DocumentWriter.hpp"

namespace Whip {
    /// Writes the graphviz (dot file) graph representation of a state machine.
    struct DotGraphDocumentWriter: DocumentWriter {
        virtual bool writeToDirectory(const std::string& directoryPath,
                                      const std::string& basename,
                                      const Document& document,
                                      const std::vector<std::string>& parameters);
    };
}

#endif
