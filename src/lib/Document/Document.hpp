//
//  Document.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Document_Document_hpp
#define Whip_Document_Document_hpp

#include <vector>
#include <string>

#include "PseudoCode/PseudoCode.hpp"

namespace Whip {
    /// State ID used to represent a state that doesn't exist.
    extern const int invalidStateID;
    /// Pseudo code ID used when there is no pseudo code associated with an event.
    extern const int invalidPseudoCodeID;

    /// State information.
    struct State {
        inline State();
        inline State(int stateID, const std::string& name, const std::string& label);
        inline State(const State& state);
        inline State& operator=(const State& state);

        /// State ID (ID is the index) to uniquely identify the state.
        int stateID;

        /// Name of the state.
        std::string name;
        /// Label that describes this state.
        std::string label;
        /// Name the group of this state.
        std::string groupName;

        /// ID of the pseudo code to execute when the state machine entered this state.
        int entryPseudoCodeID;
        /// ID of the pseudo code to execute when the state machine exited this state.
        int exitPseudoCodeID;
    };

    /// Transition information.
    struct Transition {
        inline Transition();
        inline Transition(const std::string& name, int srcStateID, int dstStateID);
        inline Transition(const Transition& transition);
        inline Transition& operator=(const Transition& transition);

        /// Name of the transition.
        std::string name;

        /// Source state ID.
        int srcStateID;
        /// Destination state ID.
        int dstStateID;

        /// ID of the condition code for this transition.
        int conditionPseudoCodeID;
        /// ID of the pseudo code to execute when this transition is executed.
        int transitionPseudoCodeID;
    };

    // Common meta data keys.
    namespace MetaDataKey {
        /// Specifies the name of the state machine's delegate.
        /// The associated value is the delegate class name.
        extern const std::string defineDelegateClass;
        /// Specifies to import the module in the generated code.
        /// The associated value is the name of the module.
        extern const std::string moduleToImport;
        /// Declare the package name when the code is generated.
        /// The associated value is the name of the package.
        extern const std::string definePackage;
        /// Specifies the name of the initial state.
        /// The associated value is the state name.
        extern const std::string defineInitialState;
        /// Specifies that the value acciated should be treated as a enum.
        /// The associated value is the enum identifier.
        extern const std::string treatAsEnum;
    }

    /// Meta data.
    struct MetaData {
        inline MetaData() {}
        inline MetaData(const MetaData& meta) { *this = meta; }
        inline MetaData(const std::string& key, const std::string& value);
        inline MetaData& operator=(const MetaData& meta);

        std::string key;
        std::string value;
    };

    /// Container for the definition of a state machine.
    struct Document {
        Document();

        /// Vector containing all meta infos.
        std::vector<MetaData> metas;
        /// Vector containing all states.
        std::vector<State> states;
        /// Vector containing all transitions.
        std::vector<Transition> transitions;
        /// Vector containing all the pseudo codes.
        std::vector<PseudoCode> pseudoCodes;
    };
}

// Inline

namespace Whip {
    inline State::State()
    : stateID(invalidStateID)
    , name()
    , label()
    , groupName()
    , entryPseudoCodeID(invalidPseudoCodeID)
    , exitPseudoCodeID(invalidPseudoCodeID) {
    }

    inline State::State(int stateID, const std::string& name, const std::string& label)
    : stateID(stateID)
    , name(name)
    , label(label)
    , groupName()
    , entryPseudoCodeID(invalidPseudoCodeID)
    , exitPseudoCodeID(invalidPseudoCodeID) {
    }

    inline State::State(const State& state) {
        *this = state;
    }

    inline State& State::operator=(const State& state) {
        stateID = state.stateID;
        name = state.name;
        label = state.label;
        groupName = state.groupName;
        entryPseudoCodeID = state.entryPseudoCodeID;
        exitPseudoCodeID = state.exitPseudoCodeID;
        return *this;
    }

    inline Transition::Transition()
    : name()
    , srcStateID(invalidStateID)
    , dstStateID(invalidStateID)
    , conditionPseudoCodeID(invalidPseudoCodeID)
    , transitionPseudoCodeID(invalidPseudoCodeID) {
    }

    inline Transition::Transition(const std::string& name, int srcStateID, int dstStateID)
    : name(name)
    , srcStateID(srcStateID)
    , dstStateID(dstStateID)
    , conditionPseudoCodeID(invalidPseudoCodeID)
    , transitionPseudoCodeID(invalidPseudoCodeID) {
    }

    inline Transition::Transition(const Transition& transition) {
        *this = transition;
    }

    inline Transition& Transition::operator=(const Transition& transition) {
        name = transition.name;
        srcStateID = transition.srcStateID;
        dstStateID = transition.dstStateID;
        conditionPseudoCodeID = transition.conditionPseudoCodeID;
        transitionPseudoCodeID = transition.transitionPseudoCodeID;
        return *this;
    }

    inline MetaData::MetaData(const std::string& key, const std::string& value)
    : key(key)
    , value(value) {
    }

    inline MetaData& MetaData::operator=(const MetaData& metaData) {
        key = metaData.key;
        value = metaData.value;
        return *this;
    }
}

#endif
