//
//  DocumentWriter.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Document_DocumentWriter_hpp
#define Whip_Document_DocumentWriter_hpp

#include <string>
#include <vector>

namespace Whip {
    struct Document;

    /// Protocol for writing the informations of a state machine to files.
    struct DocumentWriter {
        virtual ~DocumentWriter();
        
        /// Writes the files in dirPath with filenames generated using basename.
        virtual bool writeToDirectory(const std::string& directoryPath,
                                      const std::string& basename,
                                      const Document& document,
                                      const std::vector<std::string>& parameters) = 0;
    };
}

#endif
