/**
 * @file Lcompiler.cpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS 
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

class Alphabet{
    public:
                      // Token                 Lexeme
        static const int CONST        = 0;  // (C | c)(O | o)(N | n)(S | s)(T | t)
        static const int INT          = 1;  // (I | i)(N | n)(T | t) 
        static const int CHAR         = 2;  // (C | c)(H | h)(A | a)(R | r)
        static const int WHILE        = 3;  // (W | w)(H | h)(I | i)(L | l)(E | e) 
        static const int IF           = 4;  // (I | i)(F | f)
        static const int FLOAT        = 5;  // (F | f)(L | l)(O | o)(A | a)(T | t)
        static const int ELSE         = 6;  // (E | e)(L | l)(S | s)(E | e)
        static const int AND          = 7;  // &&
        static const int OR           = 8;  // ||
        static const int NOT          = 9;  // !
        static const int ATRIB        = 10; // :=
        static const int EQUAL        = 11; // ==
        static const int OPENPAR      = 12; // (
        static const int CLOSEPAR     = 13; // )
        static const int LESSTHAN     = 14; // <
        static const int GREATERTHAN  = 15; // >
        static const int NOTEQUAL     = 16; // !=
        static const int GREATEREQUAL = 17; // >=
        static const int LESSEQUAL    = 18; // <=
        static const int COMMA        = 19; // ,
        static const int PLUS         = 20; // +
        static const int MINNUS       = 21; // -
        static const int TIMES        = 22; // *
        static const int DIVIDE       = 23; // /
        static const int SEMICOLON    = 24; // ;
        static const int OPENBRACE    = 25; // {
        static const int CLOSEBRACE   = 26; // }
        static const int READLN       = 27; // (R | r)(E | e)(A | a)(D | d)(L | l)(N |n)
        static const int DIV          = 28; // (D | d)(I | i)(V | v)
        static const int STRING       = 29; // (S | s)(T | t)(R | r)(I | i)(N | n)(G | g)
        static const int WRITE        = 30; // (W | w)(R | r)(I | i)(T | t)(E | e)
        static const int WRITELN      = 31; // (W | w)(R | r)(I | i)(T | t)(E | e)(L | l)(N | n)
        static const int MOD          = 32; // (M | m)(O | o)(D | d)
        static const int OPENBRACKET  = 33; // [
        static const int CLOSEBRACKET = 34; // ]
        static const int TRUE         = 35; // (T | t)(R | r)(U | u)(E | e)
        static const int FALSE        = 36; // (F | f)(A | a)(L | l)(S |s)(E | e)
        static const int BOOLEAN      = 37; // (B | b)(O | o)(O | o)(L | l)(E | e)(A | a)(N| n)
        static const int ID           = 40; // (l | _) (l | d | _) -> l = letra; d = dígito 
        static const int CONSTANT     = 41; // (('c') | ("(n)*") | (d+) | (d*.d+)) 
                                            // c -> valid and printable char
                                            // n -> valid char != /n
};


class ConstType{
    public:
        static const int INT          =  0;
        static const int CHAR         =  1; 
        static const int FLOAT        =  2; 
        static const int BOOLEAN      =  3; 
        static const int NOT_CONSTANT = -1; 
};

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

struct LexicalRegister{
    string lexeme;
    int token;
    int symbolTabPos;
    int constType;

};

struct TransitionReturn
{
    int nextState;
    string lexemeConcat;
    LexicalRegister lexicalReg;
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

/**
 * @brief Return the token of a lexeme that contains a single char
 *        Transition: Initial state - Final state (Reserved words)
 * @param char c
 * @return int token 
 */
int getStateZeroToken(char c){
    switch (c)
    {
        case '(':
            return Alphabet::OPENPAR;
        case ')':
            return Alphabet::CLOSEPAR;
        case ',':
            return Alphabet::COMMA;
        case '+':
            return Alphabet::PLUS;
        case '-':
            return Alphabet::MINNUS;
        case '*':
            return Alphabet::TIMES;
        case '/':
            return Alphabet::DIVIDE;
        case '{':
            return Alphabet::OPENBRACE;
        case '}':
            return Alphabet::CLOSEBRACE;
        case '[':
            return Alphabet::OPENBRACKET;
        case ']':
            return Alphabet::CLOSEBRACKET;
        case ';':
            return Alphabet::SEMICOLON;
        default:
            return -1;
            break;
    }
}

/** @brief Return the token of a lexeme that contains a single char
 *         Transition: Initial state - Final state (Reserved words)
 * @param char c
 * @return int token 
 */
LexicalRegister getLexicalRegister(string lexeme, int token, int symbolTabPos, int constType) {
    LexicalRegister l;
    l.lexeme = lexeme;
    l.token = token;
    l.symbolTabPos = symbolTabPos;
    l.constType = constType;
    return l;
}

/* Execute state 0 transition actions
 * @param string token, char read token
 * @return TransitionReturn -> next state and (token + c)
 */
TransitionReturn stateZeroTransition(string lexeme, char c)
{

    TransitionReturn transitionReturn;

    if (c == ' ' || c == '\n' || c == '\t') // Ignore insignificant char
    {
        transitionReturn.nextState = 0;
        transitionReturn.lexemeConcat = "";

        // increment line count on line break read
        if (c == '\n')
            line++;
    }
    else if (isCharacter(c) || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (isNumber(c) && c != '0')
    { // Numeric constants

        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '0')
    { // Numeric constants

        transitionReturn.nextState = 4;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '\'')
    {

        transitionReturn.nextState = 7;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '(' || c == ')' || c == ',' || c == '+' || c == '*' || c == ';' || c == '{' || c == '}' || c == '=' || c == '-' || c == '[' || c == ']')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Setting lexical register
        transitionReturn.lexicalReg = getLexicalRegister(transitionReturn.lexemeConcat, getStateZeroToken(c), symbolTable->search(lexeme), ConstType::NOT_CONSTANT);
    }
    else if (c == '<' || c == '>' || c == '!')
    {
        transitionReturn.nextState = 14;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == ':')
    {
        transitionReturn.nextState = 15;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '&')
    {
        transitionReturn.nextState = 16;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '|')
    {
        transitionReturn.nextState = 13;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '/')
    {
        transitionReturn.nextState = 10;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '\"')
    {
        transitionReturn.nextState = 9;
        transitionReturn.lexemeConcat = lexeme + c;
    }

    return transitionReturn;
}

/* Execute state 1 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state and (lexeme + c)
 */
TransitionReturn stateOneTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isCharacter(c) || isNumber(c) || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // End of variables and reserved words lexical analysis
    {
        int pos = symbolTable->search(lexeme);

        // lexeme not found in the symbol Table
        if (pos == null)
        {
            pos = symbolTable->insert(lexeme);
        }

        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarting invalid char

        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
    }

    return transitionReturn;
}

/* Execute state 2 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state and (lexeme + c)
 */
TransitionReturn stateTwoTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isNumber(c)) // Numeric Constant
    {
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '.') // Real number
    {
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // End of Numeric Constants lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 3 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state and (lexeme + c)
 */
TransitionReturn stateThreeTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isNumber(c)) // Numeric Constant analysis
    {
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // Subtraction signal
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 4 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state and (lexeme + c)
 */
TransitionReturn stateFourTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;
    if ((c == '.')) // Real number, initializing with 0
    {
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (isNumber(c)) // Numeric Constant analysis
    {
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == 'x') // Hexa Number analysis
    {
        transitionReturn.nextState = 5;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 5 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state and (lexeme + c)
 */
TransitionReturn stateFiveTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isHexa(c)) // Hexa Number analysis
    {
        transitionReturn.nextState = 6;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 6 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final state and (lexeme + c)
 */
TransitionReturn stateSixTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isHexa(c)) // End of Hexa Number analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 7 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state and (lexeme + c)
 */
TransitionReturn stateSevenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isCharacter(c)) // Character analysis
    {
        transitionReturn.nextState = 8;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 8 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final state and (lexeme + c)
 */
TransitionReturn stateEightTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '\'') // Character analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 9 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final state and (lexeme + c)
 */
TransitionReturn stateNineTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c != '\"' && c != '\n') // String analysis
    {
        transitionReturn.nextState = 9;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '\"')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 10 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final state and final lexeme
 */
TransitionReturn stateTenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '*') // comments analysis
    {
        transitionReturn.nextState = 11;
        transitionReturn.lexemeConcat = "";
    }
    else if (c != '*') // it´s a division symbol
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 11 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> next state
 */
TransitionReturn stateElevenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c != '*')
    {
        transitionReturn.nextState = 11;
    }
    else if (c == '*')
    {
        transitionReturn.nextState = 12;
    }

    return transitionReturn;
}

/* Execute state 12 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final  or next state
 */
TransitionReturn stateTwelveTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '/')
    {
        transitionReturn.nextState = finalState;
    }
    else if (c == '*')
    {
        transitionReturn.nextState = 12;
    }
    else if (c != '*' || c != '/')
    {
        transitionReturn.nextState = 11;
    }

    return transitionReturn;
}

/* Execute state 13 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final  or next state and (lexeme + c)
 */
TransitionReturn stateThirteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '|') // it´s a operator OU
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 14 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final  or next state and lexeme or lexeme + c
 */
TransitionReturn stateFourteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '=')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c != '=')
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarting invalid char
    }

    return transitionReturn;
}

/* Execute state 15 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final  or next state and lexeme or lexeme + c
 */
TransitionReturn stateFifteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '=')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Execute state 16 transition actions
 * @param string lexeme, char read lexeme
 * @return TransitionReturn -> final  or next state and lexeme or lexeme + c
 */
TransitionReturn stateSixteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '&')
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/* Lexical analyzer method
 * @return recognized lexeme
 */
string lexicalAnalyzer()
{
    char c; // read character
    int state = 0;
    string lexeme = "";

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
            return lexeme;
        }

        TransitionReturn tr;

        switch (state)
        {
            case 0:
                tr = stateZeroTransition(lexeme, c);
                break;
            case 1:
                tr = stateOneTransition(lexeme, c);
                break;
            case 2:
                tr = stateTwoTransition(lexeme, c);
                break;
            case 3:
                tr = stateThreeTransition(lexeme, c);
                break;
            case 4:
                tr = stateFourTransition(lexeme, c);
                break;
            case 5:
                tr = stateFiveTransition(lexeme, c);
                break;
            case 6:
                tr = stateSixTransition(lexeme, c);
                break;
            case 7:
                tr = stateSevenTransition(lexeme, c);
                break;
            case 8:
                tr = stateEightTransition(lexeme, c);
                break;
            case 9:
                tr = stateNineTransition(lexeme, c);
                break;
            case 10:
                tr = stateTenTransition(lexeme, c);
                break;
            case 11:
                tr = stateElevenTransition(lexeme, c);
                break;
            case 12:
                tr = stateTwelveTransition(lexeme, c);
                break;
            case 13:
                tr = stateThirteenTransition(lexeme, c);
                break;
            case 14:
                tr = stateFourteenTransition(lexeme, c);
                break;
            case 15:
                tr = stateFifteenTransition(lexeme, c);
                break;
            case 16:
                tr = stateSixteenTransition(lexeme, c);
                break;

            default:
                break;
        }
        lexeme = tr.lexemeConcat;
        state = tr.nextState;
    }

    return lexeme;
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

    // Initializing lexeme with a char != of eof flag
    string lexeme = "";

    // Calling lexical analyzer while eof is not reached
    while (cursor != eof)
    {
        lexeme = lexicalAnalyzer();

        // if (token != "")
        // {
        //     cout << "Token: " << token << endl;
        // }
    }

    // symbolTable->print();
    line == 1 ? (cout << "1 linha compilada.") : (cout << line << " linhas compiladas.");

    return 0;
}