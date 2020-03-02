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

namespace Whip::KotlinFormat {
    StateMachineGenerator::StateMachineGenerator(const Document& document, std::ostream& output)
    : document(document)
    , output(output)
    , pseudoCodeGenerator(document) {
    }

    bool StateMachineGenerator::loadDocument() {
        delegateClassName.clear();
        for (auto it = document.metas.begin(); it != document.metas.end(); it++) {
            const MetaData& metaData = *it;
            if (metaData.key == MetaDataKey::defineDelegateClass) {
                delegateClassName = metaData.value + "Impl";
                stateMachineClassName = metaData.value;
                stateClassName = metaData.value + "State";
            }
            else if (metaData.key == MetaDataKey::defineInitialState) {
                initialStateName = metaData.value;
            }
            else if (metaData.key == MetaDataKey::definePackage) {
                packageName = metaData.value;
            }
        }

        if (delegateClassName.empty()) {
            errorLog() << "delegate class name undetermined." << std::endl;
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

        generateInterfaceCode();

        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            generateStateCode(*it);
        }

        return true;
    }

    void StateMachineGenerator::generateInterfaceCode() {
        std::string invalidStateClassName = stateClassName + "Invalid";

        // Declaration of the package.
        if (!packageName.empty()) {
            output << "package " << packageName << std::endl;
            output << std::endl;
        }

        // Imports.
        output << "import android.util.SparseArray" << std::endl;

        // State machine class.
        output << "class " << stateMachineClassName << "(val delegate: " << delegateClassName << ") {" << std::endl;
        output << indentation(1) << "var currentState: " << stateClassName << " = " << invalidStateClassName << std::endl;
        output << indentation(2) << "private set" << std::endl;
        if (stateNameMap.find(initialStateName) != stateNameMap.end()) {
            output << indentation(1) << "fun enterInitialState() { " << std::endl;
            output << indentation(2) << "enterState(" << upperCamelCaseIdentifier(stateClassName + initialStateName) << ")" << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        output << indentation(1) << "fun exitState() {" << std::endl;
        output << indentation(2) << "currentState.exitState(delegate)" << std::endl;
        output << indentation(2) << "currentState = " << invalidStateClassName << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << indentation(1) << "fun enterState(state: " << stateClassName << ") {" << std::endl;
        output << indentation(2) << "currentState = state" << std::endl;
        output << indentation(2) << "currentState.enterState(delegate)" << std::endl;
        output << indentation(1) << "}" << std::endl;

        for (auto it = transitionNameSet.begin(); it != transitionNameSet.end(); it++) {
            output << indentation(1) << "fun " << *it << "() {" << std::endl;
            output << indentation(2) << "currentState." << *it << "(this, delegate)" << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        output << "}" << std::endl;

        // State class.
        output << std::endl;
        output << "abstract class " << stateClassName << " {" << std::endl;
        output << indentation(1) << "abstract val name: String" << std::endl;
        output << indentation(1) << "abstract val id: Int" << std::endl;
        for (auto it = transitionNameSet.begin(); it != transitionNameSet.end(); it++) {
            output << indentation(1) << "open fun " << *it << "(owner: " << stateMachineClassName << ", delegate: " << delegateClassName << ") {" << std::endl;
            output << indentation(2) << "delegate.defaultTransition(this, \"" << *it << "\")" << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        output << indentation(1) << "open fun enterState(delegate: " << delegateClassName << ") {" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << indentation(1) << "open fun exitState(delegate: " << delegateClassName << ") {" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << indentation(1) << "companion object {" << std::endl;
        output << indentation(2) << "private val idToStateMap: SparseArray<" << stateClassName << "> by lazy {" << std::endl;
        output << indentation(3) << "SparseArray<" << stateClassName << ">().apply {" << std::endl;
        output << indentation(4) << "append(" << std::endl;
        output << indentation(5) << invalidStateClassName << ".id," << std::endl;
        output << indentation(5) << invalidStateClassName << std::endl;
        output << indentation(4) << ")" << std::endl;
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            std::string thisClassName(upperCamelCaseIdentifier(stateClassName + it->name));
            output << indentation(4) << "append(" << std::endl;
            output << indentation(5) << thisClassName << ".id," << std::endl;
            output << indentation(5) << thisClassName << std::endl;
            output << indentation(4) << ")" << std::endl;
        }
        output << indentation(3) << "}" << std::endl;
        output << indentation(2) << "}" << std::endl;
        output << indentation(2) << "fun valueOf(id: Int) = idToStateMap.get(id, " << invalidStateClassName<< ")" << std::endl;
        output << indentation(1) << "}" << std::endl;
        output << "}" << std::endl;

        // State class for an invalid state.
        output << std::endl;
        output << "object " << invalidStateClassName << " : " << stateClassName << "() {" << std::endl;
        output << indentation(1) << "override val name = \"" << upperCamelCaseIdentifier("Invalid") << "\"" << std::endl;
        output << indentation(1) << "override val id = -1" << std::endl;
        output << "}" << std::endl;
    }

    void StateMachineGenerator::generateStateCode(const State& state) {
        output << std::endl;
        output << "object " <<  upperCamelCaseIdentifier(stateClassName + state.name) << " : " << stateClassName << "() {" << std::endl;
        output << indentation(1) << "override val name = \"" << upperCamelCaseIdentifier(state.name) << "\"" << std::endl;
        output << indentation(1) << "override val id = " << state.stateID << std::endl;
        // Entry function.
        if (state.entryPseudoCodeID != invalidPseudoCodeID) {
            output << indentation(1) << "override fun enterState(delegate: " << delegateClassName << ") {" << std::endl;
            output << indentation(2);
            const PseudoCode& pseudoCode = document.pseudoCodes[state.entryPseudoCodeID];
            pseudoCodeGenerator.generate(pseudoCode, output);
            output << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        // Exit function.
        if (state.exitPseudoCodeID != invalidPseudoCodeID) {
            output << indentation(1) << "override fun exitState(delegate: " << delegateClassName << ") {" << std::endl;
            output << indentation(2);
            const PseudoCode& pseudoCode = document.pseudoCodes[state.exitPseudoCodeID];
            pseudoCodeGenerator.generate(pseudoCode, output);
            output << std::endl;
            output << indentation(1) << "}" << std::endl;
        }
        // Transition functions.
        TransitionEnumerator enumerator(document, state.stateID, *this);
        enumerator.enumerate();
        output << "}" << std::endl;
    }

    void StateMachineGenerator::enumerateTransition(const Transition& transition,
                                                    bool sameTransitionNameAsPrevious,
                                                    bool sameTransitionNameAsNext) {
        if (!sameTransitionNameAsPrevious) {
            output << indentation(1) << "override fun " << transition.name << "(owner: " << stateMachineClassName << ", delegate: " << delegateClassName << ") {" << std::endl;
        }

        int indentationSize = 2;
        bool hasCondition = transition.conditionPseudoCodeID != invalidPseudoCodeID;
        if (hasCondition) {
            if (!sameTransitionNameAsPrevious) {
                output << indentation(2);
            }
            output << "if (";
            const PseudoCode& pseudoCode = document.pseudoCodes[transition.conditionPseudoCodeID];
            pseudoCodeGenerator.generate(pseudoCode, output);
            output << ") {" << std::endl;
            indentationSize = 3;
        }
        else if (sameTransitionNameAsPrevious) {
            output << "{" << std::endl;
            indentationSize = 3;
        }

        bool changeState = transition.dstStateID != invalidStateID;
        if (changeState) {
            output << indentation(indentationSize) << "owner.exitState()" << std::endl;
        }
        if (transition.transitionPseudoCodeID != invalidPseudoCodeID) {
            output << indentation(indentationSize);
            const PseudoCode& pseudoCode = document.pseudoCodes[transition.transitionPseudoCodeID];
            pseudoCodeGenerator.generate(pseudoCode, output);
            output << std::endl;
        }
        if (changeState) {
            const State& dstState = document.states[transition.dstStateID];
            output << indentation(indentationSize) << "owner.enterState(" << upperCamelCaseIdentifier(stateClassName + dstState.name) << ")" << std::endl;
        }
        if (hasCondition || sameTransitionNameAsPrevious) {
            output << indentation(2) << "}" << std::endl;
        }
        if (sameTransitionNameAsNext) {
            output << indentation(2) << "else ";
        }
        else {
            output << indentation(1) << "}" << std::endl;
        }
    }
}
