//
//  SwiftEnumDocumentWriter.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "SwiftEnumDocumentWriter.hpp"

#include <fstream>

#include "Util/Logging.hpp"
#include "Util/Filename.hpp"

#include "SwiftFormat/EnumGenerator.hpp"

namespace Whip {
    using namespace SwiftFormat;
    const std::string swiftEnumFileExt("swift");

    bool SwiftEnumDocumentWriter::writeToDirectory(const std::string& directoryPath,
                                                   const std::string& basename,
                                                   const Document& document,
                                                   const std::vector<std::string>& parameters) {

        if (parameters.empty()) {
            errorLog() << "enum name not specified." << std::endl;
            return false;
        }
        std::string enumName = parameters[0];
        std::string filename = filenameFromBasenameAndExtension(enumName, swiftEnumFileExt);
        std::string path = pathFromDirectoryAndFilename(directoryPath, filename);
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file.good()) {
            errorLog() << "could not write file: \"" << filename << "\"." << std::endl;
            return false;
        }
        EnumGenerator enumGenerator(document, file, enumName);
        bool succeeded = enumGenerator.generate();
        file.close();
        if (succeeded && !file.good()) {
            errorLog() << "could not write file: " << filename << "\"." << std::endl;
            return false;
        }

        return succeeded;
    }
}
