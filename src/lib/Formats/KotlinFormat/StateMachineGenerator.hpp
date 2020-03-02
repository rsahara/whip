//
//  StateMachineGenerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_KotlinFormat_StateMachineGenerator_hpp
#define Whip_Formats_KotlinFormat_StateMachineGenerator_hpp

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <fstream>

#include "Document/Document.hpp"
#include "Document/TransitionEnumerator.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "PseudoCodeGenerator.hpp"

namespace Whip::KotlinFormat {
    /// Generates the state machine code in kotlin.
    struct StateMachineGenerator: TransitionEnumerator::Delegate {
        StateMachineGenerator(const Document& document, std::ostream& output);

        /// Generates everything needed to the output.
        /// Returns false if something failed.
        bool generate();

    public:
        virtual void enumerateTransition(const Transition& transition,
                                         bool sameTransitionNameAsPrevious,
                                         bool sameTransitionNameAsNext);

    private:
        /// Loads the informations from the document.
        bool loadDocument();
        /// Generates the code of base classes.
        void generateInterfaceCode();
        /// Generates the code of a state.
        void generateStateCode(const State& state);

    private:
        /// Package name where we'll define the classes.
        std::string packageName;
        /// Class name of the delegate that implements function calls found in pseudo codes associated with transitions and conditions.
        /// The name is generated using the meta data.
        std::string delegateClassName;
        /// Name of the class representing the state machine, generated using the meta data.
        std::string stateMachineClassName;
        /// Name of the root class of every states, generated using the meta data.
        std::string stateClassName;
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

        /// Writer that generates pseudo code in kotlin.
        PseudoCodeGenerator pseudoCodeGenerator;
    };
}

#endif
