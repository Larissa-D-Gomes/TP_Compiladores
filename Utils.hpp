/**
 * @file Utils.cpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS 
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Utils - salgorithms 
 * @version 0.1
 * @date 2022-08-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <unordered_map>
#include <list>
#include <string.h>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

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

void throwInvalidCharacterException(int line)
{
    cout << line << "\ncaractere invalido.";
    exit(1);
}

void throwUnexpectedEOFException(int line)
{
    cout << line << "\nfim de arquivo nao esperado.";
    exit(1);
}

void throwUndefinedLex(string lex, int line )
{
    lex.pop_back();
    cout << line << "\nlexema nao identificado [" << lex << "].";
    exit(1);
}

bool isValidChar(char c)
{
    return ((c >= ' ' && c <= '"') || (c >= 'A' && c <= ']') || c == '/' || c == '_' || (c >= 'a' && c <= '}') || (c >= '%' && c <= '?')) || (c == '@') || c == '\n' || c == '\r' || c == '\t';
}
