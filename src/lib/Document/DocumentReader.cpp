//
//  DocumentReader.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "DocumentReader.hpp"

#include <fstream>

#include "Util/Logging.hpp"

namespace Whip {
    DocumentReader::~DocumentReader() {
    }

    bool DocumentReader::readFromFile(Document& document, const std::string& filename) {
        std::ifstream file(filename, std::ios::binary | std::ios::ate);
        if (!file.good()) {
            errorLog() << "could not open file: \"" << filename << "\"." << std::endl;
            return false;
        }
        
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        char* buffer = new char[size + 1];
        file.read(buffer, size);
        buffer[size] = 0;
        file.close();

        bool success = read(document, buffer);
        delete[] buffer;

        return success;
    }
}
