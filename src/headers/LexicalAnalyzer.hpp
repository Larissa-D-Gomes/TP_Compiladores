#pragma once

#include <string>
#include <iostream>

#include "LexicalRegister.hpp"

using namespace std;

/**
 * @brief Control the transitions of the lexical analyzer automaton
 */
struct TransitionReturn
{
    int nextState;
    string lexemeConcat;
    LexicalRegister lexicalReg;
};

/**
 * @brief Execute state 0 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateZeroTransition(string lexeme, char c);

/**
 * @brief Execute state 1 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateOneTransition(string lexeme, char c);

/**
 * @brief Execute state 2 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateTwoTransition(string lexeme, char c);

/**
 * @brief Execute state 3 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateThreeTransition(string lexeme, char c);

/**
 * @brief Execute state 4 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFourTransition(string lexeme, char c);

/**
 * @brief Execute state 5 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFiveTransition(string lexeme, char c);

/**
 * @brief Execute state 6 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSixTransition(string lexeme, char c);

/**
 * @brief Execute state 7 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSevenTransition(string lexeme, char c);

/**
 * @brief Execute state 8 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateEightTransition(string lexeme, char c);

/**
 * @brief Execute state 9 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateNineTransition(string lexeme, char c);

/**
 * @brief Execute state 10 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateTenTransition(string lexeme, char c);

/**
 * @brief Execute state 11 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateElevenTransition(string lexeme, char c);

/**
 * @brief Execute state 12 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateTwelveTransition(string lexeme, char c);

/**
 * @brief Execute state 13 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateThirteenTransition(string lexeme, char c);

/**
 * @brief Execute state 14 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFourteenTransition(string lexeme, char c);

/**
 * @brief Execute state 15 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateFifteenTransition(string lexeme, char c);

/**
 * @brief Execute state 16 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSixteenTransition(string lexeme, char c);

/**
 * @brief Execute state 17 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateSeventeenTransition(string lexeme, char c);

/**
 * @brief Execute state 18 transition actions
 *
 * @param lexeme - lexeme in creation
 * @param c - character read
 * @return TransitionReturn - next state and (token + c)
 */
TransitionReturn stateEighteenTransition(string lexeme, char c);

/**
 * @brief Lexical analyzer method
 *
 * @return string - recognized token
 */
LexicalRegister lexicalAnalyzer();