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
    int type = null, constType = null;

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

            assemblyDec += "\tresd 1        ; Declaracao Inteiro em [" + to_string(nextFreePosition) + "]\n";

        }
        else if (this->token == Alphabet::FLOAT) // FLOAT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 4;
            matchToken(Alphabet::FLOAT);
            // Semantic Action 2 (FLOAT)
            type = ConstType::FLOAT;

            assemblyDec += "\tresd 1        ; Declaracao Float em [" + to_string(nextFreePosition) + "]\n";

        }
        else if (this->token == Alphabet::STRING) // STRING ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 256;
            matchToken(Alphabet::STRING);
            // Semantic Action 2 (STRING)
            type = ConstType::STRING;

            assemblyDec += "\tresb 100h        ; Declaracao String em [" + to_string(nextFreePosition) + "]\n";
        }
        else if (this->token == Alphabet::BOOLEAN) // BOOLEAN ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 4;
            matchToken(Alphabet::BOOLEAN);
            // Semantic Action 2 (BOOLEAN)
            type = ConstType::BOOLEAN;

             assemblyDec += "\tresb 1        ; Declaracao Boolean em [" + to_string(nextFreePosition) + "]\n";
        }
        else if (this->token == Alphabet::CHAR) // CHAR ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            size = 1;
            matchToken(Alphabet::CHAR);
            // Semantic Action 2 (CHAR)
            type = ConstType::CHAR;

            assemblyDec += "\tresb 1        ; Declaracao Character em [" + to_string(nextFreePosition) + "]\n";
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
            constType = DECONST();

            // Semantic Action 8
            if (type == ConstType::INT && constType != ConstType::INT)
            {
                cout << "(8.1-1)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::FLOAT && (constType != ConstType::INT && constType != ConstType::FLOAT))
            {
                cout << "(8.1-2)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::STRING && constType != ConstType::STRING)
            {
                cout << "(8.1-3)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::BOOLEAN && constType != ConstType::BOOLEAN)
            {
                cout << "(8.1-4)" << endl;
                throwIncompatibleType();
            }
            if (type == ConstType::CHAR && constType != ConstType::CHAR)
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
                constType = DECONST();

                // Semantic Action 8
                if (type == ConstType::INT && constType != ConstType::INT)
                {
                    cout << "(8-1)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::FLOAT && (constType != ConstType::INT || constType != ConstType::FLOAT))
                {
                    cout << "(8-2)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::STRING && constType != ConstType::STRING)
                {
                    cout << "(8-3)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::BOOLEAN && constType != ConstType::BOOLEAN)
                {
                    cout << "(8-4)" << endl;
                    throwIncompatibleType();
                }
                if (type == ConstType::CHAR && constType != ConstType::CHAR)
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
    int expType = null, typeID = null;
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
        expType = EXP();

        if (expType != ConstType::INT)
        {
            cout << "(9)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::CLOSEBRACKET);

        // Semantic Action 32
        isStringPos = true;
    }

    matchToken(Alphabet::ATRIB);
    expType = EXP();

    // Semantic Action 33
    if (isStringPos && expType != ConstType::CHAR)
    {
        cout << "(33)" << endl;
        throwIncompatibleType();
    }

    // Semantic Action 10
    if (typeID == ConstType::INT && expType != ConstType::INT)
    {
        // Incompatible type
        cout << "(10-1)" << endl;
        throwIncompatibleType();
    }
    if (typeID == ConstType::FLOAT && (expType != ConstType::FLOAT && expType != ConstType::INT))
    {
        // Incompatible type
        cout << "(10-2)" << endl;
        throwIncompatibleType();
    }
    if (!isStringPos && typeID == ConstType::STRING && expType != ConstType::STRING)
    {
        // Incompatible type
        cout << "(10-3)" << endl;
        throwIncompatibleType();
    }
    if (typeID == ConstType::BOOLEAN && expType != ConstType::BOOLEAN)
    {
        // Incompatible type
        cout << "(10-4)" << endl;
        throwIncompatibleType();
    }
    if (typeID == ConstType::CHAR && expType != ConstType::CHAR)
    {
        // Incompatible type
        cout << "(10-5)" << endl;
        throwIncompatibleType();
    }
}

/**
 * @brief Variable DECONST of the L Language Grammar
 */
int SyntaxAnalyzer::DECONST()
{
    int constType = null;
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

    // Semantic Action 30
    if (hasMinnus && (constType != ConstType::INT && constType != ConstType::FLOAT))
    {
        cout << "(30)" << endl;
        throwIncompatibleType();
    }

    // Semantic Action 4
    return constType;
}

/**
 * @brief Variable CMD of the L Language Grammar
 */
void SyntaxAnalyzer::CMD() // Language commands
{
    int parType = null;

    if (this->token == Alphabet::WHILE) // WHILE(){}
    {
        matchToken(Alphabet::WHILE);
        parType = PAR();

        // Semantic Action 28
        if (parType != ConstType::BOOLEAN)
        {
            cout << "(28-1)" << endl;
            throwIncompatibleType();
        }

        BLOCK();
    }
    else if (this->token == Alphabet::IF) // IF() [ELSE]
    {
        matchToken(Alphabet::IF);
        parType = PAR();

        // Semantic Action 28
        if (parType != ConstType::BOOLEAN)
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
int SyntaxAnalyzer::PAR()
{
    int parType;
    matchToken(Alphabet::OPENPAR);

    // Semantic Action 27
    parType = EXP();
    matchToken(Alphabet::CLOSEPAR);

    return parType;
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
int SyntaxAnalyzer::EXP()
{
    int expType = null, operation = null;

    int tType = T();

    // Semantic Action 37
    expType = tType;

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
        int t1Type = T();

        // Semantic action 26
        verifyTypesForT(tType, t1Type, operation);
        tType = ConstType::BOOLEAN;
    }

    // Semantic Action 14
    expType = tType;

    return expType;
}

/**
 * @brief Variable T of the L Language Grammar
 */
int SyntaxAnalyzer::T()
{
    int tType = null, operation = null, rType = null, r1Type = null;
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

    rType = R();
    // Semantic Action 35
    if (isPlusOrMinus && (rType != ConstType::INT && rType != ConstType::FLOAT))
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

        r1Type = R();

        // Semantic Action 24
        if (operation == Alphabet::PLUS || operation == Alphabet::MINNUS)
        {
            if ((rType != ConstType::INT && rType != ConstType::FLOAT) || (r1Type != ConstType::INT && r1Type != ConstType::FLOAT))
            {
                cout << "(24-1)" << endl;
                throwIncompatibleType();
            }
            else
            {
                if (rType == ConstType::FLOAT || r1Type == ConstType::FLOAT)
                {
                    tType = ConstType::FLOAT;
                }
                else
                {
                    tType = ConstType::INT;
                }
            }
        }
        else if (operation == Alphabet::OR)
        {
            if (rType != ConstType::BOOLEAN || r1Type != ConstType::BOOLEAN)
            {
                cout << "(24-2)" << endl;
                throwIncompatibleType();
            }
            else
            {
                tType = ConstType::BOOLEAN;
            }
        }
    }
    // Semantic Action 13
    if (tType == null)
        tType = rType;

    return tType;
}

/**
 * @brief Get type of R variable
 *
 * @param mType
 * @param m1Type
 * @param operation
 * @return int
 */
// Semantic action 22
int SyntaxAnalyzer::rGetType(int mType, int m1Type, int operation)
{
    int rType = null;

    if (operation == Alphabet::TIMES)
    {
        // Impossible to execute a times operation with types different
        // of int and float
        if ((mType != ConstType::INT && mType != ConstType::FLOAT) ||
            (m1Type != ConstType::INT && m1Type != ConstType::FLOAT))
        {
            cout << "(22-1)" << endl;
            throwIncompatibleType();
        }

        // If times operation contains a float, change the operation
        // type to float
        if (mType == ConstType::FLOAT || m1Type == ConstType::FLOAT)
            rType = ConstType::FLOAT;
        else
            rType = ConstType::INT;
    }
    else if (operation == Alphabet::DIV)
    {
        // Div operation needs two int operators
        if (mType != ConstType::INT || m1Type != ConstType::INT)
        {
            cout << "(22-2)" << endl;
            throwIncompatibleType();
        }
        else
            rType = ConstType::INT;
    }
    else if (operation == Alphabet::DIVIDE)
    {
        // Impossible to execute a divide operation with types different
        // of int and float
        if ((mType != ConstType::INT && mType != ConstType::FLOAT) ||
            (m1Type != ConstType::INT && m1Type != ConstType::FLOAT))
        {
            cout << "(22-3.0)" << endl;
            throwIncompatibleType();
        }

        // If divide operation contains a float, change the operation
        // type to float
        if (mType == ConstType::FLOAT || m1Type == ConstType::FLOAT)
            rType = ConstType::FLOAT;
        else
            rType = ConstType::INT;
    }
    else if (operation == Alphabet::AND)
    {
        // Impossible to execute a and operation with types different
        // of boolean
        if (mType != ConstType::BOOLEAN || m1Type != ConstType::BOOLEAN)
        {
            cout << "(22-3.1)" << endl;
            throwIncompatibleType();
        }
        rType = ConstType::BOOLEAN;
    }
    else if (operation == Alphabet::MOD)
    {
        // Impossible to execute a mod operation with types different
        // of int
        if (mType != ConstType::INT || m1Type != ConstType::INT)
        {
            cout << "(22-4)" << endl;
            throwIncompatibleType();
        }
        rType = ConstType::INT;
    }
    return rType;
}

/**
 * @brief Variable R of the L Language Grammar
 */
int SyntaxAnalyzer::R()
{
    int rType = null, m1Type = null;

    int mType = M();

    // Semantic Action 38
    rType = mType;

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
        m1Type = M();

        // Semantic action 22
        rType = rGetType(mType, m1Type, operation);
    }

    // If semantic action 22 has not been executed
    // Semantic action 12
    if (rType == null)
        rType = mType;

    return rType;
}

/**
 * @brief Variable M of the L Language Grammar
 */
int SyntaxAnalyzer::M()
{
    int mType = null, m1Type = null, convType = null, expType = null, lexID = null;

    if (this->token == Alphabet::NOT)
    {
        matchToken(Alphabet::NOT);
        m1Type = M();

        // Semantic Action 19
        if (m1Type != ConstType::BOOLEAN)
        {
            cout << "(19)" << endl;
            throwIncompatibleType();
        }
        else
            mType = m1Type;
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
        expType = EXP();

        // Semantic Action 17
        if (expType != ConstType::INT && expType != ConstType::FLOAT)
        {
            cout << "(17)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::CLOSEPAR);

        // Semantic Action 18
        if (convType == ConstType::INT)
            mType = ConstType::INT;
        else if (convType == ConstType::FLOAT)
            mType = ConstType::FLOAT;
    }
    else if (this->token == Alphabet::ID)
    {
        lexID = symbolTable->getType(regLex.lexeme);

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
            expType = EXP();

            if (expType != ConstType::INT)
            {
                cout << "(9)" << endl;
                throwIncompatibleType();
            }

            matchToken(Alphabet::CLOSEBRACKET);
        }

        // Semantic Action 20
        mType = lexID;
    }
    else if (this->token == Alphabet::CONSTANT)
    {
        // Semantic Action 11
        mType = regLex.constType;
        matchToken(Alphabet::CONSTANT);
    }
    else if (this->token == Alphabet::TRUE)
    {
        // Semantic Action 11
        mType = regLex.constType;
        matchToken(Alphabet::TRUE);
    }
    else if (this->token == Alphabet::FALSE)
    {
        // Semantic Action 11
        mType = regLex.constType;
        matchToken(Alphabet::FALSE);
    }
    else
    {
        matchToken(Alphabet::OPENPAR);
        // Semantic Action 36
        mType = EXP();

        matchToken(Alphabet::CLOSEPAR);
    }

    return mType;
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
