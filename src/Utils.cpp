/**
 * @file Utils.hpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Utils file and global important variables
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <string.h>

#include "headers/Utils.hpp"
#include "headers/SymbolTable.hpp"
#include "headers/LexicalRegister.hpp"

using namespace std;

// Global cursos
int cursor;
// Global program length
int eof;
// Global program string
string program;
// Lines of the program
int line;
// Flag last read token is \n 
bool lastTokenBreakLine;
// Global Symbol Table structure
SymbolTable *symbolTable;

/**
 * @brief Just a testing function to see lexem:token relationship
 * 
 * @param n - number of token
 * @return string - lexem
 */
string testLexem(int n){
    if(n == 0) return "const";
    else if(n == 1) return "int";
    else if(n == 2) return "char";
    else if(n == 3) return "while";
    else if(n == 4) return "if";
    else if(n == 5) return "float";
    else if(n == 6) return "else";
    else if(n == 7) return "&&";
    else if(n == 8) return "||";
    else if(n == 9) return "!";
    else if(n == 10) return ":=";
    else if(n == 11) return "=";
    else if(n == 12) return "(";
    else if(n == 13) return ")";
    else if(n == 14) return "<";
    else if(n == 15) return ">";
    else if(n == 16) return "!=";
    else if(n == 17) return ">=";
    else if(n == 18) return "<=";
    else if(n == 19) return ",";
    else if(n == 20) return "+";
    else if(n == 21) return "-";
    else if(n == 22) return "*";
    else if(n == 23) return "/";
    else if(n == 24) return ";";
    else if(n == 25) return "{";
    else if(n == 26) return "}";
    else if(n == 27) return "readl";
    else if(n == 28) return "div";
    else if(n == 29) return "string";
    else if(n == 30) return "write";
    else if(n == 31) return "writeln";
    else if(n == 32) return "mod";
    else if(n == 33) return "[";
    else if(n == 34) return "]";
    else if(n == 35) return "true";
    else if(n == 36) return "false";
    else if(n == 37) return "boolean";
    else if(n == 40) return "id";
    else if(n == 41) return "constant";

    return "";
}

/**
 * @brief Verify if a character is a character
 *
 * @param c character to verify
 * @return true - If is number
 * @return false - If is not number
 */
bool isCharacter(char c)
{
    return (tolower(c) >= 'a' && tolower(c) <= 'z');
}

/**
 * @brief Verify if a character is a number
 *
 * @param c character to verify
 * @return true - If is number
 * @return false - If is not number
 */
bool isNumber(char c)
{
    return (c >= '0' && c <= '9');
}

/**
 * @brief Verify if a character is a hexadecimal valid value
 *
 * @param c character to verify
 * @return true - If is hexa
 * @return false - If is not hexa
 */
bool isHexa(char c)
{
    return isNumber(c) || (toupper(c) >= 'A' && toupper(c) <= 'F');
}

/**
 * @brief Valid a char read
 *
 * @param c - character to valid
 * @return true - if is valid
 * @return false - if is not valid
 */
bool isValidChar(char c)
{
    return ((c >= ' ' && c <= '"') || (c >= 'A' && c <= ']') || c == '/' || c == '_' || (c >= 'a' && c <= '}') || (c >= '%' && c <= '?')) || (c == '@') || c == '\n' || c == '\r' || c == '\t';
}

/**
 * @brief Print Lexical Register (just for debug)
 * 
 * @param lex Lexical Register to be printed
 */
void printLexicalRegister(LexicalRegister lex)
{
    cout << "{\n   Lexeme: " << lex.lexeme
         << "\n   Token: " << testLexem(lex.token)
         << "\n   Symbol Table Position: " << lex.symbolTabPos
         << "\n   Const Type: " << lex.constType
         << "\n   Const Size (em bytes): " << lex.constSize
         << "\n}\n" << endl;
}
