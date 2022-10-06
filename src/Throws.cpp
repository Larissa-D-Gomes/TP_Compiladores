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