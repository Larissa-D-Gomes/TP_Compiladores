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

string testLexem(int n){
    if(n == 0) return "const";
    else if(n == 1) return "int";
    else if(n == 2) return "char";
    else if(n == 3) return "while";
    else if(n == 4) return "if";
    else if(n == 5) return "float";
    else if(n == 6) return "else";
    else if(n == 7) return "&&";
    else if(n == 8) return "||";
    else if(n == 9) return "!";
    else if(n == 10) return ":=";
    else if(n == 11) return "=";
    else if(n == 12) return "(";
    else if(n == 13) return ")";
    else if(n == 14) return "<";
    else if(n == 15) return ">";
    else if(n == 16) return "!=";
    else if(n == 17) return ">=";
    else if(n == 18) return "<=";
    else if(n == 19) return ",";
    else if(n == 20) return "+";
    else if(n == 21) return "-";
    else if(n == 22) return "*";
    else if(n == 23) return "/";
    else if(n == 24) return ";";
    else if(n == 25) return "{";
    else if(n == 26) return "}";
    else if(n == 27) return "readl";
    else if(n == 28) return "div";
    else if(n == 29) return "string";
    else if(n == 30) return "write";
    else if(n == 31) return "writeln";
    else if(n == 32) return "mod";
    else if(n == 33) return "[";
    else if(n == 34) return "]";
    else if(n == 35) return "true";
    else if(n == 36) return "false";
    else if(n == 37) return "boolean";
    else if(n == 40) return "id";
    else if(n == 41) return "constant";

    return "";
}

class Alphabet
{
public:
    // Token                 Lexeme
    static const int CONST = 0;         // (C | c)(O | o)(N | n)(S | s)(T | t)
    static const int INT = 1;           // (I | i)(N | n)(T | t)
    static const int CHAR = 2;          // (C | c)(H | h)(A | a)(R | r)
    static const int WHILE = 3;         // (W | w)(H | h)(I | i)(L | l)(E | e)
    static const int IF = 4;            // (I | i)(F | f)
    static const int FLOAT = 5;         // (F | f)(L | l)(O | o)(A | a)(T | t)
    static const int ELSE = 6;          // (E | e)(L | l)(S | s)(E | e)
    static const int AND = 7;           // &&
    static const int OR = 8;            // ||
    static const int NOT = 9;           // !
    static const int ATRIB = 10;        // :=
    static const int EQUAL = 11;        // =
    static const int OPENPAR = 12;      // (
    static const int CLOSEPAR = 13;     // )
    static const int LESSTHAN = 14;     // <
    static const int GREATERTHAN = 15;  // >
    static const int NOTEQUAL = 16;     // !=
    static const int GREATEREQUAL = 17; // >=
    static const int LESSEQUAL = 18;    // <=
    static const int COMMA = 19;        // ,
    static const int PLUS = 20;         // +
    static const int MINNUS = 21;       // -
    static const int TIMES = 22;        // *
    static const int DIVIDE = 23;       // /
    static const int SEMICOLON = 24;    // ;
    static const int OPENBRACE = 25;    // {
    static const int CLOSEBRACE = 26;   // }
    static const int READLN = 27;       // (R | r)(E | e)(A | a)(D | d)(L | l)(N |n)
    static const int DIV = 28;          // (D | d)(I | i)(V | v)
    static const int STRING = 29;       // (S | s)(T | t)(R | r)(I | i)(N | n)(G | g)
    static const int WRITE = 30;        // (W | w)(R | r)(I | i)(T | t)(E | e)
    static const int WRITELN = 31;      // (W | w)(R | r)(I | i)(T | t)(E | e)(L | l)(N | n)
    static const int MOD = 32;          // (M | m)(O | o)(D | d)
    static const int OPENBRACKET = 33;  // [
    static const int CLOSEBRACKET = 34; // ]
    static const int TRUE = 35;         // (T | t)(R | r)(U | u)(E | e)
    static const int FALSE = 36;        // (F | f)(A | a)(L | l)(S |s)(E | e)
    static const int BOOLEAN = 37;      // (B | b)(O | o)(O | o)(L | l)(E | e)(A | a)(N| n)
    static const int ID = 40;           // (l | _) (l | d | _) -> l = letra; d = dígito
    static const int CONSTANT = 41;     // (('c') | ("(n)*") | (d+) | (d*.d+))
                                        // c -> valid and printable char
                                        // n -> valid char != /n
};

class ConstType
{
public:
    static const int INT = 0;
    static const int CHAR = 1;
    static const int FLOAT = 2;
    static const int BOOLEAN = 3;
    static const int STRING = 4;
    static const int NOT_CONSTANT = -1;
};

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

        // fill reserved words list using a iterator
        // list position = alphabet byte value
        for (uint8_t i = 0; i < reservedWords.size(); i++)
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
        // If map does not contain lex
        if (this->symbolTable.find(lex) == this->symbolTable.end())
            return null;

        // Return token of lex
        return this->symbolTable.at(lex);
    }

    /**
     * @brief Find for the lexical form in the Symbol Table
     *
     * @param lex the lexical form that you want to get position
     * @return int index of the lexical form on the hash table
     */
    int find(string lex)
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
    int insert(string lex)
    {
        this->symbolTable[lex] = Alphabet::ID;
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

class LexicalRegister
{
public:
    string lexeme;
    int token;
    int symbolTabPos;
    int constType;

    LexicalRegister() {}

    LexicalRegister(string lexeme, int token, int symbolTabPos, int constType)
    {
        this->lexeme = lexeme;
        this->token = token;
        this->symbolTabPos = symbolTabPos;
        this->constType = constType;
    }
};

/**
 * @brief Control the transitions of the lexical analyzer automaton
 */
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
// Flag last read token is \n 
bool lastTokenBreakLine = false;

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

/**
 * @brief Valid a char read
 *
 * @param c - character to valid
 * @return true - if is valid
 * @return false - if is not valid
 */
bool isValidChar(char c)
{
    return ((c >= ' ' && c <= '"') || (c >= 'A' && c <= ']') || c == '/' || c == '_' || (c >= 'a' && c <= '}') || (c >= '%' && c <= '?')) || (c == '@') || c == '\n' || c == '\r' || c == '\t';
}

/**
 * @brief Print a error message when program read a invalid character
 *
 */
void throwInvalidCharacterException()
{
    cout << line << "\ncaractere invalido."  << endl;
    exit(1);
}

/**
 * @brief Print a error message when program read an unexpected EOF
 *
 */
void throwUnexpectedEOFException()
{
    if(lastTokenBreakLine)
    {
        cout << lastTokenBreakLine << endl;
        line--;
    }
    cout << line << "\nfim de arquivo nao esperado.";
    exit(1);
}

/**
 * @brief Print a error message when program read a invalid lexeme
 *
 */
void throwUndefinedLex(string lex)
{
    if (lex.at(lex.length() - 1) == '\n') lex.pop_back();
    cout << line << "\nlexema nao identificado [" << lex << "].";
    exit(1);
}

/**
 * @brief Print a error message when program finds a unexpected token
 *
 */
void throwUnexpectedToken(string lex)
{
    cout << line << "\ntoken nao esperado [" << lex << "].";
    exit(1);
}

/**
 * @brief Execute state 0 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateZeroTransition(string lexeme, char c)
{

    TransitionReturn transitionReturn;

    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') // Ignore insignificant char
    {
        transitionReturn.nextState = 0;
        transitionReturn.lexemeConcat = "";

        // Increment line count on line break read
        if (c == '\n'){
            line++;
        }
    }
    else if (isCharacter(c) || c == '_') // Variables and reserved words
    {
        transitionReturn.nextState = 1;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (isNumber(c) && c != '0') // Numeric constants
    {
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '0') // Numeric constants
    {
        transitionReturn.nextState = 4;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '\'') // Character constants
    {
        transitionReturn.nextState = 7;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '\"') // String constants
    {
        transitionReturn.nextState = 9;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '/') // Div operator / Comments
    {
        transitionReturn.nextState = 10;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '|') // Logic OR Operator
    {
        transitionReturn.nextState = 13;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '<' || c == '>' || c == '!') // Compost operators
    {
        transitionReturn.nextState = 14;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == ':') // Attribution operator
    {
        transitionReturn.nextState = 15;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '&') // Logic AND Operator
    {
        transitionReturn.nextState = 16;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '(' || c == ')' || c == ',' || c == '+' || c == '*' || c == ';' || c == '{' || c == '}' || c == '=' || c == '-' || c == '[' || c == ']') // Simple token operators symbols
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        //  Create lexical register to SIMPLE OPERATOR SYMBOLS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else if (c == '.')
    {
        transitionReturn.nextState = 17;
        transitionReturn.lexemeConcat = lexeme + c;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 1 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
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
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding last read char

        int pos = symbolTable->find(lexeme);

        // Lexeme not found in the symbol Table
        if (pos == null)
        {
            symbolTable->insert(lexeme);
            pos = symbolTable->find(lexeme);

            // Create lexical register to IDENTIFIER
            LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::ID, pos, ConstType::NOT_CONSTANT);
            transitionReturn.lexicalReg = lexicalRegister;
        }
        else
        {
            // Get token and insert if lexeme is an id
            int token = symbolTable->search(transitionReturn.lexemeConcat);

            if (token == Alphabet::TRUE || token == Alphabet::FALSE) // Reserved word constant TRUE or FALSE
            {
                // Create lexical register to TRUE and FALSE
                LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, token, pos, ConstType::BOOLEAN);
                transitionReturn.lexicalReg = lexicalRegister;
            }
            else // Is an identifier
            {
                // Create lexical register to RESERVED WORD
                LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, token, pos, ConstType::NOT_CONSTANT);
                transitionReturn.lexicalReg = lexicalRegister;
            }
        }

        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 2 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateTwoTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isNumber(c)) // Numeric constant
    {
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '.') // Real number constant
    {
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // End of Integer Numeric Constants lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to INT CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::INT);
        transitionReturn.lexicalReg = lexicalRegister;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 3 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateThreeTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isNumber(c)) // Numeric Constant analysis
    {
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else //  End of Float Numeric Constants lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to FLOAT CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::FLOAT);
        transitionReturn.lexicalReg = lexicalRegister;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 4 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFourTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if ((c == '.')) // Real number
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
    else // Throw exception when Hexa Number was not identified
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to INT CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::INT);
        transitionReturn.lexicalReg = lexicalRegister;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 5 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFiveTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isHexa(c)) // Hexa Number analysis
    {
        transitionReturn.nextState = 6;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // Throw exception when Hexa Number was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 6 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSixTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isHexa(c)) // End of Hexa Number analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to HEXA NUMBER CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::CHAR);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when Hexa Number was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 7 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSevenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isCharacter(c)) // Character analysis
    {
        transitionReturn.nextState = 8;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // Throw exception when character was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 8 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateEightTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '\'') // End of Character Constants lexical analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to CHAR CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, -1, ConstType::CHAR);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when character was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 9 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateNineTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c != '\"' && c != '\n') // String analysis
    {
        transitionReturn.nextState = 9;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else if (c == '\"') // End of String Constants lexical analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to STRING CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::STRING);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when string was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 10 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateTenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '*') // Comments analysis
    {
        transitionReturn.nextState = 11;
        transitionReturn.lexemeConcat = "";
    }
    else if (c != '*') // End of Divisor operator lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to DIVIDE
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 11 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateElevenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c != '*') // Checks if it's a loop of comment
    {
        transitionReturn.nextState = 11;

        // Increment line count on line break read
        if (c == '\n')
            line++;

        // If we have a unexpected eof
        if (cursor == eof)
            throwUnexpectedEOFException();
    }
    else if (c == '*') // Comment analysis
    {
        transitionReturn.nextState = 12;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 12 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateTwelveTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '/') // Checks if it's a loop of comment
    {
        transitionReturn.nextState = 0;
    }
    else if (c == '*') // Stay in comment loop
    {
        transitionReturn.nextState = 12;

        // If we have a unexpected eof
        if (cursor == eof)
            throwUnexpectedEOFException();
    }
    else if (c != '*' && c != '/') // Returns to previous state of comment
    {
        transitionReturn.nextState = 11;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 13 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateThirteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '|') // End of OR Logic lexical analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // create lexical register to OR LOGIC
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when logical operator (OR - ||) was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 14 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFourteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '=') // End of Compost Logical Operators lexical analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to COMPOST LOGICAL OPERATORS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else if (c != '=') // End of Simple logical operators lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to SIMPLE LOGICAL OPERATORS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }

    return transitionReturn;
}

/**
 * @brief Execute state 15 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFifteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '=') // End of Atribute Operator lexical analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when assignment command was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 16 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSixteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (c == '&') // End of AND logic lexical analysis
    {
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to AND LOGIC
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when logical operator (AND - &&) was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 17 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSeventeenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;
    if (isNumber(c)) // Float number that begins with '.'
    {
        transitionReturn.nextState = 18;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else // Throw exception when logical operator (AND - &&) was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme);
    }

    return transitionReturn;
}

/**
 * @brief Execute state 18 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateEighteenTransition(string lexeme, char c)
{
    TransitionReturn transitionReturn;

    if (isNumber(c)) // Float number that begins with '.'
    {
        transitionReturn.nextState = 18;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    else
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to DIVIDE
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::FLOAT);
        transitionReturn.lexicalReg = lexicalRegister;
    }

    return transitionReturn;
}

/**
 * @brief Lexical analyzer method
 *
 * @return string - recognized token
 */
LexicalRegister lexicalAnalyzer()
{
    char c = ' '; // read character
    int state = 0;
    string lexeme = "";

    TransitionReturn tr;
    while (state != finalState)
    {
        if (cursor != eof)
        {
            c = tolower(program[cursor++]);
         
            if (!isValidChar(c))
            {
                throwInvalidCharacterException();
            }
        }
        else
        {
            // Flag EOF
            return LexicalRegister("", null, null, null);
        }

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
        case 17:
            tr = stateSeventeenTransition(lexeme, c);
            break;
        case 18:
            tr = stateEighteenTransition(lexeme, c);
            break;

        default:
            break;
        }

        lexeme = tr.lexemeConcat;
        state = tr.nextState;
    }
    //cout << tr.lexicalReg.lexeme << endl;
    return tr.lexicalReg;
}

class SyntaxAnalyzer
{

private:
    LexicalRegister tokenFromLexical;
    int token;

public:
    SyntaxAnalyzer()
    {
        // Initializing lexeme with a char != of eof flag
        this->tokenFromLexical = LexicalRegister("", null, null, null);
    }

    void matchToken(int expectedToken)
    {

       // cout << "1043: AnalisadorLexico: [" << testLexem(this->token)  << "] Esperado na Gramatica: [" << testLexem(expectedToken) << "]" << endl;

        if (this->token == expectedToken)
        {
            this->tokenFromLexical = lexicalAnalyzer();
            this->token = this->tokenFromLexical.token;
        }
        else
        {
            if (cursor == eof)
            {
                line--;
                throwUnexpectedEOFException();
            }
            else
                throwUnexpectedToken(this->tokenFromLexical.lexeme);
        }
    }

    bool checkFirstDEC()
    {
        return this->token == Alphabet::INT ||
               this->token == Alphabet::FLOAT ||
               this->token == Alphabet::STRING ||
               this->token == Alphabet::BOOLEAN ||
               this->token == Alphabet::CHAR ||
               this->token == Alphabet::CONST ||
               this->token == Alphabet::ID;
    }

    bool checkFirstCMD()
    {
        return this->token == Alphabet::WHILE ||
               this->token == Alphabet::IF ||
               this->token == Alphabet::SEMICOLON ||
               this->token == Alphabet::READLN ||
               this->token == Alphabet::WRITE ||
               this->token == Alphabet::WRITELN;
    }

    bool checkFirstT()
    {
        return checkFirstR();
    }

    bool checkFirstR()
    {
        return checkFirstM();
    }

    bool checkFirstM()
    {
        return this->token == Alphabet::NOT ||
               this->token == Alphabet::INT ||
               this->token == Alphabet::FLOAT ||
               this->token == Alphabet::ID ||
               this->token == Alphabet::CONSTANT ||
               this->token == Alphabet::OPENPAR || 
               this->token == Alphabet::TRUE ||
               this->token == Alphabet::FALSE;
    }

    void S()
    {
        while (checkFirstDEC() || checkFirstCMD())
        {
            if (checkFirstDEC())
            {
                DEC();
                matchToken(Alphabet::SEMICOLON);
            }
            else if (checkFirstCMD())
            {
                CMD();
            }
        }
    }

    void DEC()
    {
        if (this->token == Alphabet::INT ||
            this->token == Alphabet::FLOAT ||
            this->token == Alphabet::STRING ||
            this->token == Alphabet::BOOLEAN ||
            this->token == Alphabet::CHAR)
        {
            if (this->token == Alphabet::INT) // INT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                matchToken(Alphabet::INT);
            }
            else if (this->token == Alphabet::FLOAT) // FLOAT ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                matchToken(Alphabet::FLOAT);
            }
            else if (this->token == Alphabet::STRING) // STRING ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                matchToken(Alphabet::STRING);
            }
            else if (this->token == Alphabet::BOOLEAN) // BOOLEAN ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                matchToken(Alphabet::BOOLEAN);
            }
            else if (this->token == Alphabet::CHAR) // CHAR ID [:= [-]CONSTANT] {, ID [:= [-]CONSTANT]}*
            {
                matchToken(Alphabet::CHAR);
            }

            matchToken(Alphabet::ID);

            if (this->token == Alphabet::ATRIB)
            {
                matchToken(Alphabet::ATRIB);
                DECONST();
            }

            while (this->token == Alphabet::COMMA)
            {
                matchToken(Alphabet::COMMA);
                matchToken(Alphabet::ID);
                if (this->token == Alphabet::ATRIB)
                {
                    matchToken(Alphabet::ATRIB);
                    DECONST();
                }
            }
        }
        else if (this->token == Alphabet::CONST) // CONST ID = [-]CONSTANT
        {
            matchToken(Alphabet::CONST);
            matchToken(Alphabet::ID);
            matchToken(Alphabet::EQUAL);
            DECONST();
        }
        else 
        {
            ATR();
        }
    }

    void ATR(){
        matchToken(Alphabet::ID);

        if (this->token == Alphabet::OPENBRACKET)
        {
            matchToken(Alphabet::OPENBRACKET);
            EXP();
            matchToken(Alphabet::CLOSEBRACKET);
        }

        matchToken(Alphabet::ATRIB);
        EXP();
    }

    void DECONST()
    {
        if (this->token == Alphabet::MINNUS) // - CONSTANT
        {
            matchToken(Alphabet::MINNUS);
        }
        if(this->token == Alphabet::CONSTANT)
        {
            matchToken(Alphabet::CONSTANT); // CONSTANT
        } 
        else if(this->token == Alphabet::TRUE)
        {
            matchToken(Alphabet::TRUE); // CONSTANT
        } 
        else 
        {
            matchToken(Alphabet::FALSE); // CONSTANT
        }
    }

    void CMD() // Language commands
    {
        if (this->token == Alphabet::WHILE) // WHILE(){}
        {
            matchToken(Alphabet::WHILE);
            PAR();
            BLOCK();
        }
        else if (this->token == Alphabet::IF) // IF() [ELSE]
        {
            matchToken(Alphabet::IF);
            PAR();
            BLOCK();
            if (this->token == Alphabet::ELSE)
            {
                matchToken(Alphabet::ELSE);
                BLOCK();
            }
        }
        else if (this->token == Alphabet::SEMICOLON) // ;
        {
            matchToken(Alphabet::SEMICOLON);
        }
        else if (this->token == Alphabet::READLN) // READLN(ID)
        {
            matchToken(Alphabet::READLN);
            matchToken(Alphabet::OPENPAR);
            matchToken(Alphabet::ID);
            matchToken(Alphabet::CLOSEPAR);
        }
        else if (this->token == Alphabet::WRITE || this->token == Alphabet::WRITELN) // (WRITE | WRITELN)(EXP {, EXP}*)
        {
            if (this->token == Alphabet::WRITE) // WRITE(EXP {, EXP}*)
            {
                matchToken(Alphabet::WRITE);
            }
            else if (this->token == Alphabet::WRITELN) // WRITELN(EXP {, EXP}*)
            {
                matchToken(Alphabet::WRITELN);
            }

            matchToken(Alphabet::OPENPAR);
            EXP();

            while (this->token == Alphabet::COMMA)
            {
                matchToken(Alphabet::COMMA);
                EXP();
            };

            matchToken(Alphabet::CLOSEPAR);
        }
    }

    void PAR()
    {
        matchToken(Alphabet::OPENPAR);
        EXP();
        matchToken(Alphabet::CLOSEPAR);
    }

    void BLOCK()
    {
        if (checkFirstCMD())
        {
            CMD();
            matchToken(Alphabet::SEMICOLON);
        } 
        else if(this->token == Alphabet::ID) 
        {
            ATR();
            matchToken(Alphabet::SEMICOLON);
        } 
        else if (this->token == Alphabet::OPENBRACE)
        {
            matchToken(Alphabet::OPENBRACE);

            while (checkFirstCMD() || this->token == Alphabet::ID)
            {
                if (checkFirstCMD())
                {
                    CMD();
                    matchToken(Alphabet::SEMICOLON);
                } 
                else if(this->token == Alphabet::ID) 
                {
                    ATR();
                    matchToken(Alphabet::SEMICOLON);
                } 
            };

            matchToken(Alphabet::CLOSEBRACE);
        }
    }

    void EXP()
    {
       // if (checkFirstT())
        //{
            T();
            while (this->token == Alphabet::EQUAL ||
                   this->token == Alphabet::NOTEQUAL ||
                   this->token == Alphabet::LESSTHAN ||
                   this->token == Alphabet::GREATERTHAN ||
                   this->token == Alphabet::LESSEQUAL ||
                   this->token == Alphabet::GREATEREQUAL)
            {
                if (this->token == Alphabet::EQUAL)
                {
                    matchToken(Alphabet::EQUAL);
                }
                else if (this->token == Alphabet::NOTEQUAL)
                {
                    matchToken(Alphabet::NOTEQUAL);
                }
                else if (this->token == Alphabet::LESSTHAN)
                {
                    matchToken(Alphabet::LESSTHAN);
                }
                else if (this->token == Alphabet::GREATERTHAN)
                {
                    matchToken(Alphabet::GREATERTHAN);
                }
                else if (this->token == Alphabet::LESSEQUAL)
                {
                    matchToken(Alphabet::LESSEQUAL);
                }
                else if (this->token == Alphabet::GREATEREQUAL)
                {
                    matchToken(Alphabet::GREATEREQUAL);
                }
                T();
            }
        //}
    }

    void T()
    {

        if (this->token == Alphabet::PLUS)
        {
            matchToken(Alphabet::PLUS);
        }
        else if (this->token == Alphabet::MINNUS)
        {
            matchToken(Alphabet::MINNUS);
        }

        R();

        while (this->token == Alphabet::PLUS || this->token == Alphabet::MINNUS || this->token == Alphabet::OR)
        {
            if (this->token == Alphabet::PLUS)
            {
                matchToken(Alphabet::PLUS);
            }
            else if (this->token == Alphabet::MINNUS)
            {
                matchToken(Alphabet::MINNUS);
            }
            else if (this->token == Alphabet::OR)
            {
                matchToken(Alphabet::OR);
            }
            R();
        }
    }

    void R()
    {
        M();

        while (this->token == Alphabet::TIMES ||
               this->token == Alphabet::DIV ||
               this->token == Alphabet::DIVIDE ||
               this->token == Alphabet::AND ||
               this->token == Alphabet::MOD)
        {
            if (this->token == Alphabet::TIMES)
            {
                matchToken(Alphabet::TIMES);
            }
            else if (this->token == Alphabet::DIV)
            {
                matchToken(Alphabet::DIV);
            }
            else if (this->token == Alphabet::DIVIDE)
            {
                matchToken(Alphabet::DIVIDE);
            }
            else if (this->token == Alphabet::AND)
            {
                matchToken(Alphabet::AND);
            }
            else if (this->token == Alphabet::MOD)
            {
                matchToken(Alphabet::MOD);
            }
            M();
        }
    }

    void M()
    {
        if (this->token == Alphabet::NOT)
        {
            matchToken(Alphabet::NOT);
            M();
        }
        else if (this->token == Alphabet::INT || this->token == Alphabet::FLOAT)
        {
            if (this->token == Alphabet::INT)
            {
                matchToken(Alphabet::INT);
            }
            else if (this->token == Alphabet::FLOAT)
            {
                matchToken(Alphabet::FLOAT);
            }
            matchToken(Alphabet::OPENPAR);
            EXP();
            matchToken(Alphabet::CLOSEPAR);
        }
        else if (this->token == Alphabet::ID)
        {
            matchToken(Alphabet::ID);
            if (this->token == Alphabet::OPENBRACKET)
            {
                matchToken(Alphabet::OPENBRACKET);
                EXP();
                matchToken(Alphabet::CLOSEBRACKET);
            }
        }
        else if (this->token == Alphabet::CONSTANT)
        {
            matchToken(Alphabet::CONSTANT);
        }
        else if (this->token == Alphabet::TRUE)
        {
            matchToken(Alphabet::TRUE);
        } 
        else if (this->token == Alphabet::FALSE)
        {
            matchToken(Alphabet::FALSE);
        }
        else 
        {
            matchToken(Alphabet::OPENPAR);
            EXP();
            matchToken(Alphabet::CLOSEPAR);
        }
    }

    void parser()
    {

        this->tokenFromLexical = lexicalAnalyzer();
        this->token = this->tokenFromLexical.token;

        S();

    }
};

int main()
{
    program = "";
    string str;
    line = 1;
    cursor = 0;

    // Read source program
    while (getline(cin, str))
    {
        program += str + '\n';
    }
    //cout << program << endl;

    // TODO? Quando acabar as entregas do TP1 e TP2 descomentar essa parte - para apresentação
    // // remove last \n
    //program.pop_back();

    // Setting the global variable to control eof
    eof = program.length();

    SyntaxAnalyzer syntaxAnalyzer;
    syntaxAnalyzer.parser();

    // Print line count and success compilation
    line == 1 ? (cout << "1 linha compilada." << endl) : (cout << line << " linhas compiladas."  << endl);

    return 0;
}