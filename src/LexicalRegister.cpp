#include "headers/LexicalRegister.hpp"

#include <string>
#include <iostream>

LexicalRegister::LexicalRegister(string lexeme, int token, int symbolTabPos, int constType)
{
    this->lexeme = lexeme;
    this->token = token;
    this->symbolTabPos = symbolTabPos;
    this->constType = constType;
}