//
//  Filename.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Filename.hpp"

namespace Whip {
    std::string filenameFromPath(const std::string& path) {
        size_t delimitPos = path.rfind('/');
        if (delimitPos == std::string::npos) {
            return path;
        }
        return path.substr(delimitPos + 1);
    }
    
    void basenameAndExtensionFromPath(std::string& basename, std::string& fileExtension, const std::string& path) {
        std::string filename = filenameFromPath(path);
        size_t delimitPos = filename.rfind('.');
        if (delimitPos == std::string::npos) {
            basename = filename;
            fileExtension.clear();
        }
        else {
            basename = filename.substr(0, delimitPos);
            fileExtension = filename.substr(delimitPos + 1);
        }
    }

    std::string pathFromDirectoryAndFilename(const std::string& directoryPath, const std::string& filename) {
        if (directoryPath.empty()) {
            return filename;
        }
        if (directoryPath.back() == '/') {
            return directoryPath + filename;
        }
        return directoryPath + "/" + filename;
    }

    std::string filenameFromBasenameAndExtension(const std::string& basename, const std::string& fileExtension) {
        return basename + "." + fileExtension;
    }
}
