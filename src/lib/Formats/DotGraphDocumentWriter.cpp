//
//  DotGraphDocumentWriter.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "DotGraphDocumentWriter.hpp"

#include <fstream>

#include "Util/Logging.hpp"
#include "Util/Filename.hpp"

#include "DotGraphFormat/GraphGenerator.hpp"

namespace Whip {
    using namespace DotGraphFormat;

    const std::string dotFileExt("dot");

    bool DotGraphDocumentWriter::writeToDirectory(const std::string& directoryPath,
                                                  const std::string& basename,
                                                  const Document& document,
                                                  const std::vector<std::string>& parameters) {
        std::string filename = filenameFromBasenameAndExtension(basename, dotFileExt);
        std::string path = pathFromDirectoryAndFilename(directoryPath, filename);
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file.good()) {
            errorLog() << "could not write file: \"" << filename << "\"." <<std::endl;
            return false;
        }
        GraphGenerator graphGenerator(document, file);
        bool success = graphGenerator.generate();
        file.close();
        if (success && !file.good()) {
            errorLog() << "could not write file: \"" << filename << "\"." << std::endl;
            return false;
        }

        return success;
    }
}
