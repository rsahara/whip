//
//  SupportedFormats.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "SupportedFormats.hpp"

// Available readers
#include "Formats/SmDocumentReader.hpp"

// Available writers
#include "Formats/SwiftDocumentWriter.hpp"
#include "Formats/SwiftEnumDocumentWriter.hpp"
#include "Formats/KotlinDocumentWriter.hpp"
#include "Formats/DotGraphDocumentWriter.hpp"

namespace Whip {
    DocumentReader* documentReaderByFileExtension(const std::string& fileExtension) {
        if (fileExtension == "sm") {
            static SmDocumentReader reader;
            return &reader;
        }
        return nullptr;
    }
}

namespace Whip {
    DocumentWriter* documentWriterByFormatName(const std::string& formatName) {
        if (formatName == "swift") {
            static SwiftDocumentWriter writer;
            return &writer;
        }
        if (formatName == "swiftenum") {
            static SwiftEnumDocumentWriter writer;
            return &writer;
        }
        if (formatName == "kotlin") {
            static KotlinDocumentWriter writer;
            return &writer;
        }
        if (formatName == "dotgraph") {
            static DotGraphDocumentWriter writer;
            return &writer;
        }
        return nullptr;
    }
}
