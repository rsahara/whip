//
//  Parser.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SmFormat_Parser_hpp
#define Whip_Formats_SmFormat_Parser_hpp

#include <string>
#include <unordered_map>

namespace Whip {
    struct Document;
    struct Transition;
    struct PseudoCode;
}

namespace Whip::SmFormat {
    struct Token;

    /// Helper class that parses an array of tokens and creates a document.
    /// The array must begin with a TokenType::Begin token and end with TokenType::End token.
    struct Parser {
        Parser(Document& document, const Token* firstToken);

        /// Parses the provided tokens and creates a document.
        /// Returns false if there was an error.
        bool parse();

        /// Document that we parsed.
        const Document& result() const { return document; }

    private:
        /// Parses any type of tokens that we can expect.
        bool parseTokens();
        /// Parses the definition of a map of states.
        bool parseMap();
        /// Parses the definition of a state.
        bool parseState();
        /// Parses the definition of a transition.
        bool parseTransition(Transition& transition);
        /// Parses a block of code.
        bool parseBlockOfCode(PseudoCode& pseudoCode);

    private:
        /// Document we're creating.
        Document& document;
        /// First token of the input.
        const Token* firstToken;

        /// Current token that we're parsing in the input.
        const Token* token;

        /// Name of the map we're parsing.
        std::string mapName;
        /// State ID of the state we're parsing.
        int stateID;

        typedef std::unordered_map<std::string, int> StateNameMap;
        /// State name -> State ID map of states we have identified.
        StateNameMap stateNameMap;
    };
}

#endif
