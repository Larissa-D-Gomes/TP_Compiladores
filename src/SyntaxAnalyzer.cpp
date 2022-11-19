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
#include <list>

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
#define isToUsePeephole true

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
    int type = null; long addrID = null;
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

        addrID = symbolTable->getAddr(this->regLex.lexeme);

        matchToken(Alphabet::ID);

        //Verify if it has atribuition 
        if (this->token == Alphabet::ATRIB)
        {
            matchToken(Alphabet::ATRIB);
            deconstRet = DECONST(true);

            // Semantic Action 8
            if (type == ConstType::INT && deconstRet.type != ConstType::INT)
            {
                cout << "(8.1-1)" << endl;
                throwIncompatibleType();
            }
            // ATRIB INT X INT
            else if (type == ConstType::INT && deconstRet.type == ConstType::INT)
            {
                getCodeAtribIntAndInt(addrID, deconstRet.addr);
            }
            //Throw exception if type ID is diferent of type ID (return)[float or int]
            if (type == ConstType::FLOAT && (deconstRet.type != ConstType::INT && deconstRet.type != ConstType::FLOAT))
            {
                cout << "(8.1-2)" << endl;
                throwIncompatibleType();
            }
            // ATRIB FLOAT X FLOAT
            else if (type == ConstType::FLOAT && deconstRet.type == ConstType::FLOAT)
            {
                getCodeAtribFloatAndFloat(addrID, deconstRet.addr);
            }
            // ATRIB FLOAT X INT
            else if (type == ConstType::FLOAT && deconstRet.type == ConstType::INT)
            {
                getCodeAtribFloatAndInt(addrID, deconstRet.addr);
            }
            //Throw exception if type ID is diferent of type ID (return)[string]
            if (type == ConstType::STRING && deconstRet.type != ConstType::STRING)
            {
                cout << "(8.1-3)" << endl;
                throwIncompatibleType();
            }
            // ATRIB STRING
            else if (type == ConstType::STRING && deconstRet.type == ConstType::STRING)
            {
                getCodeAtribStringAndString(addrID, deconstRet.addr);
            }
            //Throw exception if type ID is diferent of type ID (return) [boolean]
            if (type == ConstType::BOOLEAN && deconstRet.type != ConstType::BOOLEAN)
            {
                cout << "(8.1-4)" << endl;
                throwIncompatibleType();
            }
            // ATRIB BOOLEAN X BOOLEAN
            else if (type == ConstType::BOOLEAN && deconstRet.type == ConstType::BOOLEAN)
            {
                getCodeAtribBooleanAndBoolean(addrID, deconstRet.addr);
            }
            //Throw exception if type ID is diferent of type ID (return) [char]
            if (type == ConstType::CHAR && deconstRet.type != ConstType::CHAR)
            {
                cout << "(8.1-5)" << endl;
                throwIncompatibleType();
            }
            // ATRIB CHAR
            else if (type == ConstType::CHAR && deconstRet.type == ConstType::CHAR)
            {
                getCodeAtribCharAndChar(addrID, deconstRet.addr);
            }
        }
        // Declare new ID's and possible atribuitions after comma
        while (this->token == Alphabet::COMMA)
        {
            matchToken(Alphabet::COMMA);

            if (type == ConstType::INT) // INT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                type = ConstType::INT;

                assemblyDec += "\tresd 1\t\t\t; Declaracao Var Inteiro em [" + to_string(nextFreePosition) + "]\n";
            }
            else if (type == ConstType::FLOAT) // FLOAT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                type = ConstType::FLOAT;

                assemblyDec += "\tresd 1\t\t\t; Declaracao Var Float em [" + to_string(nextFreePosition) + "]\n";
            }
            else if (type == ConstType::STRING) // STRING ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                type = ConstType::STRING;

                assemblyDec += "\tresb 100h\t\t\t; Declaracao Var String em [" + to_string(nextFreePosition) + "]\n";
            }
            else if (type == ConstType::BOOLEAN) // BOOLEAN ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                type = ConstType::BOOLEAN;

                assemblyDec += "\tresb 1\t\t\t; Declaracao Var Boolean em [" + to_string(nextFreePosition) + "]\n";
            }
            else if (type == ConstType::CHAR) // CHAR ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
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
                throwDeclaredID(this->regLex.lexeme);
            }

            symbolTable->setAddr(this->regLex.lexeme, nextFreePosition);
            memoryAlocation(size);

            addrID = symbolTable->getAddr(this->regLex.lexeme);

            matchToken(Alphabet::ID);

            //Verify if it has atribuition 
            if (this->token == Alphabet::ATRIB)
            {
                matchToken(Alphabet::ATRIB);
                deconstRet = DECONST(true);

                // Semantic Action 8
                if (type == ConstType::INT && deconstRet.type != ConstType::INT)
                {
                    cout << "(8-1)" << endl;
                    throwIncompatibleType();
                }
                // ATRIB INT X INT
                else if (type == ConstType::INT && deconstRet.type == ConstType::INT)
                {
                    getCodeAtribIntAndInt(addrID, deconstRet.addr);
                }
                 //Throw exception if type ID is diferent of type ID (return)[float or int]
                if (type == ConstType::FLOAT && (deconstRet.type != ConstType::INT && deconstRet.type != ConstType::FLOAT))
                {
                    cout << "(8-2)" << endl;
                    throwIncompatibleType();
                }
                // ATRIB FLOAT X FLOAT
                else if (type == ConstType::FLOAT && deconstRet.type == ConstType::FLOAT)
                {
                    getCodeAtribFloatAndFloat(addrID, deconstRet.addr);
                }
                // ATRIB FLOAT X INT
                else if (type == ConstType::FLOAT && deconstRet.type == ConstType::INT)
                {
                    getCodeAtribFloatAndInt(addrID, deconstRet.addr);
                }
                //Throw exception if type ID is diferent of type ID (return)[string]
                if (type == ConstType::STRING && deconstRet.type != ConstType::STRING)
                {
                    cout << "(8-3)" << endl;
                    throwIncompatibleType();
                }
                // ATRIB STRING X STRING
                else if (type == ConstType::STRING && deconstRet.type == ConstType::STRING)
                {
                    getCodeAtribStringAndString(addrID, deconstRet.addr);
                }
                //Throw exception if type ID is diferent of type ID (return) [boolean]
                if (type == ConstType::BOOLEAN && deconstRet.type != ConstType::BOOLEAN)
                {
                    cout << "(8-4)" << endl;
                    throwIncompatibleType();
                }
                // ATRIB BOOLEAN X BOOLEAN
                else if (type == ConstType::BOOLEAN && deconstRet.type == ConstType::BOOLEAN)
                {
                    getCodeAtribBooleanAndBoolean(addrID, deconstRet.addr);
                }
                 //Throw exception if type ID is diferent of type ID (return) [char]
                if (type == ConstType::CHAR && deconstRet.type != ConstType::CHAR)
                {
                    cout << "(8-5)" << endl;
                    throwIncompatibleType();
                }
                // ATRIB CHAR
                else if (type == ConstType::CHAR && deconstRet.type == ConstType::CHAR)
                {
                    getCodeAtribCharAndChar(addrID, deconstRet.addr);
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
    ExpressionReturn expRet, exp1Ret;
    int typeID = null, addrID = null;
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
    addrID = symbolTable->getAddr(regLex.lexeme);

    matchToken(Alphabet::ID);
    bool isToResetTemp = true;

    if (this->token == Alphabet::OPENBRACKET)
    {
        isToResetTemp = false;
        // Semantic Action 31
        if (typeID != ConstType::STRING)
        {
            cout << "(31)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::OPENBRACKET);

        // Semantic Action 9
        exp1Ret = EXP(true);

        if (exp1Ret.type != ConstType::INT)
        {
            cout << "(9)" << endl;
            throwIncompatibleType();
        }

        matchToken(Alphabet::CLOSEBRACKET);

        // Semantic Action 32
        isStringPos = true;
    }

    matchToken(Alphabet::ATRIB);
    expRet = EXP(isToResetTemp);

    // Semantic Action 33
    if (isStringPos && expRet.type != ConstType::CHAR)
    {
        cout << "(33)" << endl;
        throwIncompatibleType();
    }
    // ATRIB CHAR
    else if (isStringPos && expRet.type == ConstType::CHAR)
    {
        getCodeAtribStringPos(addrID, exp1Ret.addr, expRet.addr);
    }

    // Semantic Action 10
    if (typeID == ConstType::INT && expRet.type != ConstType::INT)
    {
        // Incompatible type
        cout << "(10-1)" << endl;
        throwIncompatibleType();
    }
    // ATRIB INT X INT
    else if (typeID == ConstType::INT && expRet.type == ConstType::INT)
    {
        getCodeAtribIntAndInt(addrID, expRet.addr);
    }

    if (typeID == ConstType::FLOAT && (expRet.type != ConstType::FLOAT && expRet.type != ConstType::INT))
    {
        // Incompatible type
        cout << "(10-2)" << endl;
        throwIncompatibleType();
    }
    // ATRIB FLOAT X FLOAT
    else if (typeID == ConstType::FLOAT && expRet.type == ConstType::FLOAT)
    {
        getCodeAtribFloatAndFloat(addrID, expRet.addr);
    }
    // ATRIB FLOAT X INT
    else if (typeID == ConstType::FLOAT && expRet.type == ConstType::INT)
    {
        getCodeAtribFloatAndInt(addrID, expRet.addr);
    }

    if (!isStringPos && typeID == ConstType::STRING && expRet.type != ConstType::STRING)
    {
        // Incompatible type
        cout << "(10-3)" << endl;
        throwIncompatibleType();
    }
    // ATRIB STRING
    else if (!isStringPos && typeID == ConstType::STRING && expRet.type == ConstType::STRING)
    {
        getCodeAtribStringAndString(addrID, expRet.addr);
    }

    if (typeID == ConstType::BOOLEAN && expRet.type != ConstType::BOOLEAN)
    {
        // Incompatible type
        cout << "(10-4)" << endl;
        throwIncompatibleType();
    }
    // ATRIB BOOLEAN X BOOLEAN
    else if (typeID == ConstType::BOOLEAN && expRet.type == ConstType::BOOLEAN)
    {
        getCodeAtribBooleanAndBoolean(addrID, expRet.addr);
    }

    if (typeID == ConstType::CHAR && expRet.type != ConstType::CHAR)
    {
        // Incompatible type
        cout << "(10-5)" << endl;
        throwIncompatibleType();
    }
    // ATRIB CHAR
    else if (typeID == ConstType::CHAR && expRet.type == ConstType::CHAR)
    {
        getCodeAtribCharAndChar(addrID, expRet.addr);
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

        if (isNewConst)
        {
            deconstRet.addr = getCodeExpConst(this->regLex.lexeme, deconstRet.type);
        }
        matchToken(Alphabet::CONSTANT); // CONSTANT
    }
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
    ExpressionReturn parRet, expRet;
    bool isWriteLn = false;

    if (this->token == Alphabet::WHILE) // WHILE(){}
    {
        matchToken(Alphabet::WHILE);
        parRet = PAR();

        string labelFalse;
        string labelLoop;

        // Code open while      
        getCodeOpenWhile(parRet.addr, labelFalse, labelLoop);

        // Semantic Action 28
        if (parRet.type != ConstType::BOOLEAN)
        {
            cout << "(28-1)" << endl;
            throwIncompatibleType();
        }

        BLOCK();

        getCodeCloseBlockWhile(labelFalse, labelLoop);

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

        // Code open if      
        string label = getCodeOpenIf(parRet.addr);

        BLOCK();
        
        // Code close if
        getCodeCloseBlockIf(label);

        if (this->token == Alphabet::ELSE)
        {
            label = getCodeOpenElse(parRet.addr);
            // Code open else
            matchToken(Alphabet::ELSE);
            BLOCK();
            // Code close else
            getCodeCloseBlockIf(label);
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
        if (symbolTable->getType(regLex.lexeme) == ConstType::BOOLEAN)
        {
            cout << "(39)" << endl;
            throwIncompatibleType();
        }

        // Semantic Action 40
        if (symbolTable->getClass(regLex.lexeme) == ClassType::CONST)
        {
            cout << "(40)" << endl;
            throwIncompatibleClass(regLex.lexeme);
        }

        getCodeRead(symbolTable->getAddr(regLex.lexeme), symbolTable->getType(regLex.lexeme));

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
            isWriteLn = true;
        }

        matchToken(Alphabet::OPENPAR);

        expRet = EXP(true);
        getCodeWrite(expRet.addr, expRet.type);

        while (this->token == Alphabet::COMMA)
        {
            matchToken(Alphabet::COMMA);
            expRet = EXP(true);
            getCodeWrite(expRet.addr, expRet.type);

        };

        if (isWriteLn)
        {
            getCodeWriteLineBr();
        }

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
    parRet = EXP(true);
    matchToken(Alphabet::CLOSEPAR);

    return parRet;
}
/**
 * @brief Variable BLOCK of the L Language Grammar
 */
void SyntaxAnalyzer::BLOCK()
{
    if (checkFirstCMD())//Verify the first of CMD
    {
        CMD();
    }
    else if (checkFirstATR())//Verify the first of ATR
    {
        ATR();
        matchToken(Alphabet::SEMICOLON);
    }
    else if (this->token == Alphabet::OPENBRACE)
    {
        matchToken(Alphabet::OPENBRACE);

        while (checkFirstCMD() || checkFirstATR())//Go to commands of CMD or ATR, while existing
        {
            if (checkFirstCMD())//Go to commands of CMD
            {
                CMD();
            }
            else if (checkFirstATR())//Go to commands of ATR
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
 * @param T
 * @param T1
 * @param operation
 * @return ExpressionReturn
 */
ExpressionReturn SyntaxAnalyzer::verifyTypesForT(ExpressionReturn T, ExpressionReturn T1, int operation)
{
    ExpressionReturn tRet;

    // CHAR OPERATIONS
    if (T.type == ConstType::CHAR && T1.type != ConstType::CHAR)
    {
        cout << "(26-1)" << endl;
        throwIncompatibleType();
    }
    //Throws exception if type ID is diferent of return type [char]
    else if (T.type != ConstType::CHAR && T1.type == ConstType::CHAR)
    {
        cout << "(26-2)" << endl;
        throwIncompatibleType();
    }
    else if ((T.type == ConstType::CHAR && T1.type == ConstType::CHAR))
    {
        tRet.addr = getCodeCmpForCharAndChar(T.addr, T1.addr, operation);
    }
    // INT X FLOAT OPERATIONS
    else if (T.type == ConstType::INT && (T1.type != ConstType::INT && T1.type != ConstType::FLOAT))
    {
        cout << "(26-3)" << endl;
        throwIncompatibleType();
    }
    //Throws exception if type ID is diferent of return type [int or float]
    else if ((T.type != ConstType::INT && T.type != ConstType::FLOAT) && T1.type == ConstType::INT)
    {
        cout << "(26-4)" << endl;
        throwIncompatibleType();
    }
    //Throws exception if type ID is diferent of return type [float]
    else if (T.type == ConstType::FLOAT && (T1.type != ConstType::INT && T1.type != ConstType::FLOAT))
    {
        cout << "(26-5)" << endl;
        throwIncompatibleType();
    }
    //Throws exception if type ID is diferent of return type [int or float]
    else if ((T.type != ConstType::INT && T.type != ConstType::FLOAT) && T1.type == ConstType::FLOAT)
    {
        cout << "(26-6)" << endl;
        throwIncompatibleType();
    }
    else if (T.type == ConstType::INT && T1.type == ConstType::INT)
    {
        tRet.addr = getCodeCmpForIntAndInt(T.addr, T1.addr, operation);
    }
    else if (T.type == ConstType::INT && T1.type == ConstType::FLOAT)
    {
        tRet.addr = getCodeCmpForIntAndFloat(T.addr, T1.addr, operation);
    }
    else if (T.type == ConstType::FLOAT && T1.type == ConstType::INT)
    {
        tRet.addr = getCodeCmpForFloatAndInt(T.addr, T1.addr, operation);
    }
    else if (T.type == ConstType::FLOAT && T1.type == ConstType::FLOAT)
    {
        tRet.addr = getCodeCmpForFloatAndFloat(T.addr, T1.addr, operation);
    }
    //Throws exception if the user is trying make any operation with string diferent of "equal"
    else if ((T.type == ConstType::STRING || T1.type == ConstType::STRING) && operation != Alphabet::EQUAL)
    {
        cout << "(26-7)" << endl;
        throwIncompatibleType();
    }
    //Throws exception if type ID is diferent of return type [string]
    else if (T.type == ConstType::STRING && T1.type != ConstType::STRING)
    {
        cout << "(26-8)" << endl;
        throwIncompatibleType();
    }

    //Throws exception if type ID is diferent of return type [string]
    else if (T.type != ConstType::STRING && T1.type == ConstType::STRING)
    {
        cout << "(26-9)" << endl;
        throwIncompatibleType();
    }


    else if (T.type == ConstType::STRING && T1.type == ConstType::STRING)
    {
        tRet.addr = getCodeCmpForStringAndString(T.addr, T1.addr, operation);
    }
    // B//Throws exception if type ID is diferent of return type [string]
    else if (T.type == ConstType::BOOLEAN || T1.type == ConstType::BOOLEAN)
    {
        cout << "(26-10)" << endl;
        throwIncompatibleType();
    }
    return tRet;
}

/**
 * @brief Variable EXP of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::EXP(bool isToResetTemp)
{
    if(isToResetTemp)
    {
        resetTempCounter();
    }
    ExpressionReturn expRet, tRet, t1Ret;
    int operation = null;

    tRet = T();

    // Semantic Action 37
    expRet.type = tRet.type;
    expRet.addr = tRet.addr;

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
        tRet.addr = verifyTypesForT(tRet, t1Ret, operation).addr;
        tRet.type = ConstType::BOOLEAN;
    }

    // Semantic Action 14
    expRet.type = tRet.type;
    expRet.addr = tRet.addr;

    return expRet;
}

/**
 * @brief Variable T of the L Language Grammar
 */
ExpressionReturn SyntaxAnalyzer::T()
{
    ExpressionReturn tRet, rRet, r1Ret;
    int operation;
    bool isPlusOrMinus = false;
    bool hasMinus = false;

    if (this->token == Alphabet::PLUS)
    {
        // Semantic Action 34
        isPlusOrMinus = true;
        matchToken(Alphabet::PLUS);
    }
    else if (this->token == Alphabet::MINNUS)
    {
        hasMinus = true;
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

    if (hasMinus)
    {
        assemblyCmd += "\tmov EAX, [M + " + to_string(rRet.addr) + "] \t\t\t; Move o valor da memoria para o registrador EAX\n";
        assemblyCmd += "\tneg EAX \t\t\t; Nega o valor do registrador EAX\n";
        assemblyCmd += "\tmov [M + " + to_string(rRet.addr) + "], EAX \t\t\t; Guarda valor negado no endereco original\n";
    }

    tRet = rRet;

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
            if ((tRet.type != ConstType::INT && tRet.type != ConstType::FLOAT) || (r1Ret.type != ConstType::INT && r1Ret.type != ConstType::FLOAT))
            {
                cout << "(24-1)" << endl;
                throwIncompatibleType();
            }
            else
            {
                if (tRet.type == ConstType::FLOAT || r1Ret.type == ConstType::FLOAT)
                {
                    tRet.type = ConstType::FLOAT;

                    if (tRet.type == ConstType::FLOAT && r1Ret.type == ConstType::FLOAT)
                    {
                        tRet.addr = getCodePlusMinnusForFloat(tRet.addr, r1Ret.addr, operation);
                    }
                    else if (tRet.type == ConstType::INT)
                    {
                        tRet.addr = getCodePlusMinnusForFloatAndInt(tRet.addr, r1Ret.addr, operation);
                    }
                    else
                    {
                        tRet.addr = getCodePlusMinnusForIntAndFloat(tRet.addr, r1Ret.addr, operation);
                    }
                }
                else
                {
                    tRet.type = ConstType::INT;
                    tRet.addr = getCodePlusMinnusForInt(tRet.addr, r1Ret.addr, operation);
                }
            }
        }
        else if (operation == Alphabet::OR)
        {
            if (tRet.type != ConstType::BOOLEAN || r1Ret.type != ConstType::BOOLEAN)
            {
                cout << "(24-2)" << endl;
                throwIncompatibleType();
            }
            else
            {
                tRet.addr = getCodeForOR(tRet.addr, r1Ret.addr);
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
            if (M.type == ConstType::FLOAT && M1.type == ConstType::FLOAT)
            {
                rRet.addr = getCodeTimesOperationtForFloat(M.addr, M1.addr);
            }
            else if (M.type == ConstType::INT)
            {
                rRet.addr = getCodeTimesOperationtForFloatAndInt(M1.addr, M.addr);
            }
            else
            {
                rRet.addr = getCodeTimesOperationtForFloatAndInt(M.addr, M1.addr);
            }
        }
        else
        {
            rRet.type = ConstType::INT;
            rRet.addr = getCodeTimesOperationtForInt(M.addr, M1.addr);
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

        rRet.type = ConstType::INT;
        rRet.addr = getCodeDivOperationtForInt(M.addr, M1.addr);
    }
    else if (operation == Alphabet::DIVIDE)
    {
        // Impossible to execute a divide operation with types different
        // of int and float
        if ((M.type != ConstType::INT && M.type != ConstType::FLOAT) ||
            (M1.type != ConstType::INT && M1.type != ConstType::FLOAT))
        {
            cout << "(22-3.0)" << endl;
            throwIncompatibleType();
        }

        // If divide operation contains a float, change the operation
        // type to float
        if (M.type == ConstType::FLOAT || M1.type == ConstType::FLOAT)
        {
            rRet.type = ConstType::FLOAT;

            if (M.type == ConstType::FLOAT && M1.type == ConstType::FLOAT)
            {
                rRet.addr = getCodeDivideOperationtForFloat(M.addr, M1.addr);
            }
            else if (M.type == ConstType::INT)
            {
                rRet.addr = getCodeDivideOperationtForIntAndFloat(M.addr, M1.addr);
            }
            else
            {
                rRet.addr = getCodeDivideOperationtForFloatAndInt(M.addr, M1.addr);
            }
        }
        else
        {
            rRet.type = ConstType::FLOAT;
            rRet.addr = getCodeDivideOperationtForInt(M.addr, M1.addr);
        }
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
        rRet.addr = getCodeTimesOperationtForInt(M.addr, M1.addr);
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
        rRet.addr = getCodeModOperationtForInt(M.addr, M1.addr);
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
        rRet = rGetReturn(rRet, m1Ret, operation);
    }

    // If semantic action 22 has not been executed
    // Semantic action 12
    if (rRet.type == null)
    {
        rRet.type = mRet.type;
        rRet.type = mRet.addr;
    }

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

        mRet.addr = getCodeNotExp(m1Ret.addr, m1Ret.type);
    }
    // TYPE CASTING
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
        expRet = EXP(true);

        // Semantic Action 17
        if (expRet.type != ConstType::INT && expRet.type != ConstType::FLOAT)
        {
            cout << "(17)" << endl;
            throwIncompatibleType();
        }

        if ((expRet.type != ConstType::INT && convType == ConstType::INT) || (expRet.type != ConstType::FLOAT && convType == ConstType::FLOAT)){

            mRet.addr = getCodeCasting(expRet.addr, convType);
        } else {
            mRet = expRet;
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
            expRet = EXP(true);

            if (expRet.type != ConstType::INT)
            {
                cout << "(9)" << endl;
                throwIncompatibleType();
            }

            mRet.addr = getCodeAccessStringPosition(mRet.addr, expRet.addr);
            lexID = ConstType::CHAR;

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
    else
    {
        matchToken(Alphabet::OPENPAR);
        // Semantic Action 36
        mRet = EXP(true);

        matchToken(Alphabet::CLOSEPAR);
    }
    return mRet;
}

/**
 * @brief Split a string s using a delimiter
 *
 * @param string s, string delimiter
 * @return list<string> split itens
 */
list<string> split(string s, string delimiter) 
{
    // Finding first apperance of delimiter
    int pos = s.find(delimiter);

    list<string> splitRet;

    // while != end of string
    while (pos != string::npos) {
        token = s.substr(0, pos);
        //cout << token << endl;
        // Slipt delimiter
        splitRet.push_back(s.substr(0, pos));

        // Deleting analised substring
        s.erase(0, pos + delimiter.length());
        // Finding next apperance of delimiter
        pos = s.find(delimiter);
    }
    splitRet.push_back(s);

    return splitRet;
}

/**
 * @brief check if the source of line 1 is the same as the destination of line 2
 * 
 * @param string splitReturnLine1P1 (string 1 arg 1), string splitReturnLine1P2 (string 1 arg 2), 
 *        string splitReturnLine2P1 (string 2 arg 1), string splitReturnLine2P2 (string 2 arg 2)
 */
bool verifyParamEqPeephole(string splitReturnLine1P1, string splitReturnLine1P2, 
                              string splitReturnLine2P1, string splitReturnLine2P2){

    return splitReturnLine1P1.length() == 9 && splitReturnLine2P2.length() >= 9 &&
           splitReturnLine2P1.length() == 3 && splitReturnLine1P2.length() >= 3 &&
           splitReturnLine1P1[0] == splitReturnLine2P2[0] &&
           splitReturnLine1P1[1] == splitReturnLine2P2[1] &&
           splitReturnLine1P1[2] == splitReturnLine2P2[2] &&
           splitReturnLine1P1[3] == splitReturnLine2P2[3] &&
           splitReturnLine1P1[4] == splitReturnLine2P2[4] &&
           splitReturnLine1P1[5] == splitReturnLine2P2[5] &&
           splitReturnLine1P1[6] == splitReturnLine2P2[6] &&
           splitReturnLine1P1[7] == splitReturnLine2P2[7] &&
           splitReturnLine1P1[8] == splitReturnLine2P2[8] &&
           splitReturnLine2P1[0] == splitReturnLine1P2[0] &&
           splitReturnLine2P1[1] == splitReturnLine1P2[1] &&
           splitReturnLine2P1[2] == splitReturnLine1P2[2];
}

/**
 * @brief Peephole assembly optimization
 */
void peephole()
{
    list<string> splitReturn = split(assemblyCmd, "\n");
    int n = splitReturn.size();
    // Iterator string [i - 1]
    list<string>::iterator it1 = splitReturn.begin();

    // Iterator string [i]
    list<string>::iterator it2 = splitReturn.begin();
    ++it2;
    string line1;
    string line2;

    assemblyCmd = "";
    assemblyCmd = *it1 + "\n";;

    for(int i = 1; i < n; i++){
       // cout << *it1 << endl;
        line1 = *it1;
        line2 = *it2;

        // Checking if both lines are mov commands
        if(line1[1] == 'm' && line1[2] == 'o' && line1[3] == 'v' && 
           line2[1] == 'm' && line2[2] == 'o' && line2[3] == 'v')
        {   
            // Removing [\tmov ]
            line1 = line1.substr(5, line1.length());
            line2 = line2.substr(5, line2.length());
            
            // Separating mov arguments
            list<string> splitReturnLine1 = split(line1, ", ");
            list<string>::iterator it3 = splitReturnLine1.begin();
            list<string> splitReturnLine2 = split(line2, ", ");
            list<string>::iterator it4 = splitReturnLine2.begin();

            string splitReturnLine1P1 = *it3;
            it3++;
            string splitReturnLine1P2 = *it3;

            string splitReturnLine2P1 = *it4;
            it4++;
            string splitReturnLine2P2 = *it4;

            // Checking if the source of line 1 is the same as the destination of line 2
            if(verifyParamEqPeephole(splitReturnLine1P1, splitReturnLine1P2, splitReturnLine2P1, splitReturnLine2P2))
            {
                // Removing line 2 of the assembly
                it1++;
                it2++;
            }
        }

        assemblyCmd += *it2 + "\n";
        it1++;
        it2++; 
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

    assembly += "section .data                 ; Sessão de dados\n";
    assembly += "M:                            ; Rotulo para demarcar o\n";
    assembly += "                              ; inicio da sessao de dados\n";
    assembly += "\tresb 0x10000              ; Reserva de temporarios\n";
    assembly += "   ; ***Definicoes de variaveis e constantes\n";

    S();

    if(isToUsePeephole)
    {
        peephole();
    }

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


