/**
 * @file LexicalRegister.hpp
 * 
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Lexical Register structure
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
 * @brief Lexical Register to represent the lexeme read from Lexical Analyzer
 *
 */
class LexicalRegister
{
public:
    string lexeme;    // lexeme read
    int token;        // lexeme token
    int symbolTabPos; // position of the lexeme on symbol table (null if it is not on symbol table)
    int constType;    // const type (if it is a const token)
    int constSize;    // const size (if it is a const token) in bytes

    /**
     * @brief Construct a new Lexical Register object
     *
     */
    LexicalRegister() {}

    /**
     * @brief Construct a new Lexical Register object
     *
     * @param lexeme        // lexeme read
     * @param token         // lexeme token
     * @param symbolTabPos  // position of the lexeme on symbol table (null if it is not on symbol table)
     * @param constType     // const type (if it is a const token)
     * @param constSize     // const size (if it is a const token)
     */
    LexicalRegister(string lexeme, int token, int symbolTabPos, int constType, int constSize);
};