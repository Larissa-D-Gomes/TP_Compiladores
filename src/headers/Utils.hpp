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

/**
 * @brief Just a testing function to see lexem:token relationship
 * 
 * @param n - number of token
 * @return string - lexem
 */
string testLexem(int n);

/**
 * @brief Verify if a char value is a character
 * 
 * @param c - char to validate
 * @return true - Is a character
 * @return false - Is NOT a character
 */
bool isCharacter(char c);

/**
 * @brief Verify if a char value is a number
 * 
 * @param c - char to validate
 * @return true - Is a number
 * @return false - Is NOT a number
 */
bool isNumber(char c);

/**
 * @brief Verify if a char value is a hexa value
 * 
 * @param c - char to validate
 * @return true - Is a hexa value
 * @return false - Is NOT a hexa value
 */
bool isHexa(char c);

/**
 * @brief Verify if a char value is a valid character
 * 
 * @param c - char to validate
 * @return true - Is a valid character
 * @return false - Is NOT a valid character
 */
bool isValidChar(char c);