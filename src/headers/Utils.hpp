#pragma once

#include <string>

#include "SymbolTable.hpp"

using namespace std;

// Global cursos
extern int cursor;
// Global program length
extern int eof;
// Global program string
extern string program;
// Lines of the program
extern int line;
// Flag last read token is \n 
extern bool lastTokenBreakLine;
// Global Symbol Table structure
extern SymbolTable *symbolTable;

string testLexem(int n);
bool isCharacter(char c);
bool isNumber(char c);
bool isHexa(char c);
bool isValidChar(char c);