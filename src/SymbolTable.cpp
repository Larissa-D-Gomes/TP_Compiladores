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

#define null -999

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
        RegST reg;
        reg.token = i;
        symbolTable[reservedWords[i]] = reg;
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
    return this->symbolTable.at(lex).token;
}

/**
 * @brief Return the class of a lexeme
 *
 * @param lex the lexical form that you want to get position
 * @return int -> class of the lexeme
 */
int SymbolTable::getClass(string lex)
{
    return this->symbolTable[lex].classType;
}

/**
 * @brief Return the type of a lexeme
 *
 * @param lex the lexical form that you want to get position
 * @return int -> type of the lexeme
 */
int SymbolTable::getType(string lex)
{
    return this->symbolTable[lex].type;
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
    this->symbolTable[lex].token = Alphabet::ID;
    return this->search(lex);
}

/**
 * @brief Alter the type of a register at the symbol table
 *
 * @param lex lexical form to insert, int type new type
 */
void SymbolTable::setType(string lex, int type)
{
    this->symbolTable[lex].type = type;
}

/**
 * @brief Alter the class of a register at the symbol table
 *
 * @param lex lexical form to insert, int classType new class
 */
void SymbolTable::setClass(string lex, int classType)
{
    this->symbolTable[lex].classType = classType;
}

/**
 * @brief Return the address of a lexeme
 *
 * @param lex the lexical form that you want to get address
 * @return int -> type of lexeme
 */
int SymbolTable::getAddr(string lex){
    return this->symbolTable[lex].addr;
}

/**
 * @brief Set the address of a register at the symbol table
 *
 * @param lex lexical form to insert, int addr new class
 */
void SymbolTable::setAddr(string lex, long addr){
    this->symbolTable[lex].addr = addr;
}

/**
 * @brief print Symbol Table content (only used for tests)
 */
void SymbolTable::print()
{
    for (auto const &pair : this->symbolTable)
    {
        cout << "{token: " << pair.second.token << "    lex: " << pair.first << "    type:" << pair.second.type << "    classType: " << pair.second.classType << "    addr: " << pair.second.addr << "}\n";
    }
}