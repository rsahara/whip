//
//  Filename.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Util_Filename_hpp
#define Whip_Util_Filename_hpp

#include <string>

namespace Whip {
    /// Returns the filename part of the a file path.
    std::string filenameFromPath(const std::string& path);
    /// Returns the basename and extension from a file path.
    void basenameAndExtensionFromPath(std::string& basename, std::string& fileExtension, const std::string& path);
    /// Creates a path with the specified directory path and filename.
    std::string pathFromDirectoryAndFilename(const std::string& directoryPath, const std::string& filename);
    /// Creates a filename with the specified basename and extension.
    std::string filenameFromBasenameAndExtension(const std::string& basename, const std::string& fileExtension);
}

#endif
