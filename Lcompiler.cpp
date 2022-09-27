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
        {
            return got->second;
        }
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

class LexicalRegister{
    public:
        string lexeme;
        int token;
        int symbolTabPos;
        int constType;

        LexicalRegister(){}
        
        LexicalRegister(string lexeme, int token, int symbolTabPos, int constType){
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
    cout << line << "\ncaractere invalido.";
    exit(1);
}

/**
 * @brief Print a error message when program read an unexpected EOF
 *
 */
void throwUnexpectedEOFException()
{
    cout << line << "\nfim de arquivo nao esperado.";
    exit(1);
}

/**
 * @brief Print a error message when program read a invalid lexeme
 *
 */
void throwUndefinedLex(string lex)
{
    lex.pop_back();
    cout << line << "\nlexema nao identificado [" << lex << "].";
    exit(1);
}

/**
 * @brief Return the reserved work token of a lexeme
 * @param string lexeme
 * @return int token
 */
int getReservedWordToken(string lexeme)
{
    if (lexeme == "const")
    {
        return Alphabet::CONST;
    }
    else if (lexeme == "int")
    {
        return Alphabet::INT;
    }
    else if (lexeme == "char")
    {
        return Alphabet::CHAR;
    }
    else if (lexeme == "while")
    {
        return Alphabet::WHILE;
    }
    else if (lexeme == "if")
    {
        return Alphabet::IF;
    }
    else if (lexeme == "float")
    {
        return Alphabet::FLOAT;
    }
    else if (lexeme == "else")
    {
        return Alphabet::ELSE;
    }
    else if (lexeme == "readln")
    {
        return Alphabet::READLN;
    }
    else if (lexeme == "div")
    {
        return Alphabet::DIV;
    }
    else if (lexeme == "string")
    {
        return Alphabet::STRING;
    }
    else if (lexeme == "write")
    {
        return Alphabet::WRITE;
    }
    else if (lexeme == "writeln")
    {
        return Alphabet::WRITELN;
    }
    else if (lexeme == "writeln")
    {
        return Alphabet::WRITELN;
    }
    else if (lexeme == "mod")
    {
        return Alphabet::MOD;
    }
    else if (lexeme == "boolean")
    {
        return Alphabet::BOOLEAN;
    }
    else
    {
        return Alphabet::ID;
    }
}

/**
 * @brief Return the simple operator token of a lexeme
 * @param char c
 * @return int token
 */
int getSimpleOperatorToken(char c)
{
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
    case '=':
        return Alphabet::EQUAL;
    case '<':
        return Alphabet::LESSTHAN;
    case '>':
        return Alphabet::GREATERTHAN;
    case '!':
        return Alphabet::NOT;
    default:
        return -1;
        break;
    }
}

/**
 * @brief Return the compost operator token of a lexeme that is reserved word
 * @param string lexeme
 * @return int token
 */
int getCompostOperatorToken(string lexeme)
{

    if (lexeme == "<=")
    {
        return Alphabet::LESSEQUAL;
    }
    else if (lexeme == ">=")
    {
        return Alphabet::GREATERTHAN;
    }
    else if (lexeme == "!=")
    {
        return Alphabet::NOTEQUAL;
    }
    else
    {
        return -1;
    }
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

    if (c == ' ' || c == '\n' || c == '\t') // Ignore insignificant char
    {
        transitionReturn.nextState = 0;
        transitionReturn.lexemeConcat = "";

        // Increment line count on line break read
        if (c == '\n')
            line++;
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
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, getSimpleOperatorToken(c), symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
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

        int pos = symbolTable->search(lexeme);

        // Lexeme not found in the symbol Table
        if (pos == null)
        {
            pos = symbolTable->insert(lexeme);

            // Create lexical register to IDENTIFIER
            LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::ID, pos, ConstType::NOT_CONSTANT);
            transitionReturn.lexicalReg = lexicalRegister;
        }
        else
        {
            // Get token and insert if lexeme is an id
            int token = getReservedWordToken(transitionReturn.lexemeConcat);

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
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
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
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, -1, ConstType::CHAR);
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
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, -1, ConstType::STRING);
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
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::DIVIDE, symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
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
        
        //If we have a unexpected eof
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
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::OR, symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
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

        int token = getCompostOperatorToken(lexeme);

        // Create lexical register to COMPOST LOGICAL OPERATORS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, token, symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else if (c != '=') // End of Simple logical operators lexical analysis
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        int token = getSimpleOperatorToken(c);

        // Create lexical register to SIMPLE LOGICAL OPERATORS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, token, symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
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

        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::ATRIB, symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    else // Throw exception when assignment command was not identified
    {
        if (cursor == eof)
            throwUnexpectedEOFException();
        else
            throwUndefinedLex(lexeme + c);
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
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::AND, symbolTable->search(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT);
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
 * @brief Lexical analyzer method
 *
 * @return string - recognized token
 */
string lexicalAnalyzer()
{
    char c; // read character
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
            return lexeme;
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

    // Read source program
    while (getline(cin, str))
    {
        program += str + '\n';
    }

    // TODO? Quando acabar as entregas do TP1 e TP2 descomentar essa parte - para apresentação
    // // remove last \n
    // program.pop_back();

    // Setting the global variable to control eof
    eof = program.length();

    // Initializing lexeme with a char != of eof flag
    string lexeme = "";

    // Calling lexical analyzer while eof is not reached
    while (cursor != eof)
    {
        lexeme = lexicalAnalyzer();
    }

    // Print line count and success compilation
    line == 1 ? (cout << "1 linha compilada.") : (cout << line << " linhas compiladas.");

    return 0;
}