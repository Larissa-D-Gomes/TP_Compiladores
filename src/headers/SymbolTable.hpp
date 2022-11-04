/**
 * @file SymbolTable.hpp
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

#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

#define null -999

using namespace std;

struct RegST
{
    int token = null;
    int classType = null;
    int type = null;
};

class SymbolTable
{
    // TODO: Alterar a hash para não precisar de iteração
private:
    // reserved words to L language
    vector<string> reservedWords = {"const", "int", "char", "while", "if", "float", "else", "&&", "||", "!", ":=", "=", "(", ")", "<", ">", "!=", ">=", "<=", ",", "+", "-", "*", "/", ";", "{", "}", "readln", "div", "string", "write", "writeln", "mod", "[", "]", "true", "false", "boolean"};

    // Symbol hash table
    unordered_map<string, RegST> symbolTable;

public:
    // Constructor
    SymbolTable();

    /**
     * @brief Fill Symbol Table (only called at the begin of the program)
     */
    void fillHash();

    /**
     * @brief Search for the lexical form in the Symbol Table
     *
     * @param lex the lexical form that you want to get position
     * @return int position of the lexical form
     */
    int search(string lex);

    /**
     * @brief Find for the lexical form in the Symbol Table
     *
     * @param lex the lexical form that you want to get position
     * @return int index of the lexical form on the hash table
     */
    int find(string lex);

    /**
     * @brief Insert a new lexical form at the symbol table
     *
     * @param lex lexical form to insert
     * @return int position of insertion
     */
    int insert(string lex);

    /**
     * @brief print Symbol Table content (only used for tests)
     */
    void print();

    /**
     * @brief Alter the type of a register at the symbol table
     *
     * @param lex lexical form to insert, int type new type
     */
    void setType(string lex, int type);

    /**
     * @brief Alter the class of a register at the symbol table
     *
     * @param lex lexical form to insert, int classType new class
     */
     void setClass(string lex, int classType);

    /**
     * @brief Return the type of a lexeme
     *
     * @param lex the lexical form that you want to get position
     * @return int -> type of lexeme
     */
     int getType(string lex);

    /**
     * @brief Return the class of a lexeme
     *
     * @param lex the lexical form that you want to get position
     * @return int -> class of the lexeme
     */
    int getClass(string lex);
};