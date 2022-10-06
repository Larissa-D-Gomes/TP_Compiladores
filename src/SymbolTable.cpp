/**
 * @file SymbolTable.cpp
 * 
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Symbol Table structure
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#include "headers/SymbolTable.hpp"
#include "headers/Alphabet.hpp"

using namespace std;

#define finalState -1
#define null -1

SymbolTable::SymbolTable()
{
    // numbering tokens
    fillHash();
}

/**
 * @brief Fill Symbol Table (only called at the begin of the program)
 */
void SymbolTable::fillHash()
{

    // fill reserved words list using a iterator
    // list position = alphabet byte value
    for (uint8_t i = 0; i < reservedWords.size(); i++)
    {
        symbolTable[reservedWords[i]] = i;
    }
}

/**
 * @brief Search for the lexical form in the Symbol Table
 *
 * @param lex the lexical form that you want to get position
 * @return int position of the lexical form
 */
int SymbolTable::search(string lex)
{
    // If map does not contain lex
    if (this->symbolTable.find(lex) == this->symbolTable.end())
        return null;

    // Return token of lex
    return this->symbolTable.at(lex);
}

/**
 * @brief Find for the lexical form in the Symbol Table
 *
 * @param lex the lexical form that you want to get position
 * @return int index of the lexical form on the hash table
 */
int SymbolTable::find(string lex)
{
    int position = 0;

    for (auto const &pair : this->symbolTable)
    {
        if (lex == pair.first)
            return position;
        position++;
    }

    return null;
}

/**
 * @brief Insert a new lexical form at the symbol table
 *
 * @param lex lexical form to insert
 * @return int position of insertion
 */
int SymbolTable::insert(string lex)
{
    this->symbolTable[lex] = Alphabet::ID;
    return this->search(lex);
}

/**
 * @brief print Symbol Table content (only used for tests)
 */
void SymbolTable::print()
{
    for (auto const &pair : this->symbolTable)
    {
        cout << "{ " << pair.second << " : " << pair.first << " }\n";
    }
}