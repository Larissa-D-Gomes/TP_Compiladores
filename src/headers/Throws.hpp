/**
 * @file Throws.hpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Throws for many errors exceptions
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Invalid read character exception
 * Occurs when the Lexical Analyzer read a invalid character, like à
 */
void throwInvalidCharacterException();

/**
 * @brief Unexpected EOF read exception
 * Occurs when the Syntax Analyzer or Lexical Analyzer read EOF when it is not expected to
 */
void throwUnexpectedEOFException();

/**
 * @brief Undefined Lex read exception
 * Occurs when the lexeme was not identified by the Lexical Analyzer as a valid lexeme
 * @param lex - The lexeme read
 */
void throwUndefinedLex(string lex);

/**
 * @brief Unexpected Lex read exception
 * Occurs when the read token doesn't match the expected token by the L Language Grammar
 * @param lex - The lexeme read
 */
void throwUnexpectedToken(string lex);