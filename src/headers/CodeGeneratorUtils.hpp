/**
 * @file codeGeneratorUtils.hpp
 *
 * COMPILADORES - CIÊNCIA DA COMPUTAÇÃO - PUC MINAS
 * @authors Larissa Domingues Gomes, Lucas Bottrel Lopes de Moura e Vinicius Silva Mendes
 * @brief Utils file and global important variables
 * @version 0.1
 * @date 2022-11-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#pragma once

#include <string>

#include "SymbolTable.hpp"
#include "LexicalRegister.hpp"

// Global next memory free position
extern long nextFreePosition;
// Global next memory free position
extern long M;
// Global next memory free position
extern long nextFreePosition;
// Global Nasm Assembly program
extern string assembly;
// Global Nasm Assembly for declarations program
extern string assemblyDec;
// Global Nasm Assembly for commands program
extern string assemblyCmd;
// Assembly Label counter
extern int assemblyLabel;
// Assembly Temp counter
extern int assemblyTempCount;

/**
 * @brief Struct to return address and type for variables 
 * 
 */
struct ExpressionReturn {
    long addr = null;
    int type = null;
};

/**
 * @brief Allocate memory for a variable tha contains 
 * memSize bytes
 * 
 * @param long memSize
 */
void memoryAlocation(long memSize);

string getNextAssemblyLabel();

string getCodeWrite(long addr, int type);

string getCodeWriteStr(long addr, int type);

int newTemp(int memSize);

void resetTempCounter();

long getTypeMemSize(int type);

long getCodeDeconst(bool hasMinnus, int type, string stringValue);

long getCodeExpConst(string lexeme, int const);

long getCodeNotExp(long addr, int type);

long getCodeTimesOperationtForFloat(long addr1, long addr2);

long getCodeTimesOperationtForFloatAndInt(long addr1, long addr2);

long getCodeTimesOperationtForInt(long addr1, long addr2);

long getCodeDivOperationtForInt(long addr1, long addr2);

long getCodeDivideOperationtForFloat(long addr1, long addr2);

long getCodeDivideOperationtForIntAndFloat(long addr1, long addr2);

long getCodeDivideOperationtForFloatAndInt(long addr1, long addr2);

long getCodeDivideOperationtForInt(long addr1, long addr2);

long getCodeModOperationtForInt(long addr1, long addr2);

long getCodePlusMinnusForFloat(long addr1, long addr2, int operation);

long getCodePlusMinnusForFloatAndInt(long addr1, long addr2, int operation);

long getCodePlusMinnusForIntAndFloat(long addr1, long addr2, int operation);

long getCodePlusMinnusForInt(long addr1, long addr2, int operation);