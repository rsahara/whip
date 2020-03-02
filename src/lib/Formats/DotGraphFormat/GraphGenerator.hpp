//
//  GraphGenerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_DotGraphFormat_GraphGenerator_hpp
#define Whip_Formats_DotGraphFormat_GraphGenerator_hpp

#include <ostream>
#include <vector>
#include <string>

#include "PseudoCode/BasicPseudoCodeGenerator.hpp"

namespace Whip {
    struct State;
    struct Document;
    struct Transition;
}

namespace Whip::DotGraphFormat {
    struct GraphGenerator {
        GraphGenerator(const Document& document, std::ostream& output);

        /// Generates the graph to the output.
        /// Returns false if something failed.
        bool generate();

    private:
        /// Generates subgraphs representing groups of states.
        bool generateStates();
        /// Generates each transition.
        bool generateTransitions();

        struct StateContext {
            inline StateContext() {}
            inline StateContext(const StateContext& stateContext) { *this = stateContext; }
            inline StateContext(const State* state, int groupID);
            inline StateContext& operator=(const StateContext& stateContext);
            const State* state;
            int groupID;
        };

        /// Compares state contexts.
        /// States contexts are first sorted by group id, then by state id.
        static bool compareStateContexts(const StateContext& left, const StateContext& right);
        /// Compares the source state of transitions.
        static bool compareTransitionSrcState(const Transition* left, const Transition* right);
        /// Escapes special characters.
        void escape(std::string& str);

    private:
        std::string diagramName;
        std::string initialStateName;

        typedef std::vector<StateContext> StateContextVector;
        /// Context of states, sorted by group and in order of appearance.
        StateContextVector stateContexts;
        typedef std::vector<const Transition*> TransitionVector;
        /// Null transitions (transitions with a null destination).
        TransitionVector nullTransitions;
        typedef std::vector<const Transition**> TransitionPointerVector;
        /// Pointer to the first null transition for each state. (State id (= index) -> Pointer.)
        TransitionPointerVector nullTransitionIndices;

        /// Document to read.
        const Document& document;
        /// Stream where we will output data.
        std::ostream& output;
        /// Writer that generates pseudo code for labels.
        BasicPseudoCodeGenerator pseudoCodeGenerator;
    };
}

// Inline

namespace Whip::DotGraphFormat {
    inline GraphGenerator::StateContext::StateContext(const State* state, int groupID)
    : state(state)
    , groupID(groupID) {
    }

    inline GraphGenerator::StateContext& GraphGenerator::StateContext::operator=(const StateContext& stateContext) {
        state = stateContext.state;
        groupID = stateContext.groupID;
        return *this;
    }
}

#endif
