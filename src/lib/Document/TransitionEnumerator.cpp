//
//  TransitionEnumerator.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "TransitionEnumerator.hpp"

#include <algorithm>

#include "Document/Document.hpp"

namespace Whip {

    TransitionEnumerator::TransitionEnumerator(const Document& document, int srcStateID, Delegate& delegate)
    : document(document)
    , srcStateID(srcStateID)
    , delegate(delegate) {
    }

    TransitionEnumerator::Delegate::~Delegate() {
    }

    void TransitionEnumerator::enumerate() {
        // Collect transition information.
        typedef std::vector<const Transition*> TransitionVector;
        TransitionVector selectedTransitions;
        for (auto it = document.transitions.begin(); it != document.transitions.end(); it++) {
            const Transition& transition = *it;
            if (transition.srcStateID != srcStateID) {
                continue;
            }
            selectedTransitions.emplace_back(&transition);
        }

        // Sort by transition name and condition.
        std::stable_sort(selectedTransitions.begin(), selectedTransitions.end(), compareTransitions);

        // Enumerate transitions taking care of names and conditions.
        Transition finalTransition;
        selectedTransitions.emplace_back(&finalTransition);
        bool sameTransitionNameAsPrevious = false;
        for (auto it = selectedTransitions.begin(); *it != &finalTransition;) {
            const Transition& transition = **it;
            it++;
            const Transition& nextTransition = **it;

            bool sameTransitionNameAsNext = (nextTransition.name == transition.name);
            delegate.enumerateTransition(transition, sameTransitionNameAsPrevious, sameTransitionNameAsNext);
            sameTransitionNameAsPrevious = sameTransitionNameAsNext;
        }
    }

    bool TransitionEnumerator::compareTransitions(const Transition* left, const Transition* right)
    {
        int compareName = left->name.compare(right->name);
        if (compareName < 0) {
            return true;
        }

        // If right condition is empty and left condition is not, then left is lower.
        if (compareName == 0
            && left->conditionPseudoCodeID != invalidPseudoCodeID
            && right->conditionPseudoCodeID == invalidPseudoCodeID) {
            return true;
        }
        
        return false;
    }
}
