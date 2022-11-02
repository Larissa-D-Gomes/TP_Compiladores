/**
 * @file SyntaxAnalyzer.hpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Syntax Analyzer structure
 * @version 0.1
 * @date 2022-10-06
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma once

#include <string>
#include <iostream>

// import headers
#include "LexicalRegister.hpp"

using namespace std;

/**
 * @brief Syntax Analyzer class
 *
 */
class SyntaxAnalyzer
{

private:
    LexicalRegister regLex; // Lexical Register read by Lexical Analyzer
    int token;                        // Token from  Lexical Register read

public:
    /**
     * @brief Construct a new Syntax Analyzer object
     *
     */
    SyntaxAnalyzer();

    /**
     * @brief Match token from Lexical Analyzer and the expected token by L Language Grammar
     *
     * @param expectedToken Token expected by L Language Grammar
     */
    void matchToken(int expectedToken);

    /**
     * @brief Check if actual token is included in the first tokens of DEC grammar variable
     *
     * @return true - token is first of DEC variable
     * @return false - token is NOT first of DEC variable
     */
    bool checkFirstDEC();

    /**
     * @brief Check if actual token is included in the first tokens of ATR grammar variable
     *
     * @return true - token is first of ATR variable
     * @return false - token is NOT first of ATR variable
     */
    bool checkFirstATR();

    /**
     * @brief Check if actual token is included in the first tokens of DECONST grammar variable
     *
     * @return true - token is first of DECONST variable
     * @return false - token is NOT first of DECONST variable
     */
    bool checkFirstDECONST();

    /**
     * @brief Check if actual token is included in the first tokens of CMD grammar variable
     *
     * @return true - token is first of CMD variable
     * @return false - token is NOT first of CMD variable
     */
    bool checkFirstCMD();

    /**
     * @brief Check if actual token is included in the first tokens of PAR grammar variable
     *
     * @return true - token is first of PAR variable
     * @return false - token is NOT first of PAR variable
     */
    bool checkFirstPAR();

    /**
     * @brief Check if actual token is included in the first tokens of BLOCK grammar variable
     *
     * @return true - token is first of BLOCK variable
     * @return false - token is NOT first of BLOCK variable
     */
    bool checkFirstBLOCK();

    /**
     * @brief Check if actual token is included in the first tokens of EXP grammar variable
     *
     * @return true - token is first of EXP variable
     * @return false - token is NOT first of EXP variable
     */
    bool checkFirstEXP();

    /**
     * @brief Check if actual token is included in the first tokens of T grammar variable
     *
     * @return true - token is first of T variable
     * @return false - token is NOT first of T variable
     */
    bool checkFirstT();

    /**
     * @brief Check if actual token is included in the first tokens of R grammar variable
     *
     * @return true - token is first of R variable
     * @return false - token is NOT first of R variable
     */
    bool checkFirstR();

    /**
     * @brief Check if actual token is included in the first tokens of M grammar variable
     *
     * @return true - token is first of M variable
     * @return false - token is NOT first of M variable
     */
    bool checkFirstM();

    /**
     * @brief Variable S of the L Language Grammar
     */
    void S();

    /**
     * @brief Variable DEC of the L Language Grammar
     */
    void DEC();

    /**
     * @brief Variable ATR of the L Language Grammar
     */
    void ATR();

    /**
     * @brief Variable DECONST of the L Language Grammar
     */
    int DECONST();

    /**
     * @brief Variable CMD of the L Language Grammar
     */
    void CMD();

    /**
     * @brief Variable PAR of the L Language Grammar
     */
    void PAR();

    /**
     * @brief Variable BLOCK of the L Language Grammar
     */
    void BLOCK();

    /**
     * @brief Variable EXP of the L Language Grammar
     */
    int EXP();

    /**
     * @brief Variable T of the L Language Grammar
     */
    void T();

    /**
     * @brief Variable R of the L Language Grammar
     */
    void R();

    /**
     * @brief Variable M of the L Language Grammar
     */
    void M();

    /**
     * @brief Initial Syntax Analyzer call
     */
    void parser();
};