#pragma once

#include <string>
#include <iostream>

using namespace std;

void throwInvalidCharacterException();

void throwUnexpectedEOFException();

void throwUndefinedLex(string lex);

void throwUnexpectedToken(string lex);