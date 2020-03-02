//
//  TransitionEnumerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Document_TransitionEnumerator_hpp
#define Whip_Document_TransitionEnumerator_hpp

namespace Whip {
    struct Document;
    struct Transition;

    /// Helper class that enumerates transitions from a given state, to help generating the code of transitions, using a if-then-else logic.
    /// The transitions are sorted by name and by condition, before being enumerated.
    /// Among transitions with the same name, those with an empty condition will move to the end.
    /// The other transitions with a non-empty condition will keep their order.
    struct TransitionEnumerator {
        struct Delegate;
        TransitionEnumerator(const Document& document, int srcStateID, Delegate& delegate);

        void enumerate();

        struct Delegate {
            virtual ~Delegate();

            /// Function called for each transition.
            /// sameTransitionNameAsPrevious is true if the previous transition exists and has the same name.
            /// sameTransitionNameAsNext is true when the next transition exists and has the same name.
            virtual void enumerateTransition(const Transition& transition,
                                             bool sameTransitionNameAsPrevious,
                                             bool sameTransitionNameAsNext) = 0;
        };

    private:
        /// Compare transitions.
        /// Transitions are sorted by name, then by condition.
        /// Transitions with an empty condition are put after others.
        static bool compareTransitions(const Transition* left, const Transition* right);

    private:
        /// Document that contains all the transitions.
        const Document& document;
        /// Souce state ID of transitions we'll process.
        int srcStateID;
        /// Delegate that will receive the informations of enumerated transitions.
        Delegate& delegate;
    };
}

#endif
