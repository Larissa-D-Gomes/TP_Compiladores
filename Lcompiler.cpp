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

class SymbolTable
{
    // TODO: Alterar a hash para não precisar de iteração
private:
    // reserved words to L language
    vector<string> reservedWords = {"const", "int", "char", "while", "if", "float", "else", "&&", "||", "!", ":=", "=", "(", ")", "<", ">", "!=", ">=", "<=", ",", "+", "-", "*", "/", ";", "{", "}", "readln", "div", "string", "write", "writeln", "mod", "[", "]", "true", "false", "boolean"};
    // Symbol hash table
    unordered_map<string, int> symbolTable;

public:
    // Constructor
    SymbolTable()
    {
        // numbering tokens
        fillHash();
    }

    /**
     * @brief Fill Symbol Table (only called at the begin of the program
     */
    void fillHash()
    {
        // fill reserved words list using a iterator to use like position for each word
        for (int i = 0; i < reservedWords.size(); i++)
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
    int search(string lex)
    {
        unordered_map<string, int>::const_iterator got = this->symbolTable.find(lex);
        if (got == this->symbolTable.end())
        {
            return -1;
        }
        else
            return got->second;
    }

    /**
     * @brief Insert a new lexical form at the symbol table
     * 
     * @param lex lexical form to insert
     * @return int position of insertion
     */
    int insert(string lex)
    {
        this->symbolTable[lex] = this->symbolTable.size();
        return this->search(lex);
    }

    /**
     * @brief print Symbol Table content (only used for tests)
     */
    void print()
    {
        for (auto const &pair : this->symbolTable)
        {
            std::cout << "{ " << pair.second << " : " << pair.first << " }\n";
        }
    }
};

struct TransitionReturn
{
    int nextState;
    string tokenConcat;
};

// Global Symbol Table
SymbolTable *symbolTable = new SymbolTable();

// Global cursos
int cursor;
// Global program length
int eof;
// Global program string
string program;

bool validateChar(char c)
{
    // TODO: Validar char
    return true;
}

/* Execute state 0 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateZeroTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == ' ' || c == '\n' || c == '\t')
    {
        transitionReturn.nextState = 0;
        transitionReturn.tokenConcat = "";
    }
    else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.tokenConcat = token + c;
    }
    else if ((c >= '0' && c <= '9'))
    { // Numeric constants

        transitionReturn.nextState = 2;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c >= '-')
    { // Numeric constants

        transitionReturn.nextState = 3;
        transitionReturn.tokenConcat = token + c;
    }

    return transitionReturn;
}

/* Execute state 1 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateOneTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '1' && c <= '2') || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.tokenConcat = token + c;
    }
    else // End of variables and reserved words lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next token analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 2 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateTwoTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if ((c >= '0' && c <= '9')) // Numeric Constant
    {
        transitionReturn.nextState = 2;
        transitionReturn.tokenConcat = token + c;
    }
    else // End of Numeric Constants lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next token analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 3 transition actions
 * @param string token, char read toke
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateThreeTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if ((c >= '0' && c <= '9')) // Numeric Constant analysis
    {
        transitionReturn.nextState = 2;
        transitionReturn.tokenConcat = token + c;
    }
    else // Subtraction signal
    {
        // Returning a cursor position to avoid discarding valid characters for the next token analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token; // Discarting invalid char
    }

    return transitionReturn;
}

/* Lexical analyzer method
 * @return recognized token
 */
string lexicalAnalyzer()
{

    char c; // read character
    int state = 0;
    string token = "";

    while (state != finalState)
    {
        if (cursor != eof)
        {
            c = program[cursor++];

            if (validateChar(c))
            {
                // TODO: Erro
            }
        }
        else
        {
            // Flag EOF
            return token;
        }

        TransitionReturn tr;

        switch (state)
        {
        case 0:
            tr = stateZeroTransition(token, c);
            break;
        case 1:
            tr = stateOneTransition(token, c);
            break;
        case 2:
            tr = stateTwoTransition(token, c);
            break;
        case 3:
            tr = stateThreeTransition(token, c);
            break;

        default:
            break;
        }
        token = tr.tokenConcat;
        state = tr.nextState;
    }
    return token;
}

int main()
{
    string srcProgram;
    int line = 1;
    cursor = 0;

    ifstream programFile("program.txt");
    // if file is valid
    if (programFile)
    {
        // Read the complete program file
        ostringstream stream;
        stream << programFile.rdbuf(); // reading data
        program = stream.str();
    }

    // Setting the global variable to control eof
    eof = program.length();
    // Initalizing token with a char != of eof flag
    string token = " ";

    // Calling lexical analyzer while eof is not reached
    while (token != "")
    {
        token = lexicalAnalyzer();

        if (token != "")
        {
            cout << "Token: " << token << endl;
        }
    }

    return 0;
}