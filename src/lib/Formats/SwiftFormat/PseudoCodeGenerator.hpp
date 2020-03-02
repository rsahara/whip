//
//  PseudoCodeGenerator.hpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#ifndef Whip_Formats_SwiftFormat_PseudoCodeGenerator_hpp
#define Whip_Formats_SwiftFormat_PseudoCodeGenerator_hpp

#include <ostream>
#include <unordered_set>
#include <string>

#include "PseudoCode/BasicPseudoCodeGenerator.hpp"

namespace Whip {
    struct Document;
}

namespace Whip::SwiftFormat {
    /// A pseudo code generator for the swift language.
    /// Generates swift code that represents the pseudo code by traversing the expression tree.
    struct PseudoCodeGenerator: BasicPseudoCodeGenerator::Writer {
        PseudoCodeGenerator(const Document& document);
        virtual ~PseudoCodeGenerator();

        void generate(const PseudoCode& pseudoCode, std::ostream& output);

    public:
        // BasicPseudoCodeGenerator::Writer
        virtual void writeCommonWord(const Expression& expression, int expressionID, std::ostream& output);
        virtual void writeDelegateWord(const Expression& expression, int expressionID, std::ostream& output);
        virtual void writeFunctionCall(const Expression& expression, int expressionID, std::ostream& output);

    private:
        /// Set of enum names.
        std::unordered_set<std::string> enumSet;
        /// Pseudo code to output. Valid only inside the generate function.
        PseudoCodeAccessor* pseudoCodeAccessor;
    };
}

#endif
