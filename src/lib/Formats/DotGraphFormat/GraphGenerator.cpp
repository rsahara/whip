//
//  GraphGenerator.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "GraphGenerator.hpp"

#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <algorithm>

#include "Document/Document.hpp"
#include "Util/Text.hpp"

namespace Whip::DotGraphFormat {
    /// Name of the initial node (the circle).
    const std::string initialNodeName("__initialNode");
    /// Last element of nullTransitions.
    static const Transition nullTransitionEnd;

    GraphGenerator::GraphGenerator(const Document& document, std::ostream& output)
    : document(document)
    , output(output) {
    }

    bool GraphGenerator::generate() {
        // Check meta infos.
        for (auto it = document.metas.begin(); it != document.metas.end(); it++) {
            const MetaData& metaData = *it;
            if (metaData.key == MetaDataKey::defineDelegateClass) {
                diagramName = metaData.value;
            }
            else if (metaData.key == MetaDataKey::defineInitialState) {
                initialStateName = metaData.value;
            }
        }

        // Check state infos.
        typedef std::unordered_map<std::string, int> GroupNameMap;
        GroupNameMap groupNameMap; // Group name -> Group id.
        for (auto it = document.states.begin(); it != document.states.end(); it++) {
            const State& state = *it;

            int groupID = 0;
            auto groupNameIt = groupNameMap.find(state.groupName);
            if (groupNameIt == groupNameMap.end()) {
                groupID = (int)groupNameMap.size();
                groupNameMap[state.groupName] = groupID;
            }
            else {
                groupID = groupNameIt->second;
            }
            stateContexts.emplace_back(&state, groupID);
        }
        std::stable_sort(stateContexts.begin(), stateContexts.end(), compareStateContexts);

        // Create vector of null transitions.
        nullTransitionIndices.resize(document.states.size(), nullptr);
        for (auto it = document.transitions.begin(); it != document.transitions.end(); it++) {
            const Transition& transition = *it;
            if (transition.dstStateID == invalidStateID) {
                nullTransitions.push_back(&transition);
            }
        }
        std::stable_sort(nullTransitions.begin(), nullTransitions.end(), compareTransitionSrcState);
        nullTransitions.push_back(&nullTransitionEnd); // Terminal element.

        // Create an index of null transitions to fasten the access. (nullTransitions will not change anymore.)
        for (auto it = nullTransitions.begin();; it++) {
            const Transition*& transition = *it;
            if (transition == &nullTransitionEnd) {
                break;
            }
            const Transition** head = nullTransitionIndices[transition->srcStateID];
            if (head == nullptr) {
                nullTransitionIndices[transition->srcStateID] = &transition;
            }
        }

        output << "digraph " << diagramName << " {" << std::endl;
        output << indentation(1) << "node" << std::endl;
        output << indentation(2) << "[shape=plain fontsize=14];" << std::endl;
        output << indentation(1) << "graph" << std::endl;
        output << indentation(2) << "[color=gray40 fontcolor=gray40 fontsize=18];" << std::endl;

        generateStates();
        generateTransitions();

        output << "}" << std::endl;

        return true;
    }

    bool GraphGenerator::generateStates() {
        for (auto stateIt = stateContexts.begin(); stateIt != stateContexts.end(); ) {
            std::string groupName = stateIt->state->groupName;
            escape(groupName);

            output << indentation(1) << "subgraph cluster_" << stateIt->state->groupName << " {" << std::endl;
            output << indentation(2) << "label=<" << groupName << ">;" << std::endl;

            bool containsInitialState = false;
            for (; stateIt != stateContexts.end() && stateIt->state->groupName == groupName; stateIt++) {
                const StateContext& stateContext = *stateIt;
                const State& state = *stateContext.state;

                // Generate label.
                std::string nodeLabelName(state.label);
                escape(nodeLabelName);
                output << indentation(2) << "\"" << state.name << "\"" << std::endl;
                output << indentation(3) << "[label=<";
                output << "<table border=\"1\" cellspacing=\"0\" cellborder=\"0\" cellpadding=\"5\" style=\"rounded\" bgcolor=\"gray97\" rows=\"*\">";
                output << "<tr><td align=\"center\">" << nodeLabelName << "</td></tr>";

                // Entry/exit actions.
                std::ostringstream labelStream;
                if (state.entryPseudoCodeID != invalidPseudoCodeID) {
                    labelStream << "entry:\n" << indentation(1);
                    pseudoCodeGenerator.generate(document.pseudoCodes[state.entryPseudoCodeID], labelStream);
                    labelStream << "\n";
                }
                if (state.exitPseudoCodeID != invalidPseudoCodeID) {
                    labelStream << "exit:\n" << indentation(1);
                    pseudoCodeGenerator.generate(document.pseudoCodes[state.exitPseudoCodeID], labelStream);
                    labelStream << "\n";
                }

                // Null transitions.
                const Transition** nullTransitionPtr = nullTransitionIndices[state.stateID];
                if (nullTransitionPtr != nullptr) {
                    for (; (*nullTransitionPtr)->srcStateID == state.stateID; nullTransitionPtr++) {
                        const Transition& nullTransition = **nullTransitionPtr;
                        labelStream << "" << nullTransition.name << "()";
                        if (nullTransition.conditionPseudoCodeID != invalidPseudoCodeID) {
                            labelStream << " [";
                            pseudoCodeGenerator.generate(document.pseudoCodes[nullTransition.conditionPseudoCodeID], labelStream);
                            labelStream << "]";
                        }
                        if (nullTransition.transitionPseudoCodeID != invalidPseudoCodeID) {
                            labelStream << ":\n" << indentation(1);
                            pseudoCodeGenerator.generate(document.pseudoCodes[nullTransition.transitionPseudoCodeID], labelStream);
                            labelStream << "\n";
                        }
                    }
                }

                if (!labelStream.str().empty()) {
                    std::string infoLabel(labelStream.str());
                    escape(infoLabel);
                    output << "<tr><td align=\"left\" balign=\"left\"><font point-size=\"14\">" << infoLabel << "</font></td></tr>";
                }

                output << "</table>>];" << std::endl;

                if (initialStateName == state.name) {
                    containsInitialState = true;
                }
            }

            if (containsInitialState) {
                output << indentation(2) << "\"" << initialNodeName <<  "\"" << std::endl;
                output << indentation(3) << "[label=\"\" shape=circle style=filled fillcolor=black width=0.25];" << std::endl;
            }

            output << indentation(1) << "}" << std::endl;
        }

        return true;
    }

    bool GraphGenerator::generateTransitions() {
        for (auto it = document.transitions.begin(); it != document.transitions.end(); it++) {
            const Transition& transition = *it;
            if (transition.dstStateID == invalidStateID) {
                // No transition.
                continue;
            }

            const State& srcState = document.states[transition.srcStateID];
            const State& dstState = document.states[transition.dstStateID];

            // Generate label.
            std::ostringstream labelStream;
            labelStream << transition.name << "()";
            if (transition.conditionPseudoCodeID != invalidPseudoCodeID) {
                labelStream << "\n[";
                pseudoCodeGenerator.generate(document.pseudoCodes[transition.conditionPseudoCodeID], labelStream);
                labelStream << "]";
            }
            if (transition.transitionPseudoCodeID != invalidPseudoCodeID) {
                labelStream << ":\n";
                pseudoCodeGenerator.generate(document.pseudoCodes[transition.transitionPseudoCodeID], labelStream);
                labelStream << "\n";
            }
            std::string label(labelStream.str());
            escape(label);
            output << indentation(1) << "\"" << srcState.name << "\" -> \"" << dstState.name << "\"" << std::endl;
            output << indentation(2) << "[label=<<table border=\"0\" cellspacing=\"0\" cellborder=\"0\" cellpadding=\"5\">";
            output << "<tr><td align=\"left\" balign=\"left\"><font point-size=\"14\">" << label << "</font></td></tr>";
            output << "</table>>];" << std::endl;
        }
        output << indentation(1) << "\"" << initialNodeName << "\" -> \"" << initialStateName << "\";" << std::endl;

        return true;
    }

    bool GraphGenerator::compareStateContexts(const StateContext& left, const StateContext& right) {
        if (left.groupID < right.groupID) {
            return true;
        }
        if (left.groupID == right.groupID) {
            return left.state->stateID < right.state->stateID;
        }
        return false;
    }

    bool GraphGenerator::compareTransitionSrcState(const Transition* left, const Transition* right) {
        return left->srcStateID < right->srcStateID;
    }

    void GraphGenerator::escape(std::string& str) {
        replaceAllOccurences(str, "&", "&amp;");
        replaceAllOccurences(str, ">", "&gt;");
        replaceAllOccurences(str, "<", "&lt;");
        replaceAllOccurences(str, " ", "&nbsp;");
        replaceAllOccurences(str, "\n", "<br />");
    }
}
