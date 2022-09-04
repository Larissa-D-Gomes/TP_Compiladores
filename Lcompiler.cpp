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
#include<sstream>

using namespace std;

class SymbolTable
{
    //TODO: Alterar a hash para não precisar de iteração
    private:
        // reserved words to L language
        vector<string> reservedWords = {"const", "int", "char", "while", "if", "float", "else", "&&", "||", "!", ":=", "=", "(", ")", "<", ">", "!=", ">=", "<=", ",", "+", "-", "*", "/", ";", "{", "}", "readln", "div", "string", "write", "writeln", "mod", "[", "]", "true", "false", "boolean"};
        // Symbol hash table 
        unordered_map<string, int> symbolTable;

    public:
        // Constructor
        SymbolTable(){
            // numbering tokens
            fillHash();
        }

        // Fill hash symbol table with reserved words 
        void fillHash()
        {
            // fill reserved words list using a iterator to use like position for each word
            for (int i = 0; i < reservedWords.size(); i++)
            {
                symbolTable[reservedWords[i]] = i;
            }
        }

};

// Global Symbol Table
SymbolTable* symbolTable = new SymbolTable();

// Global cursos
int cursor;
// Global program length
int eof;
// Global program string
string program;


bool validateChar(char c)
{
    //TODO: Validar char 
    return true;

}

string lexicalAnalyzer(){
    char c; // read character
    int state = 0;
    string token = "";

    // While state != final state
    while(state != -1)
    {
        if(cursor == eof)
        {
            c = program[cursor];

            if(validateChar(c))
            {
                //TODO: Erro
            }
        } 
        else
        {
            // Flag EOF
            token = "";
        }
        
        switch(state) {
            case 1:

                break;
            case 2:

                break;
            default:
                break;
        }
        
    }
    return token;
}

int main()
{
    string srcProgram;
    int line = 1;
    cursor = 0;

    std::ifstream programFile ("program.txt");
    // if file is valid
    if(programFile) 
    {
        // Read the complete program file
        ostringstream stream;
        stream << programFile.rdbuf(); // reading data
        program = stream.str();
    }

    // Setting the global variable to control eof
    eof = program.length();
    string token;

    // Calling lexical analyzer while eof is not reached
    while(token != "")
    {
        token = lexicalAnalyzer();
    }

    return 0;
}