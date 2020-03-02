//
//  Parser.cpp
//  Whip
//
//  Copyright © 2019 Runo Sahara. All rights reserved.
//

#include "Parser.hpp"

#include "Document/Document.hpp"
#include "PseudoCode/PseudoCode.hpp"
#include "Util/Debug.hpp"
#include "Util/Text.hpp"

#include "Token.hpp"
#include "PseudoCodeLoader.hpp"

namespace Whip::SmFormat {
    /// Delimiter between map and state name.
    const std::string mapStateDelimiter = "::";

    Parser::Parser(Document& document, const Token* firstToken)
    : document(document)
    , firstToken(firstToken)
    , token(firstToken) {
    }

    bool Parser::parse() {
        token = firstToken;
        stateNameMap.clear();
        ASSERT(token->type == TokenType::Begin, "Unexpected token");
        token++;
        while (parseTokens());
        return token->type == TokenType::End;
    }

    bool Parser::parseTokens() {
        switch (token->type) {
            case TokenType::Begin:
                token++;
                break;
                
            case TokenType::End:
                return false;
                
            case TokenType::Class:
                document.metas.emplace_back(MetaDataKey::defineDelegateClass, std::string(token[1].text));
                token += 2;
                break;
            case TokenType::Start:
                document.metas.emplace_back(MetaDataKey::defineInitialState, std::string(token[1].text));
                token += 2;
                break;
            case TokenType::Include:
                document.metas.emplace_back(MetaDataKey::moduleToImport, std::string(token[1].text));
                token += 2;
                break;
            case TokenType::Package:
                document.metas.emplace_back(MetaDataKey::definePackage, std::string(token[1].text));
                token += 2;
                break;
            case TokenType::Enum:
                document.metas.emplace_back(MetaDataKey::treatAsEnum, std::string(token[1].text));
                token += 2;
                break;
            case TokenType::Map:
                mapName = token[1].text;
                token += 2;
                if (!parseMap()) {
                    return false;
                }
                break;
                
            case TokenType::MapDelimiter:
            case TokenType::Word:
            case TokenType::BraceOpen:
            case TokenType::BraceClose:
            case TokenType::CondBegin:
            case TokenType::CondEnd:
            case TokenType::RawCode:
            case TokenType::Entry:
            case TokenType::Exit:
            case TokenType::Error:
            case TokenType::RawLine:
                ASSERT(false, "Unexpected token");
                return false;
        }
        return true;
    }
    
    bool Parser::parseMap() {
        ASSERT(token->type == TokenType::MapDelimiter, "Unexpected token");
        token++;

        for (;;) {
            switch (token->type) {
                case TokenType::MapDelimiter:
                    token++;
                    return true;
                case TokenType::Word:
                {
                    std::string tokenText(token->text);
                    std::string stateName(mapName + mapStateDelimiter + tokenText);
                    auto it = stateNameMap.find(stateName);
                    if (it != stateNameMap.end()) {
                        stateID = it->second;
                    } else {
                        stateID = (int)document.states.size();
                        document.states.emplace_back(stateID, stateName, tokenText);
                        stateNameMap[stateName] = stateID;
                    }
                    document.states[stateID].groupName = mapName;
                    token++;
                    if (!parseState()) {
                        return false;
                    }
                }
                    break;
                default:
                    ASSERT(false, "Unexpected token");
                    return false;
            }
        }
    }

    bool Parser::parseState() {
        if (token->type == TokenType::Entry) {
            token++;
            PseudoCode& pseudoCode = document.pseudoCodes.emplace_back();
            if (!parseBlockOfCode(pseudoCode)) {
                document.pseudoCodes.pop_back();
                return false;
            }
            if (pseudoCode.isEmpty()) {
                document.pseudoCodes.pop_back();
            }
            else {
                document.states[stateID].entryPseudoCodeID = (int)document.pseudoCodes.size() - 1;
            }
        }
        if (token->type == TokenType::Exit) {
            token++;
            PseudoCode& pseudoCode = document.pseudoCodes.emplace_back();
            if (!parseBlockOfCode(pseudoCode)) {
                document.pseudoCodes.pop_back();
                return false;
            }
            if (pseudoCode.isEmpty()) {
                document.pseudoCodes.pop_back();
            }
            else {
                document.states[stateID].exitPseudoCodeID = (int)document.pseudoCodes.size() - 1;
            }
        }
        ASSERT(token->type == TokenType::BraceOpen, "Unexpected token");
        token++;
        while (token->type == TokenType::Word) {
            Transition& transition = document.transitions.emplace_back();
            if (!parseTransition(transition)) {
                document.transitions.pop_back();
                return false;
            }
        }
        ASSERT(token->type == TokenType::BraceClose, "Unexpected token");
        token++;
        return true;
    }

    bool Parser::parseTransition(Transition& transition) {
        transition.name = token->text;
        transition.srcStateID = stateID;
        token++;

        if (token->type == TokenType::CondBegin) {
            token++;
            if (token->type == TokenType::RawCode) {
                PseudoCode& pseudoCode = document.pseudoCodes.emplace_back();
                std::string tokenText(token->text);
                PseudoCodeLoader pseudoCodeLoader(pseudoCode, tokenText.c_str());
                if (!pseudoCodeLoader.load(token->line)) {
                    document.pseudoCodes.pop_back();
                    return false;
                }
                if (pseudoCode.isEmpty()) {
                    document.pseudoCodes.pop_back();
                }
                else {
                    transition.conditionPseudoCodeID = (int)document.pseudoCodes.size() - 1;
                }
                token++;
            }
            ASSERT(token->type == TokenType::CondEnd, "Unexpected token");
            token++;
        }

        ASSERT(token->type == TokenType::Word, "Unexpected token");
        std::string dstStateName(token->text);
        if (dstStateName == "nil") {
            transition.dstStateID = invalidStateID;
        }
        else {
            std::string dstStateLabel;
            size_t delimiterPos = dstStateName.find(mapStateDelimiter);
            if (delimiterPos == std::string::npos) {
                dstStateLabel = dstStateName;
                dstStateName = mapName + mapStateDelimiter + dstStateName;
            }
            else {
                dstStateLabel = dstStateName.substr(delimiterPos + mapStateDelimiter.size());
            }
            
            auto it = stateNameMap.find(dstStateName);
            if (it != stateNameMap.end()) {
                transition.dstStateID = it->second;
            } else {
                transition.dstStateID = (int)document.states.size();
                document.states.emplace_back(transition.dstStateID, dstStateName, dstStateLabel);
                stateNameMap[dstStateName] = transition.dstStateID;
            }
        }
        token++;
        PseudoCode& pseudoCode = document.pseudoCodes.emplace_back();
        if (!parseBlockOfCode(pseudoCode)) {
            document.pseudoCodes.pop_back();
            return false;
        }
        if (pseudoCode.isEmpty()) {
            document.pseudoCodes.pop_back();
        }
        else {
            transition.transitionPseudoCodeID = (int)document.pseudoCodes.size() - 1;
        }
        return true;
    }

    bool Parser::parseBlockOfCode(PseudoCode& pseudoCode) {
        ASSERT(token->type == TokenType::BraceOpen, "Unexpected token");
        token++;
        std::string tokenText(token->text);
        PseudoCodeLoader pseudoCodeLoader(pseudoCode, tokenText.c_str());
        if (!pseudoCodeLoader.load(token->line)) {
            return false;
        }
        token++;
        ASSERT(token->type == TokenType::BraceClose, "Unexpected token");
        token++;
        return true;
    }
}
