//
//  SwiftDocumentWriter.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "SwiftDocumentWriter.hpp"

#include <fstream>

#include "Util/Logging.hpp"
#include "Util/Filename.hpp"

#include "SwiftFormat/StateMachineGenerator.hpp"

namespace Whip {
    using namespace SwiftFormat;

    const std::string swiftFileExt("swift");

    bool SwiftDocumentWriter::writeToDirectory(const std::string& directoryPath,
                                               const std::string& basename,
                                               const Document& document,
                                               const std::vector<std::string>& parameters) {
        std::string filename = filenameFromBasenameAndExtension(basename, swiftFileExt);
        std::string path = pathFromDirectoryAndFilename(directoryPath, filename);
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file.good()) {
            errorLog() << "could not write file: \"" << filename << "\"." << std::endl;
            return false;
        }
        StateMachineGenerator stateMachineGenerator(document, file);
        bool succeeded = stateMachineGenerator.generate();
        file.close();
        if (succeeded && !file.good()) {
            errorLog() << "could not write file: " << filename << "\"." << std::endl;
            return false;
        }

        return succeeded;
    }
}
