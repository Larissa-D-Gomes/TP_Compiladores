/**
 * @file LexicalRegister.cpp
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

#include "headers/LexicalRegister.hpp"

#include <string>
#include <iostream>

/**
 * @brief Construct a new Lexical Register object
 *
 * @param lexeme        // lexeme read
 * @param token         // lexeme token
 * @param symbolTabPos  // position of the lexeme on symbol table (null if it is not on symbol table)
 * @param constType     // const type (if it is a const token)
 */
LexicalRegister::LexicalRegister(string lexeme, int token, int symbolTabPos, int constType)
{
    this->lexeme = lexeme;
    this->token = token;
    this->symbolTabPos = symbolTabPos;
    this->constType = constType;
}