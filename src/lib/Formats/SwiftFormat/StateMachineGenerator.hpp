//
//  StateMachineGenerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SwiftFormat_StateMachineGenerator_hpp
#define Whip_Formats_SwiftFormat_StateMachineGenerator_hpp

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

#include "Document/Document.hpp"
#include "Document/TransitionEnumerator.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCodeGenerator.hpp"

namespace Whip::SwiftFormat {
    /// Generates the state machine code in swift.
    struct StateMachineGenerator {
        StateMachineGenerator(const Document& document, std::ostream& output);

        /// Generates everything needed to the output.
        /// Returns false if something failed.
        bool generate();

    private:
        /// Loads the informations from the document.
        bool loadDocument();
        /// Generates the code.
        void generateCode();

    private:
        /// Class name of the delegate that implements function calls found in pseudo codes associated with transitions and conditions.
        /// The name is generated using the meta data.
        std::string delegateClassName;
        /// Name of the class representing the state machine, generated using the meta data.
        std::string stateMachineClassName;
        /// Name of the enum that contains every state, generated using the meta data.
        std::string stateEnumName;
        /// Name of the enum that contains every event, generated using the meta data.
        std::string eventEnumName;
        /// Name of the enum that contains transition types, generated using the meta data.
        std::string transitionEnumName;
        /// Name of the enum that contains errors that the state machine can throw, generated using the meta data.
        std::string errorEnumName;
        /// Name of the initial state, decided with the meta data.
        std::string initialStateName;

        typedef std::unordered_map<std::string, int> StateNameMap;
        /// State name -> State ID map.
        StateNameMap stateNameMap;

        typedef std::unordered_set<std::string> TransitionNameSet;
        /// Set of transition names.
        /// Multiple states may have the same transition.
        TransitionNameSet transitionNameSet;

        /// Document to read.
        const Document& document;
        /// Stream where we will output data.
        std::ostream& output;

        /// Writer that generates pseudo code in swift.
        PseudoCodeGenerator pseudoCodeGenerator;
    };
}

#endif
