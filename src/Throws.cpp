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

#include "./headers/Throws.hpp"
#include "./headers/Utils.hpp"

#include <string>
#include <iostream>

using namespace std;

/**
 * @brief Print a error message when program read a invalid character
 *
 */
void throwInvalidCharacterException()
{
    cout << line << "\ncaractere invalido."  << endl;
    exit(1);
}

/**
 * @brief Print a error message when program read an unexpected EOF
 *
 */
void throwUnexpectedEOFException()
{
    if(lastTokenBreakLine)
    {
        cout << lastTokenBreakLine << endl;
        line--;
    }
    cout << line << "\nfim de arquivo nao esperado.";
    exit(1);
}

/**
 * @brief Print a error message when program read a invalid lexeme
 *
 */
void throwUndefinedLex(string lex)
{
    if (lex.at(lex.length() - 1) == '\n') lex.pop_back();
    cout << line << "\nlexema nao identificado [" << lex << "].";
    exit(1);
}

/**
 * @brief Print a error message when program finds a unexpected token
 *
 */
void throwUnexpectedToken(string lex)
{
    cout << line << "\ntoken nao esperado [" << lex << "].";
    exit(1);
}

/**
 * @brief Print a error message when program finds a alredy declared id
 *
 */
void throwDeclaredID(string lex)
{
    cout << line << "\nidentificador ja declarado [" << lex << "].";
    exit(1);
}

/**
 * @brief Print a error message when program finds a alredy declared id
 *
 */
void throwNotDeclaredID(string lex)
{
    cout << line << "\nidentificador nao declarado [" << lex << "].";
    exit(1);
}

/**
 * @brief Print a error message when program finds incompatibles classes
 *
 */
void throwIncompatibleClass(string lex)
{
    cout << line << "\nclasse do identificador incompativel [" << lex << "]."<< endl;
    exit(1);
}

/**
 * @brief Print a error message when program finds incompatibles types
 *
 */
void throwIncompatibleType(string lex)
{
    cout << line << "\ntipos incompativeis"<< endl;
    exit(1);
}