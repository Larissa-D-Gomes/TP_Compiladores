#pragma once

#include <string>
#include <iostream>

#include "LexicalRegister.hpp"

using namespace std;

class SyntaxAnalyzer
{

private:
    LexicalRegister tokenFromLexical;
    int token;

public:
    SyntaxAnalyzer();

    void matchToken(int expectedToken);

    bool checkFirstDEC();

    bool checkFirstCMD();

    bool checkFirstT();

    bool checkFirstR();

    bool checkFirstM();

    void S();

    void DEC();

    void ATR();

    void DECONST();

    void CMD();

    void PAR();

    void BLOCK();

    void EXP();

    void T();

    void R();

    void M();

    void parser();
};