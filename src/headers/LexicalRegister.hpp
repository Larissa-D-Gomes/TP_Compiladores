#pragma once

#include <string>
#include <iostream>

using namespace std;

class LexicalRegister
{
public:
    string lexeme;
    int token;
    int symbolTabPos;
    int constType;

    LexicalRegister() {}

    LexicalRegister(string lexeme, int token, int symbolTabPos, int constType);
};