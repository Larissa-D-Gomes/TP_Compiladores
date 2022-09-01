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
using namespace std;

unordered_map<string, int> symbolTable; // global symbol table

/**
 * @brief Initialize the symbol table with the reserverd words
 *
 * @param symbolTable
 * @return unordered_map<string, int>
 */
void fillSymbolTable()
{
    // reserved words to L language
    vector<string> reservedWords = {"const", "int", "char", "while", "if", "float", "else", "&&", "||", "!", ":=", "=", "(", ")", "<", ">", "!=", ">=", "<=", ",", "+", "-", "*", "/", ";", "{", "}", "readln", "div", "string", "write", "writeln", "mod", "[", "]", "true", "false", "boolean"};

    // fill reserved words list using a iterator to use like position for each word
    for (int i = 0; i < reservedWords.size(); i++)
    {
        symbolTable[reservedWords[i]] = i;
    }
}

bool isReservedWord(string s)
{
    unordered_map<string, int>::const_iterator got = symbolTable.find(s);
    if (got == symbolTable.end()){
        cout << "not found";
        return false;
    }
    else cout << got->first << " is " << got->second;

    cout << endl;
    return true;
    
}

bool isDelimiter(char c){

    // bool isDelimiter = ' '||'='||'<'||'>'||'('||')'||'"'||''
    return false;
}

/**
 * @brief 
 * 
 * @param srcProgram 
 */
void lexicalAnalyzer(int linha, string srcProgram){

    int l_cursor = 0, r_cursor = 0;
    int program_lenght = srcProgram.length();

    // do the parser while the program isn't the end of the source program      int x 19;
    while (r_cursor <= program_lenght && l_cursor <= r_cursor) {
        // if it isn't a delimiter, it means that the program is reading a lexeme yet
        if(!isDelimiter(srcProgram[r_cursor])){
            cout << srcProgram[r_cursor];
            r_cursor++;
        }
    }
}


int main()
{
    // symbol table implemented with hash table cpp standard
    fillSymbolTable();

    string srcProgram;
    int line = 1;

    while(getline(cin, srcProgram)){
        lexicalAnalyzer(line, srcProgram);
        line++;
    }

    return 0;
}