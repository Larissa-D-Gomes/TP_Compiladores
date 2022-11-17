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
extern long addrStringTrue;
extern long addrStringFalse;

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

int newTemp(int memSize);

void resetTempCounter();

long getTypeMemSize(int type);

// WRITE / WRITELN
void getCodeWrite(long addr, int type);

void getCodeWriteLineBr();

void getCodeWriteStr(long addr);

void getCodeWriteChar(long addr);

void getCodeWriteInt(long addr);

void getCodeWriteFloat(long addr);

void getCodeWriteBoolean(long addr);

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

long getCodeForOR(long addr1, long addr2);

// EXP OPERATIONS
long getCodeCmpForCharAndChar(long addr1, long addr2, int operation);

long getCodeCmpForIntAndInt(long addr1, long addr2, int operation);

long getCodeCmpForIntAndFloat(long addr1, long addr2, int operation);

long getCodeCmpForFloatAndInt(long addr1, long addr2, int operation);

long getCodeCmpForFloatAndFloat(long addr1, long addr2, int operation);

long getCodeCmpForStringAndString(long addr1, long addr2, int operation);

string getCmpCodeFloat(int operation);

string getCmpCodeInt(int operation);

long getCodeAccessStringPosition(long addrString, long addrIndex);

void getCodeAtribStringPos(long addrID, long addrIndex, long addrAtrib);

void getCodeAtribIntAndInt(long addr1, long addr2);

void getCodeAtribFloatAndFloat(long addr1, long addr2);

void getCodeAtribFloatAndInt(long addr1, long addr2);

void getCodeAtribStringAndString(long addr1, long addr2);

void getCodeAtribBooleanAndBoolean(long addr1, long addr2);

void getCodeAtribCharAndChar(long addr1, long addr2);

// READLN
void getCodeRead(long addr, int type);

void getCodeReadStr(long addr);

void getCodeReadFloat(long addr);

void getCodeReadInt(long addr);