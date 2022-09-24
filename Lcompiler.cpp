/**
 * @file Lcompiler.cpp
 *
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief A compiler for the L language, created for the compiler course of computation science at PUC Minas MG - BR
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

#define finalState -1
#define null -1



int main()
{
    program = "";
    string str;
    line = 1;
    cursor = 0;

    while (getline(cin, str))
    {
        program += str;
        program.push_back('\n');
    }

    // Setting the global variable to control eof
    eof = program.length();

    // Initializing token with a char != of eof flag
    string token = "";

    // Calling lexical analyzer while eof is not reached
    while (cursor != eof)
    {
        token = lexicalAnalyzer();

        // if (token != "")
        // {
        //     cout << "Token: " << token << endl;
        // }
    }

    // symbolTable->print();
    line == 1 ? (cout << "1 linha compilada.") : (cout << line << " linhas compiladas.");

    return 0;
}