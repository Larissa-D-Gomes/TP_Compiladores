/**
 * @file ConstType.hpp
 * 
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Const Type header file for Lexical Register
 * @version 0.1
 * @date 2022-10-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

/**
 * @brief Auxiliar cons types class for Lexical Register
 * 
 */
class ConstType
{
public:
    static const int INT = 0;
    static const int CHAR = 1;
    static const int FLOAT = 2;
    static const int BOOLEAN = 3;
    static const int STRING = 4;
    static const int NOT_CONSTANT = -1;

    ConstType();
};