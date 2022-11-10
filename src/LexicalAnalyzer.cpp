/**
 * @file LexicalAnalyzer.cpp
 * 
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Lexical Analyzer structure
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <iostream>
#include <string>

// import headers
#include "headers/LexicalAnalyzer.hpp"
#include "headers/LexicalRegister.hpp"
#include "headers/Utils.hpp"
#include "headers/Alphabet.hpp"
#include "headers/ConstType.hpp"
#include "headers/Throws.hpp"
#include "headers/SymbolTable.hpp"

#define finalState -1
#define null -999

using namespace std;

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

    // Ignore insignificant char
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
    {
        // Stay on the same state
        transitionReturn.nextState = 0;
        transitionReturn.lexemeConcat = "";

        // Increment line count on line break read
        if (c == '\n')
        {
            line++;
        }
    }
    // Variables and reserved words path
    else if (isCharacter(c) || c == '_')
    {
        // Go from state 0 to state 1
        transitionReturn.nextState = 1;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Integer without start 0 constants path
    else if (isNumber(c) && c != '0')
    {
        // Go from state 0 to state 2
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Numeric constants starting whit 0 / Hexa constants path
    else if (c == '0')
    {
        // Go from state 0 to state 4
        transitionReturn.nextState = 4;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Character constants path
    else if (c == '\'')
    {
        // Go from state 0 to state 7
        transitionReturn.nextState = 7;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // String constants path
    else if (c == '\"')
    {
        // Go from state 0 to state 9
        transitionReturn.nextState = 9;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Div operator or Comments path
    else if (c == '/')
    {
        // Go from state 0 to state 10
        transitionReturn.nextState = 10;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Logic OR Operator path
    else if (c == '|')
    {
        // Go from state 0 to state 13
        transitionReturn.nextState = 13;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Compost operators path
    else if (c == '<' || c == '>' || c == '!')
    {
        // Go from state 0 to state 14
        transitionReturn.nextState = 14;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Attribution operator path
    else if (c == ':')
    {
        // Go from state 0 to state 15
        transitionReturn.nextState = 15;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Logic AND Operator path
    else if (c == '&')
    {
        // Go from state 0 to state 16
        transitionReturn.nextState = 16;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Numeric constants starting whit . constants path
    else if (c == '.')
    {
        transitionReturn.nextState = 17;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Simple token operators symbols
    else if (c == '(' || c == ')' || c == ',' || c == '+' || c == '*' || c == ';' || c == '{' || c == '}' || c == '=' || c == '-' || c == '[' || c == ']')
    {
        // Go from state 0 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        //  Create lexical register to SIMPLE OPERATOR SYMBOLS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, null);
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

    // Variables and reserved words path
    if (isCharacter(c) || isNumber(c) || c == '_')
    {
        // Stay on the same state
        transitionReturn.nextState = 1;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // End of variables and reserved words lexical analysis
    else
    {
        // Go from state 1 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding last read char

        // Search for the lexeme on the Symbol Table
        int pos = symbolTable->find(lexeme);

        // Lexeme not found in the Symbol Table
        if (pos == null)
        {
            // Insert lexeme on Symbol Table
            symbolTable->insert(lexeme);
            pos = symbolTable->find(lexeme);

            // Create lexical register to IDENTIFIER
            LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::ID, pos, ConstType::NOT_CONSTANT,null);
            transitionReturn.lexicalReg = lexicalRegister;
        }
        else
        {
            // Get token and insert if lexeme is an identifier
            int token = symbolTable->search(transitionReturn.lexemeConcat);

            if (token == Alphabet::TRUE || token == Alphabet::FALSE) // Reserved word constant TRUE or FALSE
            {
                // Create lexical register to TRUE and FALSE
                LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, pos, ConstType::BOOLEAN, 1);
                transitionReturn.lexicalReg = lexicalRegister;
            }
            else // Is an identifier
            {
                // Create lexical register to RESERVED WORD
                LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, token, pos, ConstType::NOT_CONSTANT, null);
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

    // Numeric constant path
    if (isNumber(c))
    {
        // Stay on the same state
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Real number constant path
    else if (c == '.')
    {
        // Go from state 2 to state 3
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // End of Integer Numeric Constants lexical analysis
    else
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;

        // Go from state 2 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to INT CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::INT, 4);
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

    // Real number constant path
    if (isNumber(c))
    {
        // Stay on the same state
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    //  End of Float Numeric Constants lexical analysis
    else
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;

        // Go from state 3 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to FLOAT CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::FLOAT, 4);
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

    // Real number constant path
    if ((c == '.'))
    {
        // Go from state 4 to state 3
        transitionReturn.nextState = 3;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Numeric constants starting whit 0 constants path
    else if (isNumber(c))
    {
        // Go from state 4 to state 2
        transitionReturn.nextState = 2;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Hexa Number path
    else if (c == 'x')
    {
        // Go from state 4 to state 5
        transitionReturn.nextState = 5;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // End of 0 constante number lexical analysis
    else
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;

        // Go from state 4 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to INT CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::INT, 4);
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

    // Hexa Number path
    if (isHexa(c))
    {
        // Go from state 5 to state 6
        transitionReturn.nextState = 6;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Throw exception when Hexa Number was not identified
    else
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

    // End of Hexa Number path
    if (isHexa(c))
    {
        // Go from state 6 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to HEXA NUMBER CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::CHAR, 4);
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

    // Character constants path
    if (isValidChar(c))
    {
        // Go from state 7 to state 8
        transitionReturn.nextState = 8;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Throw exception when character was not identified
    else
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

    // End of Character Constants lexical analysis
    if (c == '\'')
    {
        // Go from state 8 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to CHAR CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::CHAR, 1);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    // Throw exception when character was not identified
    else
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

    // String constants path
    if (c != '\"' && c != '\n')
    {
        // Stay on same state
        transitionReturn.nextState = 9;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // End of String Constants lexical analysis
    else if (c == '\"')
    {
        // Go from state 9 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to STRING CONSTANT
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::STRING, 256);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    // Throw exception when string was not identified
    else
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

    // Comments analysis path
    if (c == '*')
    {
        // Go from state 10 to state 11
        transitionReturn.nextState = 11;
        transitionReturn.lexemeConcat = "";
    }
    // End of Divisor operator lexical analysis
    else if (c != '*')
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;

        // Go from state 10 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to DIVIDE
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, null);
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

    // Checks if it's a loop of comment
    if (c != '*')
    {
        // Stay on same state
        transitionReturn.nextState = 11;

        // Increment line count on line break read
        if (c == '\n')
            line++;

        // If we have a unexpected eof
        if (cursor == eof)
            throwUnexpectedEOFException();
    }
    // Comment analysis Path
    else if (c == '*')
    {
        // Go from state 11 to stat 12
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

    // Checks if it's a loop of comment
    if (c == '/')
    {
        // Go from state 12 to state 0
        transitionReturn.nextState = 0;
    }
    // Stay in comment loop
    else if (c == '*')
    {
        // Stay on the same state
        transitionReturn.nextState = 12;

        // If it have a unexpected eof
        if (cursor == eof)
            throwUnexpectedEOFException();
    }
    // Returns to previous state of comment
    else if (c != '*' && c != '/')
    {
        // Go from state 12 to state 11
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

    // End of OR Logic lexical analysis
    if (c == '|')
    {
        // Go from state 13 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // create lexical register to OR LOGIC
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, null);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    // Throw exception when logical operator OR was not identified
    else
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

    // End of Compost Logical Operators lexical analysis
    if (c == '=')
    {
        // Go from state 14 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to COMPOST LOGICAL OPERATORS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, null);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    // End of Simple logical operators lexical analysis
    else if (c != '=')
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;

        // Go from state 14 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to SIMPLE LOGICAL OPERATORS
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, 1);
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

    // End of Atribute Operator lexical analysis
    if (c == '=')
    {
        // Go from state 15 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to ATTRIBUTION OPERATOR
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, null);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    // Throw exception when assignment command was not identified
    else
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

    // End of AND logic lexical analysis
    if (c == '&')
    {
        // Go from state 16 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme + c;

        // Create lexical register to AND LOGIC
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, symbolTable->search(transitionReturn.lexemeConcat), symbolTable->find(transitionReturn.lexemeConcat), ConstType::NOT_CONSTANT, null);
        transitionReturn.lexicalReg = lexicalRegister;
    }
    // Throw exception when logical operator AND was not identified
    else
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

    // Numeric constants starting whit . constants path
    if (isNumber(c))
    {
        // Go from state 17 to 18
        transitionReturn.nextState = 18;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // Throw exception when logical operator (AND - &&) was not identified
    else
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

    // Numeric constants starting whit . constants path
    if (isNumber(c))
    {
        // Stay on the same state
        transitionReturn.nextState = 18;
        transitionReturn.lexemeConcat = lexeme + c;
    }
    // End of numeric constants starting whit . constants path
    else
    {
        // Returning a cursor position to avoid discarding valid characters for the next lexeme analysis
        cursor--;

        // Go from state 18 to FINAL STATE
        transitionReturn.nextState = finalState;
        transitionReturn.lexemeConcat = lexeme; // Discarding invalid char

        // Create lexical register to FLOAT NUMBER
        LexicalRegister lexicalRegister(transitionReturn.lexemeConcat, Alphabet::CONSTANT, null, ConstType::FLOAT, 4);
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
            return LexicalRegister("eof", -1, null, null, null);
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

    return tr.lexicalReg;
}