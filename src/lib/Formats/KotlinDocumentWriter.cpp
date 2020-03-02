//
//  KotlinDocumentWriter.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "KotlinDocumentWriter.hpp"

#include <fstream>

#include "Util/Logging.hpp"
#include "Util/Filename.hpp"

#include "KotlinFormat/StateMachineGenerator.hpp"

namespace Whip {
    using namespace KotlinFormat;
    const std::string kotlinFileExt("kt");

    bool KotlinDocumentWriter::writeToDirectory(const std::string& directoryPath,
                                                const std::string& basename,
                                                const Document& document,
                                                const std::vector<std::string>& parameters) {
        std::string filename = filenameFromBasenameAndExtension(basename, kotlinFileExt);
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
