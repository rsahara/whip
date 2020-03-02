//
//  PseudoCodeLoader.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SmFormat_PseudoCodeLoader_hpp
#define Whip_Formats_SmFormat_PseudoCodeLoader_hpp

#include "PseudoCode/PseudoCodeTraversal.hpp"

namespace Whip {
    struct PseudoCodeAccessor;
    struct Expression;
    struct PseudoCode;
}

namespace Whip::SmFormat {
    /// A pseudo code loader for the sm format.
    /// Basically, it replaces the placeholder "ctxt" used to represent the delegate variable.
    struct PseudoCodeLoader: PseudoCodeTraversal::Delegate {
        PseudoCodeLoader(PseudoCode& pseudoCode, const char* inputText);
        virtual ~PseudoCodeLoader();

        bool load(int firstLine = 1);

    public:
        // PseudoCodeTraversal::Delegate
        virtual void traversePreOrder(Expression& expression, int expressionID);
        virtual void traverseInOrder(Expression& expression, int expressionID, int childIndex);
        virtual void traversePostOrder(Expression& expression, int expressionID);

    private:
        /// Where we'll store the pseudo code we read.
        PseudoCode& pseudoCode;
        /// First character of the input text.
        const char* inputText;

        /// Information about the expression tree.
        PseudoCodeAccessor* accessor;
    };

}

#endif
