//
//  Command.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Command.hpp"

#include <string>
#include <iostream>

#include "Whip.hpp"

using namespace Whip;

const std::string Command::defaultFormatName("swift");
const std::string Command::defaultDirectoryPath(".");

Command::Command(int argumentCount, const char** argumentArray)
: argumentArray(argumentArray)
, argumentCount(argumentCount)
, inputFiles()
, formatName()
, directoryPath()
, parameters()
, needHelp(false) {
}

void Command::printUsage() const {
    std::cout << "Usage: " << argumentArray[0] << " [options] file ..." << std::endl;
    std::cout << "State machine generator." << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "--dir=path" << std::endl;
    std::cout << "\tSpecifies the output directory of the files to generate." << std::endl;
    std::cout << "\tBy default, it is \"" << defaultDirectoryPath <<  "\"." << std::endl;
    std::cout << "--format=key" << std::endl;
    std::cout << "\tSpecifies the output format." << std::endl;
    std::cout << "--param=value" << std::endl;
    std::cout << "\tSpecifies a parameter for the output format." << std::endl;
    std::cout << std::endl;
    std::cout << "Supported output formats:" << std::endl;
    std::cout << "--format=swift (default)" << std::endl;
    std::cout << "\tSwift code representing the state machine." << std::endl;
    std::cout << "--format=swiftenum --param=EnumName" << std::endl;
    std::cout << "\tDefinition of an enum, containing all values found in pseudo codes." << std::endl;
    std::cout << "--format=kotlin" << std::endl;
    std::cout << "\tKotlin code representing the state machine." << std::endl;
    std::cout << "--format=dotgraph" << std::endl;
    std::cout << "\tGraphviz graph (dot file) representing the state machine." << std::endl;
    std::cout << std::endl;
    std::cout << "Supported input file extensions:" << std::endl;
    std::cout << ".sm" << std::endl;
    std::cout << "\tState machine definition file. See the specifications on github." << std::endl;
    std::cout << std::endl;
}

int Command::execute() {
    inputFiles.clear();
    formatName = defaultFormatName;
    directoryPath = defaultDirectoryPath;
    parameters.clear();
    needHelp = false;

    if (argumentCount == 1) {
        printUsage();
        return 1;
    }

    if (!readArguments()) {
        if (needHelp) {
            printUsage();
        }
        return 1;
    }
    if (inputFiles.empty()) {
        errorLog() << "no input files." << std::endl;
        return 1;
    }

    bool successful = true;
    for (auto it = inputFiles.begin(); it != inputFiles.end(); it++) {
        successful = processDocument(*it) && successful;
    }

    return successful ? 0 : 1;
}

bool Command::processDocument(const std::string& path) const {
    Document document;

    std::string filename = filenameFromPath(path);
    std::string basename;
    std::string fileExtension;
    basenameAndExtensionFromPath(basename, fileExtension, path);

    DocumentReader* reader = documentReaderByFileExtension(fileExtension);
    if (reader == nullptr) {
        errorLog() << "\"" << fileExtension << "\" extension not supported." << std::endl;
        return false;
    }
    if (!reader->readFromFile(document, path)) {
        errorLog() << "errors occured while reading: \"" << path << "\"." << std::endl;
        return false;
    }

    DocumentWriter* writer = documentWriterByFormatName(formatName);
    if (!writer->writeToDirectory(directoryPath, basename, document, parameters)) {
        errorLog() << "errors occured while generating files in the format: \"" << formatName << "\"." << std::endl;
        return false;
    }

    return true;
}

/// Scans "option=value" from an argument.
bool scanOption(std::string& optionKey, std::string& optionValue, const std::string_view& argument) {
    if (argument.size() < 3) {
        return false;
    }
    if (argument[0] != '-' || argument[1] != '-') {
        return false;
    }
    size_t delimiterPosition = argument.find('=', 2);
    if (delimiterPosition == std::string::npos) {
        optionKey = argument.substr(2);
        optionValue.clear();
    }
    else if (delimiterPosition > 2) {
        optionKey = argument.substr(2, delimiterPosition - 2);
        optionValue = argument.substr(delimiterPosition + 1);
    }
    else {
        return false;
    }
    return true;
}

bool Command::readArguments() {
    const char** argumentEnd = argumentArray + argumentCount;
    for (const char** argumentIt = argumentArray + 1; argumentIt != argumentEnd; argumentIt++) {
        std::string_view argument(*argumentIt);

        if (argument == "-h" || argument == "--help") {
            needHelp = true;
            return false;
        }

        std::string optionKey;
        std::string optionValue;
        if (scanOption(optionKey, optionValue, argument)) {
            if (optionKey == "dir") {
                directoryPath = optionValue;
            }
            else if (optionKey == "format") {
                if (documentWriterByFormatName(optionValue) == nullptr) {
                    errorLog() << "unknown format: \"" << optionValue << "\"." << std::endl;
                    return false;
                }
                formatName = optionValue;
            }
            else if (optionKey == "param") {
                parameters.emplace_back(optionValue);
            }
            else {
                errorLog() << "invalid option: \"" << optionKey << "\"." << std::endl;
                return false;
            }
        }
        else {
            inputFiles.emplace_back(argument);
        }
    }

    return true;
}
