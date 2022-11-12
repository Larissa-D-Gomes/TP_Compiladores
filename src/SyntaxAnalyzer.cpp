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
#include "headers/CodeGeneratorUtils.hpp"
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

    cout << "AnalisadorLexico: [" << tokenToString(this->token) << "] Esperado na Gramatica: [" << tokenToString(expectedToken) << "]" << endl;

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
    int type = null;
    ExpressionReturn deconstRet;

    if (this->token == Alphabet::INT ||
        this->token == Alphabet::FLOAT ||
        this->token == Alphabet::STRING ||
        this->token == Alphabet::BOOLEAN ||
        this->token == Alphabet::CHAR)
    {
        int size;
        if (this->token == Alphabet::INT) // INT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 4;
            matchToken(Alphabet::INT);
            // Semantic Action 2 (INT)
            type = ConstType::INT;

            assemblyDec += "\tresd 1\t\t\t; Declaracao Var Inteiro em [" + to_string(nextFreePosition) + "]\n";

        }
        else if (this->token == Alphabet::FLOAT) // FLOAT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 4;
            matchToken(Alphabet::FLOAT);
            // Semantic Action 2 (FLOAT)
            type = ConstType::FLOAT;

            assemblyDec += "\tresd 1\t\t\t; Declaracao Var Float em [" + to_string(nextFreePosition) + "]\n";

        }
        else if (this->token == Alphabet::STRING) // STRING ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 256;
            matchToken(Alphabet::STRING);
            // Semantic Action 2 (STRING)
            type = ConstType::STRING;

            assemblyDec += "\tresb 100h\t\t\t; Declaracao Var String em [" + to_string(nextFreePosition) + "]\n";
        }
        else if (this->token == Alphabet::BOOLEAN) // BOOLEAN ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 4;
            matchToken(Alphabet::BOOLEAN);
            // Semantic Action 2 (BOOLEAN)
            type = ConstType::BOOLEAN;

             assemblyDec += "\tresb 1\t\t\t; Declaracao Var Boolean em [" + to_string(nextFreePosition) + "]\n";
        }
        else if (this->token == Alphabet::CHAR) // CHAR ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 1;
            matchToken(Alphabet::CHAR);
            // Semantic Action 2 (CHAR)
            type = ConstType::CHAR;

            assemblyDec += "\tresb 1\t\t\t; Declaracao Var Character em [" + to_string(nextFreePosition) + "]\n";
        }

        // Semantic Action 1
        if (symbolTable->getType(this->regLex.lexeme) == null)
        {
            symbolTable->setType(this->regLex.lexeme, type);
            symbolTable->setClass(this->regLex.lexeme, ClassType::VAR);
        }
        else
        {
            cout << "(1.1)" << endl;
            throwDeclaredID(this->regLex.lexeme);
        }

        symbolTable->setAddr(this->regLex.lexeme, nextFreePosition);
        memoryAlocation(size);

        matchToken(Alphabet::ID);

        if (this->token == Alphabet::ATRIB)
        {
            matchToken(Alphabet::ATRIB);
            deconstRet = DECONST(false);

            // Semantic Action 8
            if (type == ConstType::INT && deconstRet.type != ConstType::INT)
            {
                cout << "(8.1-1)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::FLOAT && (deconstRet.type != ConstType::INT && deconstRet.type != ConstType::FLOAT))
            {
                cout << "(8.1-2)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::STRING && deconstRet.type != ConstType::STRING)
            {
                cout << "(8.1-3)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::BOOLEAN && deconstRet.type != ConstType::BOOLEAN)
            {
                cout << "(8.1-4)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::CHAR && deconstRet.type != ConstType::CHAR)
            {
                cout << "(8.1-5)" << endl;
                throwIncompatibleType();
            }
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
                deconstRet = DECONST(false);

                // Semantic Action 8
                if (type == ConstType::INT && deconstRet.type != ConstType::INT)
                {
                    cout << "(8-1)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::FLOAT && (deconstRet.type != ConstType::INT || deconstRet.type != ConstType::FLOAT))
                {
                    cout << "(8-2)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::STRING && deconstRet.type != ConstType::STRING)
                {
                    cout << "(8-3)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::BOOLEAN && deconstRet.type != ConstType::BOOLEAN)
                {
                    cout << "(8-4)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::CHAR && deconstRet.type != ConstType::CHAR)
                {
                    cout << "(8-5)" << endl;
                    throwIncompatibleType();
                }
            }
        }
    }
    else if (this->token == Alphabet::CONST) // CONST ID = [-]CONSTANT
    {
        matchToken(Alphabet::CONST);

        string IDLex = regLex.lexeme;

        // Semantic Action 7
        if (symbolTable->getType(IDLex) != null)
        {
            throwDeclaredID(IDLex);
        }

        matchToken(Alphabet::ID);
        matchToken(Alphabet::EQUAL);

        // Semantic Action 5
        deconstRet = DECONST(true);

        if (symbolTable->getType(IDLex) == null)
        {
            symbolTable->setType(IDLex, deconstRet.type);
            symbolTable->setAddr(IDLex, deconstRet.addr);
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
    ExpressionReturn expRet;
    int typeID = null;
    bool isStringPos = false;

    // Semantic Action 3
    if (symbolTable->getType(regLex.lexeme) == null)
    {
        cout << "(3.1)" << endl;
        throwNotDeclaredID(regLex.lexeme);
    }

    if (symbolTable->getClass(regLex.lexeme) == ClassType::CONST)
    {
        cout << "(3.1)" << endl;
        throwIncompatibleClass(regLex.lexeme);
    }

    typeID = symbolTable->getType(regLex.lexeme);

    matchToken(Alphabet::ID);

    if (this->token == Alphabet::OPENBRACKET)
    {
        // Semantic Action 31
        if (typeID != ConstType::STRING)
        {
            cout << "(31)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::OPENBRACKET);

        // Semantic Action 9
        expRet = EXP();

        if (expRet.type != ConstType::INT)
        {
            cout << "(9)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::CLOSEBRACKET);

        // Semantic Action 32
        isStringPos = true;
    }

    matchToken(Alphabet::ATRIB);
    expRet = EXP();

    // Semantic Action 33
    if (isStringPos && expRet.type != ConstType::CHAR)
    {
        cout << "(33)" << endl;
        throwIncompatibleType();
    }

    // Semantic Action 10
    if (typeID == ConstType::INT && expRet.type != ConstType::INT)
    {
        // Incompatible type
        cout << "(10-1)" << endl;
        throwIncompatibleType();
    }
    if (typeID == ConstType::FLOAT && (expRet.type  != ConstType::FLOAT && expRet.type  != ConstType::INT))
    {
        // Incompatible type
        cout << "(10-2)" << endl;
        throwIncompatibleType();
    }
    if (!isStringPos && typeID == ConstType::STRING && expRet.type != ConstType::STRING)
    {
        // Incompatible type
        cout << "(10-3)" << endl;
        throwIncompatibleType();
    }
    if (typeID == ConstType::BOOLEAN && expRet.type != ConstType::BOOLEAN)
    {
        // Incompatible type
        cout << "(10-4)" << endl;
        throwIncompatibleType();
    }
    if (typeID == ConstType::CHAR && expRet.type != ConstType::CHAR)
    {
        // Incompatible type
        cout << "(10-5)" << endl;
        throwIncompatibleType();
    }
}

/**
 * @brief Variable DECONST of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::DECONST(bool isNewConst)
{
    ExpressionReturn deconstRet;

    bool hasMinnus = false;

    if (this->token == Alphabet::MINNUS) // - CONSTANT
    {
        // Semantic Action 29
        hasMinnus = true;
        matchToken(Alphabet::MINNUS);
    }
    if (this->token == Alphabet::CONSTANT)
    {
        // Semantic Action 4
        deconstRet.type = this->regLex.constType;

        if(isNewConst)
        {
            deconstRet.addr = getCodeDeconst(hasMinnus, deconstRet.type, this->regLex.lexeme);
        }
        matchToken(Alphabet::CONSTANT); // CONSTANT

        
    }
    /*else if (this->token == Alphabet::TRUE)
    {
        deconstRet.type = this->regLex.constType;
        deconstRet.addr = getCodeDeconst(hasMinnus, deconstRet.type, this->regLex.lexeme);
        matchToken(Alphabet::TRUE); // CONSTANT
    }
    else
    {
        deconstRet.type = this->regLex.constType;
        deconstRet.addr = getCodeDeconst(hasMinnus, deconstRet.type, this->regLex.lexeme);
        matchToken(Alphabet::FALSE); // CONSTANT
    }*/

    // Semantic Action 30
    if (hasMinnus && (deconstRet.type != ConstType::INT && deconstRet.type != ConstType::FLOAT))
    {
        cout << "(30)" << endl;
        throwIncompatibleType();
    }
    
    // Semantic Action 4
    return deconstRet;
}

/**
 * @brief Variable CMD of the L Language Grammar
 */
void SyntaxAnalyzer::CMD() // Language commands
{
    ExpressionReturn parRet;

    if (this->token == Alphabet::WHILE) // WHILE(){}
    {
        matchToken(Alphabet::WHILE);
        parRet = PAR();

        // Semantic Action 28
        if (parRet.type != ConstType::BOOLEAN)
        {
            cout << "(28-1)" << endl;
            throwIncompatibleType();
        }

        BLOCK();
    }
    else if (this->token == Alphabet::IF) // IF() [ELSE]
    {
        matchToken(Alphabet::IF);
        parRet = PAR();

        // Semantic Action 28
        if (parRet.type != ConstType::BOOLEAN)
        {
            cout << "(28-2)" << endl;
            throwIncompatibleType();
        }

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
            cout << "(6)" << endl;
            throwNotDeclaredID(regLex.lexeme);
        }

        // Semantic Action 39
        if (symbolTable->getType(regLex.lexeme) == ConstType::BOOLEAN){
            cout << "(39)" << endl;
            throwIncompatibleType();
        }

        // Semantic Action 40
        if (symbolTable->getClass(regLex.lexeme) == ClassType::CONST){
            cout << "(40)" << endl;
            throwIncompatibleClass(regLex.lexeme);
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
ExpressionReturn SyntaxAnalyzer::PAR()
{
    ExpressionReturn parRet;

    matchToken(Alphabet::OPENPAR);

    // Semantic Action 27
    parRet = EXP();
    matchToken(Alphabet::CLOSEPAR);

    return parRet;
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
 * @brief Verify if type conversation is accepted
 *
 * @param tType
 * @param t1Type
 * @param operation
 */
void SyntaxAnalyzer::verifyTypesForT(int tType, int t1Type, int operation)
{
    if (tType == ConstType::CHAR && t1Type != ConstType::CHAR)
    {
        cout << "(26-1)" << endl;
        throwIncompatibleType();
    }
    else if (tType != ConstType::CHAR && t1Type == ConstType::CHAR)
    {
        cout << "(26-2)" << endl;
        throwIncompatibleType();
    }
    else if (tType == ConstType::INT && (t1Type != ConstType::INT && t1Type != ConstType::FLOAT))
    {
        cout << "(26-3)" << endl;
        throwIncompatibleType();
    }
    else if ((tType != ConstType::INT && tType != ConstType::FLOAT) && t1Type == ConstType::INT)
    {
        cout << "(26-4)" << endl;
        throwIncompatibleType();
    }
    else if (tType == ConstType::FLOAT && (t1Type != ConstType::INT && t1Type != ConstType::FLOAT))
    {
        cout << "(26-5)" << endl;
        throwIncompatibleType();
    }
    else if ((tType != ConstType::INT && tType != ConstType::FLOAT) && t1Type == ConstType::FLOAT)
    {
        cout << "(26-6)" << endl;
        throwIncompatibleType();
    }
    else if ((tType == ConstType::STRING || t1Type == ConstType::STRING) && operation != Alphabet::EQUAL)
    {
        cout << "(26-7)" << endl;
        throwIncompatibleType();
    }
    else if (tType == ConstType::STRING && t1Type != ConstType::STRING)
    {
        cout << "(26-8)" << endl;
        throwIncompatibleType();
    }
    else if (tType != ConstType::STRING && t1Type == ConstType::STRING)
    {
        cout << "(26-9)" << endl;
        throwIncompatibleType();
    }
    else if (tType == ConstType::BOOLEAN || t1Type == ConstType::BOOLEAN)
    {
        cout << "(26-10)" << endl;
        throwIncompatibleType();
    }
}

/**
 * @brief Variable EXP of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::EXP()
{
    resetTempCounter();
    ExpressionReturn expRet, tRet, t1Ret;
    int operation = null;

    tRet = T();

    // Semantic Action 37
    expRet.type = tRet.type;

    while (this->token == Alphabet::EQUAL ||
           this->token == Alphabet::NOTEQUAL ||
           this->token == Alphabet::LESSTHAN ||
           this->token == Alphabet::GREATERTHAN ||
           this->token == Alphabet::LESSEQUAL ||
           this->token == Alphabet::GREATEREQUAL)
    {
        if (this->token == Alphabet::EQUAL)
        {
            // Semantic Action 25
            operation = Alphabet::EQUAL;
            matchToken(Alphabet::EQUAL);
        }
        else if (this->token == Alphabet::NOTEQUAL)
        {
            // Semantic Action 25
            operation = Alphabet::NOTEQUAL;
            matchToken(Alphabet::NOTEQUAL);
        }
        else if (this->token == Alphabet::LESSTHAN)
        {
            // Semantic Action 25
            operation = Alphabet::LESSTHAN;
            matchToken(Alphabet::LESSTHAN);
        }
        else if (this->token == Alphabet::GREATERTHAN)
        {
            // Semantic Action 25
            operation = Alphabet::GREATERTHAN;
            matchToken(Alphabet::GREATERTHAN);
        }
        else if (this->token == Alphabet::LESSEQUAL)
        {
            // Semantic Action 25
            operation = Alphabet::LESSEQUAL;
            matchToken(Alphabet::LESSEQUAL);
        }
        else if (this->token == Alphabet::GREATEREQUAL)
        {
            // Semantic Action 25
            operation = Alphabet::GREATEREQUAL;
            matchToken(Alphabet::GREATEREQUAL);
        }
        t1Ret = T();

        // Semantic action 26
        verifyTypesForT(tRet.type, t1Ret.type, operation);
        tRet.type = ConstType::BOOLEAN;
    }

    // Semantic Action 14
    expRet.type = tRet.type;

    return expRet;
}

/**
 * @brief Variable T of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::T()
{
    ExpressionReturn tRet, rRet , r1Ret;
    int operation;
    bool isPlusOrMinus = false;

    if (this->token == Alphabet::PLUS)
    {
        // Semantic Action 34
        isPlusOrMinus = true;
        matchToken(Alphabet::PLUS);
    }
    else if (this->token == Alphabet::MINNUS)
    {
        // Semantic Action 34
        isPlusOrMinus = true;
        matchToken(Alphabet::MINNUS);
    }

    rRet = R();
    // Semantic Action 35
    if (isPlusOrMinus && (rRet.type != ConstType::INT && rRet.type != ConstType::FLOAT))
    {
        // ERROR: Incompatible types
        cout << "(35)" << endl;
        throwIncompatibleType();
    }
    while (this->token == Alphabet::PLUS || this->token == Alphabet::MINNUS || this->token == Alphabet::OR)
    {
        if (this->token == Alphabet::PLUS)
        {
            // Semantic Action 23
            operation = Alphabet::PLUS;
            matchToken(Alphabet::PLUS);
        }
        else if (this->token == Alphabet::MINNUS)
        {
            // Semantic Action 23
            operation = Alphabet::MINNUS;
            matchToken(Alphabet::MINNUS);
        }
        else if (this->token == Alphabet::OR)
        {
            // Semantic Action 23
            operation = Alphabet::OR;
            matchToken(Alphabet::OR);
        }

        r1Ret = R();

        // Semantic Action 24
        if (operation == Alphabet::PLUS || operation == Alphabet::MINNUS)
        {
            if ((rRet.type != ConstType::INT && rRet.type != ConstType::FLOAT) || (r1Ret.type != ConstType::INT && r1Ret.type != ConstType::FLOAT))
            {
                cout << "(24-1)" << endl;
                throwIncompatibleType();
            }
            else
            {
                if (rRet.type == ConstType::FLOAT || r1Ret.type == ConstType::FLOAT)
                {
                    tRet.type = ConstType::FLOAT;
                }
                else
                {
                    tRet.type = ConstType::INT;
                }
            }
        }
        else if (operation == Alphabet::OR)
        {
            if (rRet.type != ConstType::BOOLEAN || r1Ret.type != ConstType::BOOLEAN)
            {
                cout << "(24-2)" << endl;
                throwIncompatibleType();
            }
            else
            {
                tRet.type = ConstType::BOOLEAN;
            }
        }
    }
    // Semantic Action 13
    if (tRet.type == null)
        tRet.type = rRet.type;

    return tRet;
}

/**
 * @brief Get type of R variable
 *
 * @param mType
 * @param m1Return
 * @param operation
 * @return int
 */
// Semantic action 22
ExpressionReturn SyntaxAnalyzer::rGetReturn(ExpressionReturn M, ExpressionReturn M1, int operation)
{
    ExpressionReturn rRet;

    if (operation == Alphabet::TIMES)
    {
        // Impossible to execute a times operation with types different
        // of int and float
        if ((M.type != ConstType::INT && M.type != ConstType::FLOAT) ||
            (M1.type != ConstType::INT && M1.type != ConstType::FLOAT))
        {
            cout << "(22-1)" << endl;
            throwIncompatibleType();
        }

        // If times operation contains a float, change the operation
        // type to float
        if (M.type == ConstType::FLOAT || M1.type == ConstType::FLOAT)
        {
            rRet.type = ConstType::FLOAT;

            if(M.type == ConstType::FLOAT && M1.type == ConstType::FLOAT)
            {
                // Metodo para dois floats
            } else if(M.type == ConstType::INT) {
                // Metodo para UM floats m, m1
            } else {
                // Metodo para UM floats m1, m 
            }
        }
        else
        {
            rRet.type = ConstType::INT;
        }
    }
    else if (operation == Alphabet::DIV)
    {
        // Div operation needs two int operators
        if (M.type != ConstType::INT || M1.type != ConstType::INT)
        {
            cout << "(22-2)" << endl;
            throwIncompatibleType();
        }
        else
            rRet.type = ConstType::INT;
    }
    else if (operation == Alphabet::DIVIDE)
    {
        // Impossible to execute a divide operation with types different
        // of int and float
        if ((M.type != ConstType::INT && M1.type != ConstType::FLOAT) ||
            (M1.type != ConstType::INT && M1.type != ConstType::FLOAT))
        {
            cout << "(22-3.0)" << endl;
            throwIncompatibleType();
        }

        // If divide operation contains a float, change the operation
        // type to float
        if (M.type == ConstType::FLOAT || M1.type == ConstType::FLOAT)
            rRet.type = ConstType::FLOAT;
        else
            rRet.type = ConstType::INT;
    }
    else if (operation == Alphabet::AND)
    {
        // Impossible to execute a and operation with types different
        // of boolean
        if (M.type != ConstType::BOOLEAN || M1.type != ConstType::BOOLEAN)
        {
            cout << "(22-3.1)" << endl;
            throwIncompatibleType();
        }
        rRet.type = ConstType::BOOLEAN;
    }
    else if (operation == Alphabet::MOD)
    {
        // Impossible to execute a mod operation with types different
        // of int
        if (M.type != ConstType::INT || M1.type != ConstType::INT)
        {
            cout << "(22-4)" << endl;
            throwIncompatibleType();
        }
        rRet.type = ConstType::INT;
    }
    return rRet;
}

/**
 * @brief Variable R of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::R()
{
    ExpressionReturn rRet, mRet, m1Ret;

    mRet = M();

    // Semantic Action 38
    rRet.type = mRet.type;

    // Synthesizes address from son
    rRet.addr = mRet.addr;

    // Semantic Action 21
    int operation = null;

    while (this->token == Alphabet::TIMES ||
           this->token == Alphabet::DIV ||
           this->token == Alphabet::DIVIDE ||
           this->token == Alphabet::AND ||
           this->token == Alphabet::MOD)
    {
        if (this->token == Alphabet::TIMES)
        {
            // Semantic Action 21
            operation = Alphabet::TIMES;
            matchToken(Alphabet::TIMES);
        }
        else if (this->token == Alphabet::DIV)
        {
            // Semantic Action 21
            operation = Alphabet::DIV;
            matchToken(Alphabet::DIV);
        }
        else if (this->token == Alphabet::DIVIDE)
        {
            // Semantic Action 21
            operation = Alphabet::DIVIDE;
            matchToken(Alphabet::DIVIDE);
        }
        else if (this->token == Alphabet::AND)
        {
            // Semantic Action 21
            operation = Alphabet::AND;
            matchToken(Alphabet::AND);
        }
        else if (this->token == Alphabet::MOD)
        {
            // Semantic Action 21
            operation = Alphabet::MOD;
            matchToken(Alphabet::MOD);
        }

        m1Ret = M();

        // Semantic action 22
        rRet = rGetReturn(mRet, m1Ret, operation);
    }

    // If semantic action 22 has not been executed
    // Semantic action 12
    if (rRet.type == null)
        rRet.type = mRet.type;

    return rRet;
}

/**
 * @brief Variable M of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::M()
{
    ExpressionReturn mRet, m1Ret, expRet;

    int convType = null, lexID = null;

    if (this->token == Alphabet::NOT)
    {
        matchToken(Alphabet::NOT);
        m1Ret = M();

        // Semantic Action 19
        if (m1Ret.type != ConstType::BOOLEAN)
        {
            cout << "(19)" << endl;
            throwIncompatibleType();
        }
        else
            mRet.type = m1Ret.type;

        getCodeNotExp(m1Ret.addr, m1Ret.type);

    }
    else if (this->token == Alphabet::INT || this->token == Alphabet::FLOAT)
    {
        if (this->token == Alphabet::INT)
        {
            matchToken(Alphabet::INT);
            // Semantic Action 15
            convType = ConstType::INT;
        }
        else if (this->token == Alphabet::FLOAT)
        {
            matchToken(Alphabet::FLOAT);
            // Semantic Action 16
            convType = ConstType::FLOAT;
        }
        matchToken(Alphabet::OPENPAR);
        expRet = EXP();

        // Semantic Action 17
        if (expRet.type != ConstType::INT && expRet.type != ConstType::FLOAT)
        {
            cout << "(17)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::CLOSEPAR);

        // Semantic Action 18
        if (convType == ConstType::INT)
            mRet.type = ConstType::INT;
        else if (convType == ConstType::FLOAT)
            mRet.type = ConstType::FLOAT;
    }
    else if (this->token == Alphabet::ID)
    {
        lexID = symbolTable->getType(regLex.lexeme);
        mRet.addr = symbolTable->getAddr(regLex.lexeme);

        // Semantic Action 3
        if (lexID == null)
        {
            cout << "(3.2)" << endl;
            throwNotDeclaredID(regLex.lexeme);
        }

        matchToken(Alphabet::ID);

        if (this->token == Alphabet::OPENBRACKET)
        {
            // Semantic Action 31
            if (lexID != ConstType::STRING)
            {
                cout << "(31)" << endl;
                throwIncompatibleType();
            }

            matchToken(Alphabet::OPENBRACKET);

            // Semantic Action 9
            expRet = EXP();

            if (expRet.type != ConstType::INT)
            {
                cout << "(9)" << endl;
                throwIncompatibleType();
            }

            matchToken(Alphabet::CLOSEBRACKET);
        }

        // Semantic Action 20
        mRet.type = lexID;
    }
    else if (this->token == Alphabet::CONSTANT)
    {
        // Semantic Action 11
        mRet.type = regLex.constType;

        mRet.addr = getCodeExpConst(regLex.lexeme, regLex.constType);
        mRet.type = regLex.constType;

        matchToken(Alphabet::CONSTANT);
    }
    // else if (this->token == Alphabet::TRUE)
    // {
    //     // Semantic Action 11
    //     mRet.type = regLex.constType;
    //     matchToken(Alphabet::TRUE);
    // }
    // else if (this->token == Alphabet::FALSE)
    // {
    //     // Semantic Action 11
    //     mRet.type = regLex.constType;
    //     matchToken(Alphabet::FALSE);
    // }
    else
    {
        matchToken(Alphabet::OPENPAR);
        // Semantic Action 36
        mRet = EXP();

        matchToken(Alphabet::CLOSEPAR);
    }

    return mRet;
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

    assembly += "section .data                 ; Sessão de dados\n";
    assembly += "M:                            ; Rotulo para demarcar o\n";
    assembly += "                              ; inicio da sessao de dados\n";
    assembly += "\tresb 0x10000              ; Reserva de temporarios\n";
    assembly += "   ; ***Definicoes de variaveis e constantes\n";

    S();

    assembly += assemblyDec;

    assembly += "section .text                 ; Sessao de codigo\n";
    assembly += "global _start                 ; Ponto inicial do programa\n";
    assembly += "_start:                       ; Inicio do programa\n";
    assembly += "   ; ***Comandos\n";
    assembly += assemblyCmd;
    assembly += "; Halt\n";
    assembly += "mov rax, 60                   ; Chamada de saida\n";
    assembly += "mov rdi, 0                    ; Codigo de saida sem erros\n";
    assembly += "syscall                       ; Chama o kernel\n";
}
