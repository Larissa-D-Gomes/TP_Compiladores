/**
 * @file SyntaxAnalyzer.cpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Syntax Analyzer structure
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <string>

#include "headers/SyntaxAnalyzer.hpp"
#include "headers/LexicalRegister.hpp"
#include "headers/LexicalAnalyzer.hpp"
#include "headers/Utils.hpp"
#include "headers/Throws.hpp"
#include "headers/Alphabet.hpp"
#include "headers/ConstType.hpp"
#include "headers/SymbolTable.hpp"
#include "headers/ClassType.hpp"

#define finalState -1
#define null -999

using namespace std;

/**
 * @brief Construct a new Syntax Analyzer object
 *
 */
SyntaxAnalyzer::SyntaxAnalyzer()
{
    // Initializing lexeme with a char != of eof flag
    this->regLex = LexicalRegister("", null, null, null, null);
}

/**
 * @brief Match token from Lexical Analyzer and the expected token by L Language Grammar
 *
 * @param expectedToken Token expected by L Language Grammar
 */
void SyntaxAnalyzer::matchToken(int expectedToken)
{

    cout << "AnalisadorLexico: [" << testLexem(this->token) << "] Esperado na Gramatica: [" << testLexem(expectedToken) << "]" << endl;

    // Verify if read token by LexicalAnalyzer matches the expected token by L Language Grammar
    if (this->token == expectedToken)
    {
        this->regLex = lexicalAnalyzer();
        // printLexicalRegister(this->regLex);
        this->token = this->regLex.token;
    }
    // Throws exceptions if the token doesn't match the expected token
    else
    {
        if (cursor == eof)
        {
            throwUnexpectedEOFException();
        }
        else
        {
            throwUnexpectedToken(this->regLex.lexeme);
        }
    }
}

/**
 * @brief Check if actual token is included in the first tokens of DEC grammar variable
 *
 * @return true - token is first of DEC variable
 * @return false - token is NOT first of DEC variable
 */
bool SyntaxAnalyzer::checkFirstDEC()
{
    return this->token == Alphabet::INT ||
           this->token == Alphabet::FLOAT ||
           this->token == Alphabet::STRING ||
           this->token == Alphabet::BOOLEAN ||
           this->token == Alphabet::CHAR ||
           this->token == Alphabet::CONST ||
           this->token == Alphabet::ID;
}

/**
 * @brief Check if actual token is included in the first tokens of ATR grammar variable
 *
 * @return true - token is first of ATR variable
 * @return false - token is NOT first of ATR variable
 */
bool SyntaxAnalyzer::checkFirstATR()
{
    return this->token == Alphabet::ID;
}

/**
 * @brief Check if actual token is included in the first tokens of DECONST grammar variable
 *
 * @return true - token is first of DECONST variable
 * @return false - token is NOT first of DECONST variable
 */
bool SyntaxAnalyzer::checkFirstDECONST()
{
    return this->token == Alphabet::CONSTANT;
}

/**
 * @brief Check if actual token is included in the first tokens of CMD grammar variable
 *
 * @return true - token is first of CMD variable
 * @return false - token is NOT first of CMD variable
 */
bool SyntaxAnalyzer::checkFirstCMD()
{
    return this->token == Alphabet::WHILE ||
           this->token == Alphabet::IF ||
           this->token == Alphabet::SEMICOLON ||
           this->token == Alphabet::READLN ||
           this->token == Alphabet::WRITE ||
           this->token == Alphabet::WRITELN;
}

/**
 * @brief Check if actual token is included in the first tokens of PAR grammar variable
 *
 * @return true - token is first of PAR variable
 * @return false - token is NOT first of PAR variable
 */
bool SyntaxAnalyzer::checkFirstPAR()
{
    return this->token == Alphabet::OPENPAR;
}

/**
 * @brief Check if actual token is included in the first tokens of BLOCK grammar variable
 *
 * @return true - token is first of BLOCK variable
 * @return false - token is NOT first of BLOCK variable
 */
bool SyntaxAnalyzer::checkFirstBLOCK()
{
    return checkFirstCMD() || checkFirstATR() || this->token == Alphabet::OPENBRACE;
}

/**
 * @brief Check if actual token is included in the first tokens of EXP grammar variable
 *
 * @return true - token is first of EXP variable
 * @return false - token is NOT first of EXP variable
 */
bool SyntaxAnalyzer::checkFirstEXP()
{
    return checkFirstT();
}

/**
 * @brief Check if actual token is included in the first tokens of T grammar variable
 *
 * @return true - token is first of T variable
 * @return false - token is NOT first of T variable
 */
bool SyntaxAnalyzer::checkFirstT()
{
    return checkFirstR();
}

/**
 * @brief Check if actual token is included in the first tokens of R grammar variable
 *
 * @return true - token is first of R variable
 * @return false - token is NOT first of R variable
 */
bool SyntaxAnalyzer::checkFirstR()
{
    return checkFirstM();
}

/**
 * @brief Check if actual token is included in the first tokens of M grammar variable
 *
 * @return true - token is first of M variable
 * @return false - token is NOT first of M variable
 */
bool SyntaxAnalyzer::checkFirstM()
{
    return this->token == Alphabet::NOT ||
           this->token == Alphabet::INT ||
           this->token == Alphabet::FLOAT ||
           this->token == Alphabet::ID ||
           this->token == Alphabet::CONSTANT ||
           this->token == Alphabet::OPENPAR ||
           this->token == Alphabet::TRUE ||
           this->token == Alphabet::FALSE;
}

/**
 * @brief Variable S of the L Language Grammar
 */
void SyntaxAnalyzer::S()
{
    while (checkFirstDEC() || checkFirstCMD())
    {
        if (checkFirstDEC()) // DEC ;
        {
            DEC();
            matchToken(Alphabet::SEMICOLON);
        }
        else if (checkFirstCMD()) // CMD
        {
            CMD();
        }
    }

    matchToken(EOF);
}

/**
 * @brief Variable DEC of the L Language Grammar
 */
void SyntaxAnalyzer::DEC()
{
    // Variable type for Semantic Action 2
    int type = -1;

    if (this->token == Alphabet::INT ||
        this->token == Alphabet::FLOAT ||
        this->token == Alphabet::STRING ||
        this->token == Alphabet::BOOLEAN ||
        this->token == Alphabet::CHAR)
    {
        if (this->token == Alphabet::INT) // INT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::INT);
            // Semantic Action 2 (INT)
            type = ConstType::INT;
        }
        else if (this->token == Alphabet::FLOAT) // FLOAT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::FLOAT);
            // Semantic Action 2 (FLOAT)
            type = ConstType::FLOAT;
        }
        else if (this->token == Alphabet::STRING) // STRING ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::STRING);
            // Semantic Action 2 (STRING)
            type = ConstType::STRING;
        }
        else if (this->token == Alphabet::BOOLEAN) // BOOLEAN ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::BOOLEAN);
            // Semantic Action 2 (BOOLEAN)
            type = ConstType::BOOLEAN;
        }
        else if (this->token == Alphabet::CHAR) // CHAR ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::CHAR);
            // Semantic Action 2 (CHAR)
            type = ConstType::CHAR;
        }

                        
        // Semantic Action 1
        if (symbolTable->getType(this->regLex.lexeme) == null)
        {
            symbolTable->setType(this->regLex.lexeme, type);
            symbolTable->setClass(this->regLex.lexeme, ClassType::VAR);
        }
        else
        {
            throwDeclaredID(this->regLex.lexeme);
        }

        matchToken(Alphabet::ID);

        if (this->token == Alphabet::ATRIB)
        {
            matchToken(Alphabet::ATRIB);
            int constType = DECONST();

            // Semantic Action 8
            if (type == ConstType::INT && constType != ConstType::INT)
                throwIncompatibleType();
            if (type == ConstType::FLOAT && (constType != ConstType::INT || constType != ConstType::FLOAT))
                throwIncompatibleType();
            if (type == ConstType::STRING && constType != ConstType::STRING)
                throwIncompatibleType();
            if (type == ConstType::BOOLEAN && constType != ConstType::BOOLEAN)
                throwIncompatibleType();
            if (type == ConstType::CHAR && constType != ConstType::CHAR)
                throwIncompatibleType();
        }

        while (this->token == Alphabet::COMMA)
        {
            matchToken(Alphabet::COMMA);

            // Semantic Action 1
            if (symbolTable->getType(this->regLex.lexeme) == null)
            {
                symbolTable->setType(this->regLex.lexeme, type);
                symbolTable->setClass(this->regLex.lexeme, ClassType::VAR);
            }
            else
            {
                throwDeclaredID(this->regLex.lexeme);
            }

            matchToken(Alphabet::ID);
            if (this->token == Alphabet::ATRIB)
            {
                matchToken(Alphabet::ATRIB);
                int constType = DECONST();

                // Semantic Action 8
                if (type == ConstType::INT && constType != ConstType::INT)
                    throwIncompatibleType();
                if (type == ConstType::FLOAT && (constType != ConstType::INT || constType != ConstType::FLOAT))
                    throwIncompatibleType();
                if (type == ConstType::STRING && constType != ConstType::STRING)
                    throwIncompatibleType();
                if (type == ConstType::BOOLEAN && constType != ConstType::BOOLEAN)
                    throwIncompatibleType();
                if (type == ConstType::CHAR && constType != ConstType::CHAR)
                    throwIncompatibleType();
            }
        }
    }
    else if (this->token == Alphabet::CONST) // CONST ID = [-]CONSTANT
    {
        matchToken(Alphabet::CONST);

        string IDLex = regLex.lexeme;

        // Semantic Action 7
        if (symbolTable->getType(regLex.lexeme) != null)
        {
            throwDeclaredID(regLex.lexeme);
        }

        matchToken(Alphabet::ID);
        matchToken(Alphabet::EQUAL);

        // Semantic Action 5
        int constType = DECONST();

        if (symbolTable->getType(IDLex) == null)
        {
            symbolTable->setType(IDLex, constType);
            symbolTable->setClass(IDLex, ClassType::CONST);
        }
    }
    else
    {
        ATR();
    }
}

/**
 * @brief Variable ATR of the L Language Grammar
 */
void SyntaxAnalyzer::ATR()
{
    int expType;
    
    // Semantic Action 3
    if (symbolTable->getType(regLex.lexeme) == null)
    {
        throwNotDeclaredID(regLex.lexeme);
    }

    if (symbolTable->getClass(regLex.lexeme) == ClassType::CONST)
    {
        throwIncompatibleClass(regLex.lexeme);
    }

    int typeID = symbolTable->getType(regLex.lexeme);

    matchToken(Alphabet::ID);

    if (this->token == Alphabet::OPENBRACKET)
    {
        matchToken(Alphabet::OPENBRACKET);
        
        // Semantic Action 9
        expType = EXP();

        cout << expType << endl;

        if(expType != ConstType::INT){
            throwIncompatibleType();
        }

        matchToken(Alphabet::CLOSEBRACKET);
    }

    matchToken(Alphabet::ATRIB);
    expType = EXP();

    // Semantic Action 10
    if(typeID == ConstType::INT && expType != ConstType::INT)
    {
        //Incompatible type
        throwIncompatibleType();
    }
    if(typeID == ConstType::FLOAT && (expType != ConstType::FLOAT && expType != ConstType::INT))
    {
        //Incompatible type
        throwIncompatibleType();
    }
    if(typeID == ConstType::STRING && expType != ConstType::STRING)
    {
        //Incompatible type
        throwIncompatibleType();
    }
    if(typeID == ConstType::BOOLEAN && expType != ConstType::BOOLEAN)
    {
        //Incompatible type
        throwIncompatibleType();
    }
    if(typeID == ConstType::CHAR && expType != ConstType::CHAR)
    {
        //Incompatible type
        throwIncompatibleType();
    }

}

/**
 * @brief Variable DECONST of the L Language Grammar
 */
int SyntaxAnalyzer::DECONST()
{
    int constType = null;

    if (this->token == Alphabet::MINNUS) // - CONSTANT
    {
        matchToken(Alphabet::MINNUS);
    }
    if (this->token == Alphabet::CONSTANT)
    {
        // Semantic Action 4
        constType = this->regLex.constType;
        matchToken(Alphabet::CONSTANT); // CONSTANT
    }
    else if (this->token == Alphabet::TRUE)
    {
        matchToken(Alphabet::TRUE); // CONSTANT
    }
    else
    {
        matchToken(Alphabet::FALSE); // CONSTANT
    }

    // Semantic Action 4
    return constType;
}

/**
 * @brief Variable CMD of the L Language Grammar
 */
void SyntaxAnalyzer::CMD() // Language commands
{
    if (this->token == Alphabet::WHILE) // WHILE(){}
    {
        matchToken(Alphabet::WHILE);
        PAR();
        BLOCK();
    }
    else if (this->token == Alphabet::IF) // IF() [ELSE]
    {
        matchToken(Alphabet::IF);
        PAR();
        BLOCK();
        if (this->token == Alphabet::ELSE)
        {
            matchToken(Alphabet::ELSE);
            BLOCK();
        }
    }
    else if (this->token == Alphabet::SEMICOLON) // ;
    {
        matchToken(Alphabet::SEMICOLON);
    }
    else if (this->token == Alphabet::READLN) // READLN(ID)
    {
        matchToken(Alphabet::READLN);
        matchToken(Alphabet::OPENPAR);

        // Semantic Action 6
        if (symbolTable->getType(regLex.lexeme) == null)
        {
            throwNotDeclaredID(regLex.lexeme);
        }

        matchToken(Alphabet::ID);
        matchToken(Alphabet::CLOSEPAR);
        matchToken(Alphabet::SEMICOLON);
    }
    else if (this->token == Alphabet::WRITE || this->token == Alphabet::WRITELN) // (WRITE | WRITELN)(EXP {, EXP}*)
    {
        if (this->token == Alphabet::WRITE) // WRITE(EXP {, EXP}*)
        {
            matchToken(Alphabet::WRITE);
        }
        else if (this->token == Alphabet::WRITELN) // WRITELN(EXP {, EXP}*)
        {
            matchToken(Alphabet::WRITELN);
        }

        matchToken(Alphabet::OPENPAR);
        EXP();

        while (this->token == Alphabet::COMMA)
        {
            matchToken(Alphabet::COMMA);
            EXP();
        };

        matchToken(Alphabet::CLOSEPAR);
        matchToken(Alphabet::SEMICOLON);
    }
}

/**
 * @brief Variable PAR of the L Language Grammar
 */
void SyntaxAnalyzer::PAR()
{
    matchToken(Alphabet::OPENPAR);
    EXP();
    matchToken(Alphabet::CLOSEPAR);
}

/**
 * @brief Variable BLOCK of the L Language Grammar
 */
void SyntaxAnalyzer::BLOCK()
{
    if (checkFirstCMD())
    {
        CMD();
    }
    else if (checkFirstATR())
    {
        ATR();
        matchToken(Alphabet::SEMICOLON);
    }
    else if (this->token == Alphabet::OPENBRACE)
    {
        matchToken(Alphabet::OPENBRACE);

        while (checkFirstCMD() || checkFirstATR())
        {
            if (checkFirstCMD())
            {
                CMD();
            }
            else if (checkFirstATR())
            {
                ATR();
                matchToken(Alphabet::SEMICOLON);
            }
        };

        matchToken(Alphabet::CLOSEBRACE);
    }
}

/**
 * @brief Variable EXP of the L Language Grammar
 */
int SyntaxAnalyzer::EXP()
{
    int constType = ConstType::FLOAT;
    
    T();
    while (this->token == Alphabet::EQUAL ||
           this->token == Alphabet::NOTEQUAL ||
           this->token == Alphabet::LESSTHAN ||
           this->token == Alphabet::GREATERTHAN ||
           this->token == Alphabet::LESSEQUAL ||
           this->token == Alphabet::GREATEREQUAL)
    {
        if (this->token == Alphabet::EQUAL)
        {
            matchToken(Alphabet::EQUAL);
        }
        else if (this->token == Alphabet::NOTEQUAL)
        {
            matchToken(Alphabet::NOTEQUAL);
        }
        else if (this->token == Alphabet::LESSTHAN)
        {
            matchToken(Alphabet::LESSTHAN);
        }
        else if (this->token == Alphabet::GREATERTHAN)
        {
            matchToken(Alphabet::GREATERTHAN);
        }
        else if (this->token == Alphabet::LESSEQUAL)
        {
            matchToken(Alphabet::LESSEQUAL);
        }
        else if (this->token == Alphabet::GREATEREQUAL)
        {
            matchToken(Alphabet::GREATEREQUAL);
        }
        T();
    }

    return constType;
}

/**
 * @brief Variable T of the L Language Grammar
 */
void SyntaxAnalyzer::T()
{

    if (this->token == Alphabet::PLUS)
    {
        matchToken(Alphabet::PLUS);
    }
    else if (this->token == Alphabet::MINNUS)
    {
        matchToken(Alphabet::MINNUS);
    }

    R();

    while (this->token == Alphabet::PLUS || this->token == Alphabet::MINNUS || this->token == Alphabet::OR)
    {
        if (this->token == Alphabet::PLUS)
        {
            matchToken(Alphabet::PLUS);
        }
        else if (this->token == Alphabet::MINNUS)
        {
            matchToken(Alphabet::MINNUS);
        }
        else if (this->token == Alphabet::OR)
        {
            matchToken(Alphabet::OR);
        }
        R();
    }
}

/**
 * @brief Variable R of the L Language Grammar
 */
void SyntaxAnalyzer::R()
{
    M();

    while (this->token == Alphabet::TIMES ||
           this->token == Alphabet::DIV ||
           this->token == Alphabet::DIVIDE ||
           this->token == Alphabet::AND ||
           this->token == Alphabet::MOD)
    {
        if (this->token == Alphabet::TIMES)
        {
            matchToken(Alphabet::TIMES);
        }
        else if (this->token == Alphabet::DIV)
        {
            matchToken(Alphabet::DIV);
        }
        else if (this->token == Alphabet::DIVIDE)
        {
            matchToken(Alphabet::DIVIDE);
        }
        else if (this->token == Alphabet::AND)
        {
            matchToken(Alphabet::AND);
        }
        else if (this->token == Alphabet::MOD)
        {
            matchToken(Alphabet::MOD);
        }
        M();
    }
}

/**
 * @brief Variable M of the L Language Grammar
 */
void SyntaxAnalyzer::M()
{
    if (this->token == Alphabet::NOT)
    {
        matchToken(Alphabet::NOT);
        M();
    }
    else if (this->token == Alphabet::INT || this->token == Alphabet::FLOAT)
    {
        if (this->token == Alphabet::INT)
        {
            matchToken(Alphabet::INT);
        }
        else if (this->token == Alphabet::FLOAT)
        {
            matchToken(Alphabet::FLOAT);
        }
        matchToken(Alphabet::OPENPAR);
        EXP();
        matchToken(Alphabet::CLOSEPAR);
    }
    else if (this->token == Alphabet::ID)
    {
        // Semantic Action 3
        if (symbolTable->getType(regLex.lexeme) == null)
        {
            throwNotDeclaredID(regLex.lexeme);
        }
        if (!(symbolTable->getClass(regLex.lexeme) == ConstType::NOT_CONSTANT))
        {
            throwIncompatibleClass(regLex.lexeme);
        }

        matchToken(Alphabet::ID);

        if (this->token == Alphabet::OPENBRACKET)
        {
            matchToken(Alphabet::OPENBRACKET);
            EXP();
            matchToken(Alphabet::CLOSEBRACKET);
        }
    }
    else if (this->token == Alphabet::CONSTANT)
    {
        matchToken(Alphabet::CONSTANT);
    }
    else if (this->token == Alphabet::TRUE)
    {
        matchToken(Alphabet::TRUE);
    }
    else if (this->token == Alphabet::FALSE)
    {
        matchToken(Alphabet::FALSE);
    }
    else
    {
        matchToken(Alphabet::OPENPAR);
        EXP();
        matchToken(Alphabet::CLOSEPAR);
    }
}

/**
 * @brief Initial Syntax Analyzer call
 */
void SyntaxAnalyzer::parser()
{
    // Call the Lexical Analyzer to get first token
    this->regLex = lexicalAnalyzer();
    // printLexicalRegister(this->regLex);
    this->token = this->regLex.token;

    S();
}
