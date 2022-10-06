#include <iostream>
#include <string>

#include "headers/LexicalAnalyzer.hpp"
#include "headers/LexicalRegister.hpp"
#include "headers/Utils.hpp"
#include "headers/Alphabet.hpp"
#include "headers/ConstType.hpp"
#include "headers/Throws.hpp"
#include "headers/SymbolTable.hpp"

#define finalState -1
#define null -1

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