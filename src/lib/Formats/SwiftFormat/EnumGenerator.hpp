//
//  EnumGenerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SwiftFormat_EnumGenerator_hpp
#define Whip_Formats_SwiftFormat_EnumGenerator_hpp

#include <unordered_set>
#include <vector>
#include <string>

#include "PseudoCode/PseudoCodeTraversal.hpp"

namespace Whip {
    struct Document;
    struct PseudoCodeAccessor;
}

namespace Whip::SwiftFormat {
    /// Generates the swift enum definition.
    struct EnumGenerator: PseudoCodeTraversal::Delegate {
        EnumGenerator(const Document& document, std::ostream& output, const std::string& enumName);

        /// Generates the enum.
        /// Returns false if something failed.
        bool generate();

    public:
        // PseudoCodeTraversal::Delegate
        virtual void traversePreOrder(Expression& expression, int expressionID);
        virtual void traverseInOrder(Expression& expression, int expressionID, int childIndex);
        virtual void traversePostOrder(Expression& expression, int expressionID);
        
    private:
        /// Set that contains the enum values we found.
        std::unordered_set<std::string> enumValuesSet;
        /// Set that contains the enum values in the order of appearance.
        std::vector<std::string> enumValues;

        /// Accessor for the pseudo code we're currently processing.
        PseudoCodeAccessor* pseudoCodeAccessor;

        /// Document to read.
        const Document& document;
        /// Stream where we will output data.
        std::ostream& output;
        /// The enum we're looking for.
        const std::string& enumName;
    };
}

#endif
