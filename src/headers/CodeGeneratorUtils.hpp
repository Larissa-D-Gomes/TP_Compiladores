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
// True memory position for boolean print
extern long addrStringTrue;
// False memory position for boolean print
extern long addrStringFalse;
// Line break memory position for boolean print
extern long addrStringLineBr;

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

/**
 * @brief Get current temp position and alloc for next use
 * 
 * @param memSize 
 * @return int 
 */
int newTemp(int memSize);

/**
 * @brief Reset memory temp count
 * 
 */
void resetTempCounter();

/**
 * @brief Get the Next Assembly Label object
 * 
 * @return string 
 */
string getNextAssemblyLabel();

/**
 * @brief Get the memory size for const type
 * 
 * @param type const type
 * @return long size in bytes
 */
long getTypeMemSize(int type);

// **** WRITE SECTION ****

/**
 * @brief Write assembly code for write String
 *
 * @param addr Expression address to be write
 */
void getCodeWriteStr(long addr);

/**
 * @brief Write assembly code for write Char
 *
 * @param addr Expression address to be write
 */
void getCodeWriteChar(long addr);

/**
 * @brief Write assembly code for write Int
 *
 * @param addr Expression address to be write
 */
void getCodeWriteInt(long addr);

/**
 * @brief Write assembly code for write Float
 *
 * @param addr Expression address to be write
 */
void getCodeWriteFloat(long addr);

/**
 * @brief Write assembly code for write Boolean
 *
 * @param addr Expression address to be write
 */
void getCodeWriteBoolean(long addr);

/**
 * @brief Write assembly code for line break
 *
 */
void getCodeWriteLineBr();

/**
 * @brief Write assembly code for write type
 *
 * @param addr Expression address to be write
 * @param type ConsType value to print
 */
void getCodeWrite(long addr, int type);

// **** DECONST SECTION ****

/**
 * @brief Write assembly code for DECONST grammar variable
 *
 * @param hasMinnus If the number is negative
 * @param type Const.type
 * @param stringValue Const.lex
 * @return long Memory position of const value
 */
long getCodeDeconst(bool hasMinnus, int type, string stringValue);

// **** M SECTION ****

/**
 * @brief Write assembly code for Const value in M  grammar variable
 *
 * @param stringValue Const.lex
 * @param type Const.type
 * @return long Memory position allocated
 */
long getCodeExpConst(string lexeme, int const);

/**
 * @brief Write assembly code for NOT Exp in M grammar variable
 *
 * @param addr Expression address to be write
 * @param type Expression type
 * @return long Temporary memory position
 */
long getCodeNotExp(long addr, int type);

/**
 * @brief Write assembly code for String Position
 *
 * @param addrString String addr
 * @param addrIndex Index of string
 * @return long Temporary memory position for index value
 */
long getCodeAccessStringPosition(long addrString, long addrIndex);

/**
 * @brief Generate code for casting command - float(int) || int(float)
 *
 * @param addr Address of the variable to be converted
 * @param convType Type to be converted
 * @return long Converted number temporary position
 */
long getCodeCasting(long addr, int convType);

// **** R SECTION ****

/**
 * @brief Generate code for times operation for two float
 * variables - (FLOAT * FLOAT)
 *
 * @param addr1 Float 1 address
 * @param addr2 Float 2 address
 * @return long Temporary variable address
 */
long getCodeTimesOperationtForFloat(long addr1, long addr2);

/**
 * @brief Generate code for times operation for one float and one integer
 * variables - (INT * FLOAT) || (FLOAT * INT)
 *
 * @param addr1 Float address
 * @param addr2 Integer address
 * @return long Temporary variable address
 */
long getCodeTimesOperationtForFloatAndInt(long addr1, long addr2);

/**
 * @brief Generate code for times operation for two int
 * variables - (INT * INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @return long Temporary variable address
 */
long getCodeTimesOperationtForInt(long addr1, long addr2);

/**
 * @brief Generate code for div operation for two int
 * variables - (INT DIV INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @return long Temporary variable address
 */
long getCodeDivOperationtForInt(long addr1, long addr2);

/**
 * @brief Generate code for divide operation for two float
 * variables - (FLOAT / FLOAT)
 *
 * @param addr1 Float 1 address
 * @param addr2 Float 2 address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForFloat(long addr1, long addr2);

/**
 * @brief Generate code for divide operation for two float
 * variables - (FLOAT / INT)
 *
 * @param addr1 Float address
 * @param addr2 Int address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForFloatAndInt(long addr1, long addr2);

/**
 * @brief Generate code for divide operation for int and float
 * variables - (INT / FLOAT)
 *
 * @param addr1 Int address
 * @param addr2 Float address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForIntAndFloat(long addr1, long addr2);

/**
 * @brief Generate code for divide operation for two int
 * variables - (INT / INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForInt(long addr1, long addr2);

/**
 * @brief Generate code for mod operation for two int
 * variables - (INT MOD INT)
 *
 * @param addr1 int 1 address
 * @param addr2 int 2 address
 * @return long Temporary variable address
 */
long getCodeModOperationtForInt(long addr1, long addr2);

// **** T SECTION ****

/**
 * @brief Generate code for plus and minnus operation for floats
 * variables - (FLOAT +- FLOAT)
 *
 * @param addr1 float 1 address
 * @param addr2 float 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForFloat(long addr1, long addr2, int operation);

/**
 * @brief Generate code for plus and minnus operation for float and int
 * variables - (FLOAT +- INT)
 *
 * @param addr1 float 1 address
 * @param addr2 float 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForFloatAndInt(long addr1, long addr2, int operation);

/**
 * @brief Generate code for plus and minnus operation for int and float
 * variables - (INT +- FLOAT)
 *
 * @param addr1 Float 1 address
 * @param addr2 Float 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForIntAndFloat(long addr1, long addr2, int operation);

/**
 * @brief Generate code for plus and minnus operation for ints
 * variables - (INT +- INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForInt(long addr1, long addr2, int operation);

/**
 * @brief Generate code for logic OR between booleans - (BOOLEAN || BOOLEAN)
 *
 * @param addr1 Boolean 1 address
 * @param addr2 Boolean 2 address
 * @return long Temporary variable address
 */
long getCodeForOR(long addr1, long addr2);

// **** EXP SECTION ****

/**
 * @brief Get the compare code between floats - (FLOAT >|>=|=|<=|<|!= FLOAT)
 *
 * @param operation Comparation operation
 * @return string Assembly code for compare operation
 */
void getCmpCodeFloat(int operation);

/**
 * @brief Get the compare code between ints - (INT >|>=|=|<=|<|!= INT)
 *
 * @param operation Comparation operation
 * @return string Assembly code for compare operation
 */
void getCmpCodeInt(int operation);

/**
 * @brief Get the assembly compare code between Chars - (CHAR >|>=|=|<=|<|!= CHAR)
 *
 * @param addr1 Char 1 address
 * @param addr2 Char 2 address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForCharAndChar(long addr1, long addr2, int operation);

/**
 * @brief Get the assembly compare code between Ints - (INT >|>=|=|<=|<|!= INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForIntAndInt(long addr1, long addr2, int operation);

/**
 * @brief Get the assembly compare code between Int and Float - (INT >|>=|=|<=|<|!= FLOAT)
 *
 * @param addr1 Int address
 * @param addr2 Float address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForIntAndFloat(long addr1, long addr2, int operation);

/**
 * @brief Get the assembly compare code between Float and Int - (FLOAT >|>=|=|<=|<|!= INT)
 *
 * @param addr1 Float address
 * @param addr2 Int address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForFloatAndInt(long addr1, long addr2, int operation);

/**
 * @brief Get the assembly compare code between Floats - (FLOAT >|>=|=|<=|<|!= FLOAT)
 *
 * @param addr1 Float 1 addr
 * @param addr2 Float 2 addr
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForFloatAndFloat(long addr1, long addr2, int operation);

/**
 * @brief Get the assembly compare code between Strings - (STRING = STRING)
 *
 * @param addr1 String 1 address
 * @param addr2 String 2 address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForStringAndString(long addr1, long addr2, int operation);

// **** ATRIB SECTION ****

/**
 * @brief Generate code for atribuition of char to a string position - (STRING[X] := CHAR)
 *
 * @param addrID address of string that will receive the value
 * @param addrIndex address of the index value
 * @param addrAtrib address of chat contains the value
 */
void getCodeAtribStringPos(long addrID, long addrIndex, long addrAtrib);

/**
 * @brief Generate code for atribuition of int to int - (INT := INT)
 *
 * @param addr1 ID address (address of int that will receive the value)
 * @param addr2 EXP address (address of int contains the value)
 */
void getCodeAtribIntAndInt(long addr1, long addr2);

/**
 * @brief Generate code for atribuition of float to float - (FLOAT := FLOAT)
 *
 * @param addr1 ID address (address of float that will receive the value)
 * @param addr2 EXP address (address of float contains the value)
 */
void getCodeAtribFloatAndFloat(long addr1, long addr2);

/**
 * @brief Generate code for atribuition of int to float - (FLOAT := INT)
 *
 * @param addr1 ID address (address of float that will receive the value)
 * @param addr2 EXP address (address of int contains the value)
 */
void getCodeAtribFloatAndInt(long addr1, long addr2);

/**
 * @brief Generate code for atribuition of string to string - (STRING := STRING)
 *
 * @param addr1 ID address (address of string that will receive the value)
 * @param addr2 EXP address (address of string contains the value)
 */
void getCodeAtribStringAndString(long addr1, long addr2);

/**
 * @brief Generate code for atribuition of boolean to boolean - (BOOLEAN := BOOLEAN)
 *
 * @param addr1 ID address (address of boolean that will receive the value)
 * @param addr2 EXP address (address of boolean contains the value)
 */
void getCodeAtribBooleanAndBoolean(long addr1, long addr2);

/**
 * @brief Generate code for atribuition of char to char - (CHAR := CHAR)
 *
 * @param addr1 ID address (address of char that will receive the value)
 * @param addr2 EXP address (address of char contains the value)
 */
void getCodeAtribCharAndChar(long addr1, long addr2);

// **** READ SECTION ****

/**
 * @brief Generate code for read string command
 *
 * @param addr (address of the string variable to receive the read value)
 */
void getCodeReadStr(long addr);

/**
 * @brief Generate code for read int command
 *
 * @param long addr (address of the int variable to receive the read value)
 */
void getCodeReadInt(long addr);

/**
 * @brief Generate code for read float command
 *
 * @param long addr (address of the float variable to receive the read value)
 */
void getCodeReadFloat(long addr);

/**
 * @brief Generate code for read char command
 *
 * @param long addr (address of the char variable to receive the read value)
 */
void getCodeReadChar(long addr);

/**
 * @brief Generate code for read command
 *
 * @param long addr (address of the variable to receive the read value), int type (type to be read)
 */
void getCodeRead(long addr, int type);

// **** IF SECTION ****

/**
 * @brief Generate code for begin of a if block
 *
 * @param long addr (address of the boolean expression used in if condition)
 * @return string end of if label
 */
string getCodeOpenIf(long addr);

/**
 * @brief Generate code for end of a if block
 *
 * @param string end of if label
 */
void getCodeCloseBlockIf(string label);

/**
 * @brief Generate code for begin of a else block
 *
 * @param long addr (address of the boolean expression used in if condition)
 * @return string end of else label
 */
string getCodeOpenElse(long addr);

// **** WHILE SECTION ****

/**
 * @brief Generate code for begin of a while block
 *
 * @param addr address of a boolean expression
 * @param labelFalse Label end of while
 * @param labelLoop Label begin of while
 */
void getCodeOpenWhile(long addr, string &labelFalse, string &labelLoop);

/**
 * @brief Generate code for end of while block
 *
 * @param string labelFalse (Label end of while) and string labelLoop (Label begin of while)
 */
void getCodeCloseBlockWhile(string labelFalse, string labelLoop);