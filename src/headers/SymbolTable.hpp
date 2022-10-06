#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

class SymbolTable
{
    // TODO: Alterar a hash para não precisar de iteração
private:
    // reserved words to L language
    vector<string> reservedWords = {"const", "int", "char", "while", "if", "float", "else", "&&", "||", "!", ":=", "=", "(", ")", "<", ">", "!=", ">=", "<=", ",", "+", "-", "*", "/", ";", "{", "}", "readln", "div", "string", "write", "writeln", "mod", "[", "]", "true", "false", "boolean"};

    // Symbol hash table
    unordered_map<string, uint8_t> symbolTable;

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
};