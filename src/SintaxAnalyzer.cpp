#include <iostream>
#include <string>

#include "headers/SintaxAnalyzer.hpp"
#include "headers/LexicalRegister.hpp"
#include "headers/LexicalAnalyzer.hpp"
#include "headers/Utils.hpp"
#include "headers/Throws.hpp"
#include "headers/Alphabet.hpp"

#define finalState -1
#define null -1

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer()
{
    // Initializing lexeme with a char != of eof flag
    this->tokenFromLexical = LexicalRegister("", null, null, null);
}

void SyntaxAnalyzer::matchToken(int expectedToken)
{

    // cout << "1043: AnalisadorLexico: [" << testLexem(this->token)  << "] Esperado na Gramatica: [" << testLexem(expectedToken) << "]" << endl;

    if (this->token == expectedToken)
    {
        this->tokenFromLexical = lexicalAnalyzer();
        this->token = this->tokenFromLexical.token;
    }
    else
    {
        if (cursor == eof)
        {
            line--;
            throwUnexpectedEOFException();
        }
        else
            throwUnexpectedToken(this->tokenFromLexical.lexeme);
    }
}

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

bool SyntaxAnalyzer::checkFirstCMD()
{
    return this->token == Alphabet::WHILE ||
           this->token == Alphabet::IF ||
           this->token == Alphabet::SEMICOLON ||
           this->token == Alphabet::READLN ||
           this->token == Alphabet::WRITE ||
           this->token == Alphabet::WRITELN;
}

bool SyntaxAnalyzer::checkFirstT()
{
    return checkFirstR();
}

bool SyntaxAnalyzer::checkFirstR()
{
    return checkFirstM();
}

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

void SyntaxAnalyzer::S()
{
    while (checkFirstDEC() || checkFirstCMD())
    {
        if (checkFirstDEC())
        {
            DEC();
            matchToken(Alphabet::SEMICOLON);
        }
        else if (checkFirstCMD())
        {
            CMD();
        }
    }
}

void SyntaxAnalyzer::DEC()
{
    if (this->token == Alphabet::INT ||
        this->token == Alphabet::FLOAT ||
        this->token == Alphabet::STRING ||
        this->token == Alphabet::BOOLEAN ||
        this->token == Alphabet::CHAR)
    {
        if (this->token == Alphabet::INT) // INT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::INT);
        }
        else if (this->token == Alphabet::FLOAT) // FLOAT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::FLOAT);
        }
        else if (this->token == Alphabet::STRING) // STRING ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::STRING);
        }
        else if (this->token == Alphabet::BOOLEAN) // BOOLEAN ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::BOOLEAN);
        }
        else if (this->token == Alphabet::CHAR) // CHAR ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
        {
            matchToken(Alphabet::CHAR);
        }

        matchToken(Alphabet::ID);

        if (this->token == Alphabet::ATRIB)
        {
            matchToken(Alphabet::ATRIB);
            DECONST();
        }

        while (this->token == Alphabet::COMMA)
        {
            matchToken(Alphabet::COMMA);
            matchToken(Alphabet::ID);
            if (this->token == Alphabet::ATRIB)
            {
                matchToken(Alphabet::ATRIB);
                DECONST();
            }
        }
    }
    else if (this->token == Alphabet::CONST) // CONST ID = [-]CONSTANT
    {
        matchToken(Alphabet::CONST);
        matchToken(Alphabet::ID);
        matchToken(Alphabet::EQUAL);
        DECONST();
    }
    else
    {
        ATR();
    }
}

void SyntaxAnalyzer::ATR()
{
    matchToken(Alphabet::ID);

    if (this->token == Alphabet::OPENBRACKET)
    {
        matchToken(Alphabet::OPENBRACKET);
        EXP();
        matchToken(Alphabet::CLOSEBRACKET);
    }

    matchToken(Alphabet::ATRIB);
    EXP();
}

void SyntaxAnalyzer::DECONST()
{
    if (this->token == Alphabet::MINNUS) // - CONSTANT
    {
        matchToken(Alphabet::MINNUS);
    }
    if (this->token == Alphabet::CONSTANT)
    {
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
}

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
        matchToken(Alphabet::ID);
        matchToken(Alphabet::CLOSEPAR);
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
    }
}

void SyntaxAnalyzer::PAR()
{
    matchToken(Alphabet::OPENPAR);
    EXP();
    matchToken(Alphabet::CLOSEPAR);
}

void SyntaxAnalyzer::BLOCK()
{
    if (checkFirstCMD())
    {
        CMD();
        matchToken(Alphabet::SEMICOLON);
    }
    else if (this->token == Alphabet::ID)
    {
        ATR();
        matchToken(Alphabet::SEMICOLON);
    }
    else if (this->token == Alphabet::OPENBRACE)
    {
        matchToken(Alphabet::OPENBRACE);

        while (checkFirstCMD() || this->token == Alphabet::ID)
        {
            if (checkFirstCMD())
            {
                CMD();
                matchToken(Alphabet::SEMICOLON);
            }
            else if (this->token == Alphabet::ID)
            {
                ATR();
                matchToken(Alphabet::SEMICOLON);
            }
        };

        matchToken(Alphabet::CLOSEBRACE);
    }
}

void SyntaxAnalyzer::EXP()
{
    // if (checkFirstT())
    //{
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
    //}
}

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

void SyntaxAnalyzer::parser()
{

    this->tokenFromLexical = lexicalAnalyzer();
    this->token = this->tokenFromLexical.token;

    S();
}
