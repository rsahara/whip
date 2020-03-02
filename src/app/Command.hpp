//
//  Command.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Command_Command_hpp
#define Command_Command_hpp

#include <vector>
#include <string>

struct Command {
    Command(int argumentCount, const char** argumentArray);

    int execute();

private:
    /// Processes the input file.
    bool processDocument(const std::string& path) const;
    /// Reads the arguments and updates the context.
    bool readArguments();
    /// Prints the usage to std::cout.
    void printUsage() const;

private:
    /// Array of arguments passed at launch time.
    const char** argumentArray;
    /// Number of arguments.
    int argumentCount;

    /// Input files that we'll process.
    std::vector<std::string> inputFiles;
    /// Output format name (default: swift).
    std::string formatName;
    /// Output directory path (default: current directory).
    std::string directoryPath;
    /// Parameters specified in the command.
    std::vector<std::string> parameters;
    /// Whether the user wants to see the usage.
    bool needHelp;

    /// Default value for formatName.
    static const std::string defaultFormatName;
    /// Default value for directoryPath.
    static const std::string defaultDirectoryPath;
};

#endif
