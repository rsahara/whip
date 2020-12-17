//
//  StateMachineGenerator.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "StateMachineGenerator.hpp"

#include <sstream>

#include "Document/Document.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "Util/Text.hpp"

namespace Whip::SwiftFormat {
    StateMachineGenerator::StateMachineGenerator(const Document& document, std::ostream& output)
    : document(document)
    , output(output)
    , pseudoCodeGenerator(document) {
    }

    bool StateMachineGenerator::loadDocument() {
        implementationsClassName.clear();
        for (auto it = document.metas.begin(); it != document.metas.end(); it++) {
            const MetaData& metaData = *it;
            if (metaData.key == MetaDataKey::defineDelegateClass) {
                implementationsClassName = upperCamelCaseIdentifier(metaData.value + "Impl");
                stateMachineClassName = upperCamelCaseIdentifier(metaData.value);
                stateEnumName = upperCamelCaseIdentifier(metaData.value + "State");
                transitionEnumName = upperCamelCaseIdentifier(metaData.value + "Transition");
                eventEnumName = upperCamelCaseIdentifier(metaData.value + "Event");
                errorEnumName = upperCamelCaseIdentifier(metaData.value + "Error");
                delegateClassName = upperCamelCaseIdentifier(metaData.value + "Delegate");
            }
            else if (metaData.key == MetaDataKey::defineInitialState) {
                initialStateName = metaData.value;
            }
        }

        if (implementationsClassName.empty()) {
            errorLog() << "implementations class name undetermined." << std::endl;
            return false;
        }

        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;
            stateNameMap[state.name] = state.stateID;
        }

        for (auto it = document.transitions.begin(); it != document.transitions.end(); it++) {
            const Transition& transition = *it;
            transitionNameSet.emplace(transition.name);
        }

        return true;
    }

    bool StateMachineGenerator::generate() {
        loadDocument();

        generateCode();

        return true;
    }

    /// Generates code for switch cases of a given source state, in the transition function.
    struct StateTransitionGenerator: TransitionEnumerator::Delegate {
        const Document& document;
        const State& state;
        std::ostream& output;
        PseudoCodeGenerator& pseudoCodeGenerator;

        StateTransitionGenerator(const Document& document, const State& state, PseudoCodeGenerator& pseudoCodeGenerator, std::ostream& output)
        : document(document)
        , state(state)
        , output(output)
        , pseudoCodeGenerator(pseudoCodeGenerator) {
        }

        virtual void enumerateTransition(const Transition& transition,
                                         bool sameTransitionNameAsPrevious,
                                         bool sameTransitionNameAsNext) {
            if (!sameTransitionNameAsPrevious) {
                output << indentation(2) << "case (." << lowerCamelCaseIdentifier(state.name) << ", ." << transition.name << "):" << std::endl;
            }

            int indentationSize = 3;
            bool hasCondition = transition.conditionPseudoCodeID != invalidPseudoCodeID;
            if (hasCondition) {
                output << indentation(3) << "if ";
                const PseudoCode& pseudoCode = document.pseudoCodes[transition.conditionPseudoCodeID];
                pseudoCodeGenerator.generate(pseudoCode, output);
                output << " {" << std::endl;
                indentationSize = 4;
            }

            if (transition.dstStateID != invalidStateID) {
                const State& dstState = document.states[transition.dstStateID];
                if (transition.transitionPseudoCodeID != invalidPseudoCodeID) {
                    const PseudoCode& pseudoCode = document.pseudoCodes[transition.transitionPseudoCodeID];
                    output << indentation(indentationSize) << "return .changeStateAndExecute(" << std::endl;
                    output << indentation(indentationSize + 1) << "newState: ." << lowerCamelCaseIdentifier(dstState.name) << "," << std::endl;
                    output << indentation(indentationSize + 1) << "body: { ";
                    pseudoCodeGenerator.generate(pseudoCode, output);
                    output << " }" << std::endl;
                    output << indentation(indentationSize) << ")" << std::endl;
                } else {
                    output << indentation(indentationSize) << "return .changeState(newState: ." << lowerCamelCaseIdentifier(dstState.name) << ")" << std::endl;
                }
            } else {
                if (transition.transitionPseudoCodeID != invalidPseudoCodeID) {
                    const PseudoCode& pseudoCode = document.pseudoCodes[transition.transitionPseudoCodeID];
                    output << indentation(indentationSize) << "return .execute(body: { ";
                    pseudoCodeGenerator.generate(pseudoCode, output);
                    output << " })" << std::endl;
                } else {
                    output << indentation(indentationSize) << "return .doNothing" << std::endl;
                }
            }
            if (hasCondition) {
                output << indentation(3) << "}" << std::endl;
            }
        }
    };

    void StateMachineGenerator::generateCode() {
        bool initialStateExists = stateNameMap.find(initialStateName) != stateNameMap.end();

        // State machine class.
        output << "class " << stateMachineClassName << " {" << std::endl;
        output << indentation(1) << "private(set) var currentState: " << stateEnumName << std::endl;
        output << indentation(1) << "weak var implementations: " << implementationsClassName << "?" << std::endl;
        output << indentation(1) << "weak var delegate: " << delegateClassName << "?" << std::endl;
        if (initialStateExists) {
            output << indentation(1) << "static let initialState = " << stateEnumName << "." << lowerCamelCaseIdentifier(initialStateName) << std::endl;
        }
        output << indentation(1) << "init() {" << std::endl;
        output << indentation(2) << "currentState = .invalid" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << "}" << std::endl;
        output << std::endl;

        // Delegate protocol.
        output << "// Delegate for customizing some operations of the state machine." << std::endl;
        output << "protocol RegStateMachineDelegate: AnyObject {" << std::endl;
        output << indentation(1) << "func handleError(stateMachine: " << stateMachineClassName << ", error: Error)" << std::endl;
        output << indentation(1) << "func willPerformTransition(stateMachine: " << stateMachineClassName
            << ", forEvent event: " << eventEnumName << ")" << std::endl;
        output << "}" << std::endl;
        output << std::endl;

        // State machine class: functions for sending events.
        output << "// Functions for sending events." << std::endl;
        output << "extension " << stateMachineClassName << " {" << std::endl;
        if (initialStateExists) {
            output << indentation(1) << "func enterInitialState() { " << std::endl;
            output << indentation(2) << "do { try enterState(" << stateMachineClassName << ".initialState) }" << std::endl;
            output << indentation(2) << "catch { delegate?.handleError(stateMachine: self, error: error) }" << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        for (auto it = transitionNameSet.begin(); it != transitionNameSet.end(); it++) {
            output << indentation(1) << "func " << *it << "() {" << std::endl;
            output << indentation(2) << "do { try performTransition(forEvent: ." << *it << ") }" << std::endl;
            output << indentation(2) << "catch { delegate?.handleError(stateMachine: self, error: error) }" << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        output << "}" << std::endl;
        output << std::endl;

        // State machine class: basic functions.
        output << "extension " << stateMachineClassName << " {" << std::endl;
        output << indentation(1) << "func exitState() throws {" << std::endl;
        output << indentation(2) << "if let implementations = implementations {" << std::endl;
        output << indentation(3) << "try currentState.exit(implementations: implementations)" << std::endl;
        output << indentation(2) << "}" << std::endl;
        output << indentation(2) << "currentState = .invalid" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << indentation(1) << "func enterState(_ state: " << stateEnumName << ") throws {" << std::endl;
        output << indentation(2) << "currentState = state" << std::endl;
        output << indentation(2) << "if let implementations = implementations {" << std::endl;
        output << indentation(3) << "try currentState.enter(implementations: implementations)" << std::endl;
        output << indentation(2) << "}" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << indentation(1) << "func performTransition(forEvent event: " << eventEnumName << ") throws {" << std::endl;
        output << indentation(2) << "guard let implementations = implementations else { return }" << std::endl;
        output << indentation(2) << "delegate?.willPerformTransition(stateMachine: self, forEvent: event)" << std::endl;
        output << indentation(2) << "let state = currentState" << std::endl;
        output << indentation(2) << "let transition = state.transition(forEvent: event, implementations: implementations)" << std::endl;
        output << indentation(2) << "switch transition {" << std::endl;
        output << indentation(2) << "case .undefined:" << std::endl;
        output << indentation(3) << "throw " << errorEnumName << ".undefinedTransition(state: state, event: event)" << std::endl;
        output << indentation(2) << "case .doNothing: break" << std::endl;
        output << indentation(2) << "case let .execute(body):" << std::endl;
        output << indentation(3) << "try body()" << std::endl;
        output << indentation(2) << "case let .changeState(newState):" << std::endl;
        output << indentation(3) << "try exitState()" << std::endl;
        output << indentation(3) << "try enterState(newState)" << std::endl;
        output << indentation(2) << "case let .changeStateAndExecute(newState, body):" << std::endl;
        output << indentation(3) << "try exitState()" << std::endl;
        output << indentation(3) << "try body()" << std::endl;
        output << indentation(3) << "try enterState(newState)" << std::endl;
        output << indentation(2) << "}" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << "}" << std::endl;

        // Error enum.
        output << std::endl;
        output << "enum " << errorEnumName << ": Error {" << std::endl;
        output << indentation(1) << "case undefinedTransition(state: " << stateEnumName << ", event: " << eventEnumName << ")" << std::endl;
        output << "}" << std::endl;

        // Event enum.
        output << std::endl;
        output << "enum " << eventEnumName << " {" << std::endl;
        for (auto it = transitionNameSet.begin(); it != transitionNameSet.end(); it++) {
            output << indentation(1) << "case " << lowerCamelCaseIdentifier(*it) << std::endl;
        }
        output << "}" << std::endl;

        // Event enum: description.
        output << std::endl;
        output << "extension " << eventEnumName << ": CustomStringConvertible {" << std::endl;
        output << indentation(1) << "var description: String {" << std::endl;
        output << indentation(2) << "switch self {" << std::endl;
        for (auto it = transitionNameSet.begin(); it != transitionNameSet.end(); it++) {
            std::string caseName(lowerCamelCaseIdentifier(*it));
            output << indentation(2) << "case ." << caseName << ":" << std::endl;
            output << indentation(3) << "return \"" << caseName << "\"" <<  std::endl;
        }
        output << indentation(2) << "}" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << "}" << std::endl;

        // State enum.
        output << std::endl;
        output << "enum " << stateEnumName << " {" << std::endl;
        output << indentation(1) << "case invalid" << std::endl;
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;
            output << indentation(1) << "case " << lowerCamelCaseIdentifier(state.name) << std::endl;
        }
        output << "}" << std::endl;

        // State enum: description.
        output << std::endl;
        output << "extension " << stateEnumName << ": CustomStringConvertible {" << std::endl;
        output << indentation(1) << "var description: String {" << std::endl;
        output << indentation(2) << "switch self {" << std::endl;
        output << indentation(2) << "case .invalid:" << std::endl;
        output << indentation(3) << "return \"invalid\"" << std::endl;
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;
            std::string caseName(lowerCamelCaseIdentifier(state.name));
            output << indentation(2) << "case ." << caseName << ":" << std::endl;
            output << indentation(3) << "return \"" << caseName << "\"" <<  std::endl;
        }
        output << indentation(2) << "}" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << "}" << std::endl;

        // State enum: transition function.
        output << std::endl;
        output << "extension " << stateEnumName << " {" << std::endl;
        output << indentation(1) << "fileprivate func transition(" << std::endl;
        output << indentation(2) << "forEvent event: " << eventEnumName << "," << std::endl;
        output << indentation(2) << "implementations: " << implementationsClassName << std::endl;
        output << indentation(1) << ") -> " << transitionEnumName << " {" << std::endl;
        output << indentation(2) << "switch (self, event) {" << std::endl;
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;
            StateTransitionGenerator generator(document, state, pseudoCodeGenerator, output);
            TransitionEnumerator enumerator(document, state.stateID, generator);
            enumerator.enumerate();
        }
        output << indentation(2) << "default: break" << std::endl;
        output << indentation(2) << "}" << std::endl;
        output << indentation(2) << "return .undefined" << std::endl;
        output << indentation(1) << "}" << std::endl;

        // State enum: enter function.
        output << std::endl;
        output << indentation(1) << "fileprivate func enter(implementations: " << implementationsClassName << ") throws {" << std::endl;
        output << indentation(2) << "switch self {" << std::endl;
        output << indentation(2) << "case .invalid: break" << std::endl;
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;
            if (state.entryPseudoCodeID == invalidPseudoCodeID) {
                output << indentation(2) << "case ." << lowerCamelCaseIdentifier(state.name) << ": break" << std::endl;
            }
            else {
                const PseudoCode& pseudoCode = document.pseudoCodes[state.entryPseudoCodeID];
                output << indentation(2) << "case ." << lowerCamelCaseIdentifier(state.name) << ":" << std::endl << indentation(3);
                pseudoCodeGenerator.generate(pseudoCode, output);
                output << std::endl;
            }
        }
        output << indentation(2) << "}" << std::endl;
        output << indentation(1) << "}" << std::endl;

        // State: exit function.
        output << std::endl;
        output << indentation(1) << "fileprivate func exit(implementations: " << implementationsClassName << ") throws {" << std::endl;
        output << indentation(2) << "switch self {" << std::endl;
        output << indentation(2) << "case .invalid: break" << std::endl;
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;
            if (state.exitPseudoCodeID == invalidPseudoCodeID) {
                output << indentation(2) << "case ." << lowerCamelCaseIdentifier(state.name) << ": break" << std::endl;
            }
            else {
                const PseudoCode& pseudoCode = document.pseudoCodes[state.exitPseudoCodeID];
                output << indentation(2) << "case ." << lowerCamelCaseIdentifier(state.name) << ":" << std::endl << indentation(3);
                pseudoCodeGenerator.generate(pseudoCode, output);
                output << std::endl;
            }
        }
        output << indentation(2) << "}" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << "}" << std::endl;

        // Transition enum.
        output << std::endl;
        output << "fileprivate enum " << transitionEnumName << " {" << std::endl;
        output << indentation(1) << "case undefined" << std::endl;
        output << indentation(1) << "case doNothing" << std::endl;
        output << indentation(1) << "case execute(body: () throws -> Void)" << std::endl;
        output << indentation(1) << "case changeState(newState: " << stateEnumName << ")" << std::endl;
        output << indentation(1) << "case changeStateAndExecute(newState: " << stateEnumName << ", body: () throws -> Void)" << std::endl;
        output << "}" << std::endl;
    }
}
