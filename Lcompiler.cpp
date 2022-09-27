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
     * @brief Fill Symbol Table (only called at the begin of the program)
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
            return null;
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
            cout << "{ " << pair.second << " : " << pair.first << " }\n";
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
// Lines of the program
int line;

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

void throwInvalidCharacterException()
{
    cout << line << "\ncaractere invalido.";
    exit(1);
}

void throwUnexpectedEOFException()
{
    cout << line << "\nfim de arquivo nao esperado.";
    exit(1);
}

void throwUndefinedLex(string lex)
{
    lex.pop_back();
    cout << line << "\nlexema nao identificado [" << lex << "].";
    exit(1);
}

bool isValidChar(char c)
{
    return ((c >= ' ' && c <= '"') || (c >= 'A' && c <= ']') || c == '/' || c == '_' || (c >= 'a' && c <= '}') || (c >= '%' && c <= '?')) || (c == '@') || c == '\n' || c == '\r' || c == '\t';
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

        // increment line count on line break read
        if (c == '\n')
            line++;
    }
    else if (isCharacter(c) || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.tokenConcat = token + c;
    }
    else if (isNumber(c) && c != '0')
    { // Numeric constants

        transitionReturn.nextState = 2;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '0')
    { // Numeric constants

        transitionReturn.nextState = 4;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '\'')
    {

        transitionReturn.nextState = 7;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '(' || c == ')' || c == ',' || c == '+' || c == '*' || c == ';' || c == '{' || c == '}' || c == '=' || c == '-' || c == '[' || c == ']')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '<' || c == '>' || c == '!')
    {
        transitionReturn.nextState = 14;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == ':')
    {
        transitionReturn.nextState = 15;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '&')
    {
        transitionReturn.nextState = 16;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '|')
    {
        transitionReturn.nextState = 13;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '/')
    {
        transitionReturn.nextState = 10;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '\"')
    {
        transitionReturn.nextState = 9;
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

    if (isCharacter(c) || isNumber(c) || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.tokenConcat = token + c;
    }
    else // End of variables and reserved words lexical analysis
    {
        int pos = symbolTable->search(token);

        // token not found in the symbol Table
        if (pos == null)
        {
            pos = symbolTable->insert(token);
        }

        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token; // Discarting invalid char

        // Returning a cursor position to avoid discarding valid characters for the next token analysis
        cursor--;
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

    if (isNumber(c)) // Numeric Constant
    {
        transitionReturn.nextState = 2;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '.') // Real number
    {
        transitionReturn.nextState = 3;
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
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateThreeTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (isNumber(c)) // Numeric Constant analysis
    {
        transitionReturn.nextState = 3;
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

/* Execute state 4 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateFourTransition(string token, char c)
{
    TransitionReturn transitionReturn;
    if ((c == '.')) // Real number, initializing with 0
    {
        transitionReturn.nextState = 3;
        transitionReturn.tokenConcat = token + c;
    }
    else if (isNumber(c)) // Numeric Constant analysis
    {
        transitionReturn.nextState = 2;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == 'x') // Hexa Number analysis
    {
        transitionReturn.nextState = 5;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 5 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateFiveTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (isHexa(c)) // Hexa Number analysis
    {
        transitionReturn.nextState = 6;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 6 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final state and (token + c)
 */
TransitionReturn stateSixTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (isHexa(c)) // End of Hexa Number analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 7 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateSevenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (isCharacter(c)) // Character analysis
    {
        transitionReturn.nextState = 8;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 8 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final state and (token + c)
 */
TransitionReturn stateEightTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '\'') // Character analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 9 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final state and (token + c)
 */
TransitionReturn stateNineTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c != '\"' && c != '\n') // String analysis
    {
        transitionReturn.nextState = 9;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c == '\"')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 10 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final state and final token
 */
TransitionReturn stateTenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '*') // comments analysis
    {
        transitionReturn.nextState = 11;
        transitionReturn.tokenConcat = "";
    }
    else if (c != '*') // it´s a division symbol
    {
        // Returning a cursor position to avoid discarding valid characters for the next token analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 11 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state
 */
TransitionReturn stateElevenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c != '*')
    {
        transitionReturn.nextState = 11;

        if (c == '\n')
            line++;

        if (cursor == eof)
            throwUnexpectedEOFException();
    }
    else if (c == '*')
    {
        transitionReturn.nextState = 12;
    }

    return transitionReturn;
}

/* Execute state 12 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final  or next state
 */
TransitionReturn stateTwelveTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '/')
    {
        transitionReturn.nextState = finalState;
    }
    else if (c == '*')
    {
        transitionReturn.nextState = 12;
        if (cursor == eof)
            throwUnexpectedEOFException();
    }
    else if (c != '*' && c != '/')
    {
        transitionReturn.nextState = 11;

    }

    return transitionReturn;
}

/* Execute state 13 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final  or next state and (token + c)
 */
TransitionReturn stateThirteenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '|') // it´s a operator OU
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 14 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final  or next state and token or token + c
 */
TransitionReturn stateFourteenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '=')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else if (c != '=')
    {
        // Returning a cursor position to avoid discarding valid characters for the next token analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 15 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final  or next state and token or token + c
 */
TransitionReturn stateFifteenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '=')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
    }

    return transitionReturn;
}

/* Execute state 16 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> final  or next state and token or token + c
 */
TransitionReturn stateSixteenTransition(string token, char c)
{
    TransitionReturn transitionReturn;

    if (c == '&')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.tokenConcat = token + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(token + c);
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
            if (!isValidChar(c))
            {
                throwInvalidCharacterException();
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
        case 4:
            tr = stateFourTransition(token, c);
            break;
        case 5:
            tr = stateFiveTransition(token, c);
            break;
        case 6:
            tr = stateSixTransition(token, c);
            break;
        case 7:
            tr = stateSevenTransition(token, c);
            break;
        case 8:
            tr = stateEightTransition(token, c);
            break;
        case 9:
            tr = stateNineTransition(token, c);
            break;
        case 10:
            tr = stateTenTransition(token, c);
            break;
        case 11:
            tr = stateElevenTransition(token, c);
            break;
        case 12:
            tr = stateTwelveTransition(token, c);
            break;
        case 13:
            tr = stateThirteenTransition(token, c);
            break;
        case 14:
            tr = stateFourteenTransition(token, c);
            break;
        case 15:
            tr = stateFifteenTransition(token, c);
            break;
        case 16:
            tr = stateSixteenTransition(token, c);
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