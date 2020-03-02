//
//  KotlinDocumentWriter.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_KotlinDocumentWriter_hpp
#define Whip_KotlinDocumentWriter_hpp

#include "Document/DocumentWriter.hpp"

namespace Whip {
    /// Generates a state machine in the kotlin language.
    struct KotlinDocumentWriter: DocumentWriter {
        virtual bool writeToDirectory(const std::string& directoryPath,
                                      const std::string& basename,
                                      const Document& document,
                                      const std::vector<std::string>& parameters);
    };
}

#endif
