/**
 * @file main.cpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief A compiler for the L language, created for the compiler course of computation science at PUC Minas MG - BR
 * @version 0.1
 * @date 2022-08-25
 *
 * @copyright Copyright (c) 2022
 */

#include <iostream>
#include <string>

// import headers
#include "headers/Utils.hpp"
#include "headers/SymbolTable.hpp"
#include "headers/SyntaxAnalyzer.hpp"

using namespace std;

int main()
{
    program = "";
    string str;
    line = 1;
    cursor = 0;
    lastTokenBreakLine = false;
    symbolTable = new SymbolTable();

    // Read source program
    while (getline(cin, str))
    {
        program += str + '\n';
    }
    //cout << program << endl;

    // remove last \n
    program.pop_back();

    // Setting the global variable to control eof
    eof = program.length();

    // Call Syntax Analyzer method
    SyntaxAnalyzer syntaxAnalyzer;
    syntaxAnalyzer.parser();

    // Print line count and success compilation
    line == 1 ? (cout << "1 linha compilada." << endl) : (cout << line << " linhas compiladas."  << endl);

    return 0;
}