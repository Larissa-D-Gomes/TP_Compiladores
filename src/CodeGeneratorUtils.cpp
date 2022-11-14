/**
 * @file codeGeneratorUtils.cpp
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

#include <iostream>
#include <string.h>

#include "headers/Utils.hpp"
#include "headers/Alphabet.hpp"
#include "headers/SymbolTable.hpp"
#include "headers/LexicalRegister.hpp"
#include "headers/ConstType.hpp"

// Global next memory free position
long nextFreePosition = 0x10000; // 65536
// Global next memory free position
long M = 0x10000;
// Global Nasm Assembly program
string assembly = "";
// Global Nasm Assembly for declarations program
string assemblyDec = "";
// Global Nasm Assembly for commands program
string assemblyCmd = "";
// Assemble Label counter
int assemblyLabel = 0;
// Assembly Temp counter
int assemblyTempCount = 0;

/**
 * @brief Allocate memory for a variable tha contains
 * memSize bytes
 *
 * @param long memSize
 */
void memoryAlocation(long memSize)
{
    // cout << nextFreePosition << " " << memSize << endl;
    nextFreePosition += memSize;
}

int newTemp(int memSize)
{
    int aux = assemblyTempCount;
    assemblyTempCount += memSize;

    return aux;
}

void resetTempCounter()
{
    assemblyTempCount = 0;
}

string getNextAssemblyLabel()
{
    return "L" + to_string(++assemblyLabel);
}

string getCodeWriteStr(long addr, int type)
{
    string code = "\t;=-=-=-=-=-= INICIO WRITELN =-=-=-=-=-=\n";
    code += "\tmov rsi, M + " + to_string(addr) + " ; Movendo string da memória para registrador\n";
    code += "\tmov rax, rsi ; Copiando endereço da string para um registrador de índice\n";
    code += "\tmov rdx, 0 ; contador de caracteres = 0\n\n";

    string labelStartLoop = getNextAssemblyLabel();
    string labeEndLoop = getNextAssemblyLabel();

    // Begin of loop to calculate the length of the string
    code += "\tLoop para calcular tamanho da string\n";
    code += labelStartLoop + ": ; Inicio do loop\n";
    code += "\tmov al, [rax] ; Leitura de caracter na posicao rax da memória\n";
    code += "\tcmp al, 0 ; Verificação de flag de fim de string\n";
    code += "\tje" + labeEndLoop + " \n ; Se caracter lido = flag de fim de string finalizar loop\n";

    code += "\tadd rdx, 1 ; Incrementando numero de caracteres\n";
    code += "\tadd rax, 1 ; Incrementando indice da string\n";
    code += "\tjmp" + labelStartLoop + "  ; Se caracter lido != flag de fim de string continuar loop\n";

    // End of loop
    code += labeEndLoop + ": ; Fim do loop\n";
    code += "\tmov rax, 1 ; Chamada para saída\n";
    code += "\tmov rdi, 1 ; Chamada para tela\n";
    code += "\tsyscall\n";

    return code += ";=-=-=-=-=-= FIM WRITELN =-=-=-=-=-=\n";
}

string getCodeWrite(long addr, int type)
{

    if (type == ConstType::STRING)
    {
        return getCodeWriteStr(addr, type);
    }

    return ";=-=-=-=-=-= FIM WRITELN =-=-=-=-=-=\n";
}

long getTypeMemSize(int type)
{
    if (ConstType::INT == type || ConstType::FLOAT == type || ConstType::BOOLEAN == type)
    {
        return 4;
    }
    if (ConstType::STRING == type)
    {
        return 256;
    }
    return 1;
}

long getCodeDeconst(bool hasMinnus, int type, string stringValue)
{
    long actualMemoryPosition = nextFreePosition;

    if (type == ConstType::CHAR)
    {
        assemblyDec += "\tdb " + stringValue + ", 1\t\t\t; Declaracao Const Char em [" + to_string(actualMemoryPosition) + "]\n";
    }
    else if (type == ConstType::INT)
    {
        assemblyDec += "\tdd ";

        if (hasMinnus)
        {
            assemblyDec += "-";
        }

        assemblyDec += stringValue + "\t\t\t; Declaracao Const Inteiro em [" + to_string(actualMemoryPosition) + "]\n";
    }
    else if (type == ConstType::STRING)
    {
        assemblyDec += "\tdb " + stringValue + ",0\t\t\t; Declaracao Const String em [" + to_string(actualMemoryPosition) + "]\n";
    }
    else if (type == ConstType::FLOAT)
    {
        assemblyDec += "\tdd ";

        if (hasMinnus)
        {
            assemblyDec += "-";
        }

        if (stringValue[0] == '.')
        {
            assemblyDec += '0';
        }

        assemblyDec += stringValue + "\t\t\t; Declaracao Const Float em [" + to_string(actualMemoryPosition) + "]\n";
    }
    else if (type == ConstType::BOOLEAN)
    {
        if (stringValue == "false")
        {
            stringValue = "0";
        }
        else
        {
            stringValue = "1";
        }
        assemblyDec += "\tdd " + stringValue + "\t\t\t; Declaracao Const Boolean em [" + to_string(actualMemoryPosition) + "]\n";
    }

    memoryAlocation(getTypeMemSize(type));

    return actualMemoryPosition;
}

long getCodeExpConst(string stringValue, int type)
{
    long actualMemoryPosition;

    if (type == ConstType::STRING)
    {
        actualMemoryPosition = nextFreePosition;
        assemblyDec += "\tdb " + stringValue + ",0\t\t\t; Declaracao String EXP em [" + to_string(actualMemoryPosition) + "]\n";
        // Memory Allocation in data area
        memoryAlocation(getTypeMemSize(type));
    }
    else if (type == ConstType::FLOAT)
    {
        actualMemoryPosition = nextFreePosition;

        assemblyDec += "\tdd ";
        if (stringValue[0] == '.')
        {
            assemblyDec += " 0";
        }

        assemblyDec += stringValue + "\t\t\t; Declaracao Float EXP em [" + to_string(actualMemoryPosition) + "]\n";
        // Memory Allocation in data area
        memoryAlocation(getTypeMemSize(type));
    }
    else
    {
        actualMemoryPosition = assemblyTempCount;

        if (type == ConstType::INT)
        {
            assemblyCmd += "\tmov EAX, " + stringValue + "\t\t\t; Move Inteiro imediato para registrador\n";
            assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Move registrador para posicao atual de memoria em [" + to_string(actualMemoryPosition) + "]\n";
        }
        else if (type == ConstType::CHAR)
        {
            assemblyCmd += "\tmov AL, " + stringValue + "\t\t\t; Move Char imediato para registrador\n";
            assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], AL \t\t\t; Move registrador para posicao atual de memoria em [" + to_string(actualMemoryPosition) + "]\n";
        }
        else if (type == ConstType::BOOLEAN)
        {
            if (stringValue == "false")
            {
                stringValue = "0";
            }
            else
            {
                stringValue = "1";
            }

            assemblyCmd += "\tmov EAX, " + stringValue + "\t\t\t; Move Boolean imediado para registrador eax\n";
            assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Move registrador para posicao atual de memoria em [" + to_string(actualMemoryPosition) + "]\n";
        }
        newTemp(getTypeMemSize(type));
    }

    return actualMemoryPosition;
}

long getCodeNotExp(long addr, int type)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr) + " ] \t\t\t; Move o valor da memoria para o registrador \n";
    assemblyCmd += "\tneg EAX \t\t\t; Nega o valor que esta no registrador\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t; Nega o valor que esta no registrador\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], RAX \t\t\t; Move o valor do registrador para a posicao de memoria [" + to_string(actualMemoryPosition) + "]\n";

    newTemp(getTypeMemSize(type));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for times operation for two float
 * variables
 *
 * @param long float 1 address and float 2 address
 * @return long temporary variable address
 */
long getCodeTimesOperationtForFloat(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para o registrador XMM1\n";

    assemblyCmd += "\tmulss XMM0, XMM1  \t\t\t; float1 * float2\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for times operation for one float and one integer
 * variables
 *
 * @param long float address and integer address
 * @return long temporary variable address
 */
long getCodeTimesOperationtForFloatAndInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";
  
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t; Expande int para float\n";
    assemblyCmd += "\tmulss XMM0, XMM1  \t\t\t; float * float(int)\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for times operation for two int
 * variables
 *
 * @param long int 1 address and int 2 address
 * @return long temporary variable address
 */
long getCodeTimesOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";

    assemblyCmd += "\timul EBX \t\t\t; int1 * int2\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for div operation for two int
 * variables
 *
 * @param long int 1 address and int 2 address
 * @return long temporary variable address
 */
long getCodeDivOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";

    assemblyCmd += "\tidiv EBX \t\t\t; int1 div int2\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for two float
 * variables
 *
 * @param long float 1 address and float 2 address
 * @return long temporary variable address
 */
long getCodeDivideOperationtForFloat(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para o registrador XMM1\n";

    assemblyCmd += "\tdivss XMM0, XMM1  \t\t\t; float1 / float2\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for one float and one integer
 * variables
 *
 * @param long float address and integer address
 * @return long temporary variable address
 */
long getCodeDivideOperationtForFloatAndInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t; Expande int para float\n";
    assemblyCmd += "\tdivss XMM0, XMM1  \t\t\t; float / float(int)\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for one float and one integer
 * variables
 *
 * @param long integer address and float address
 * @return long temporary variable address
 */
long getCodeDivideOperationtForIntAndFloat(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";   
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t; Expande int para float\n";
    assemblyCmd += "\tdivss XMM0, XMM1  \t\t\t; float(int) / float\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for two int
 * variables
 *
 * @param long int 1 address and int 2 address
 * @return long temporary variable address
 */
long getCodeDivideOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

     // Type conversion
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t; Expande int1 para float1\n";
    
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t; Expande int2 para float2\n";

    assemblyCmd += "\tdivss XMM0, XMM1  \t\t\t; float(int1) / float(int2)\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

/**
 * @brief Generate code for mod operation for two int
 * variables
 *
 * @param long int 1 address and int 2 address
 * @return long temporary variable address
 */
long getCodeModOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";

    assemblyCmd += "\tidiv EBX \t\t\t; int1 mod int2\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EDX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

long getCodePlusMinnusForFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para o registrador XMM1\n";

    if(operation == Alphabet::PLUS){
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1 + float2\n";
    } else{
        assemblyCmd += "\tsubss XMM0, XMM1  \t\t\t; float1 - float2\n";
    }
    
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

long getCodePlusMinnusForFloatAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t; Expande int para float\n";

    if(operation == Alphabet::PLUS){
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1 + float2(int)\n";
    } else{
        assemblyCmd += "\tsubss XMM0, XMM1  \t\t\t; float1 - float2(int)\n";
    }
    
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

long getCodePlusMinnusForIntAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, EAX \t\t\t; Expande int para float\n";
    
    if(operation == Alphabet::PLUS){
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1(int) + float2\n";
    } else{
        assemblyCmd += "\tsubss XMM0, XMM1  \t\t\t; float1(int) - float2\n";
    }
    
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

long getCodePlusMinnusForInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";

    if(operation == Alphabet::PLUS){
        assemblyCmd += "\tadd EAX, EBX  \t\t\t; int1 + int2\n";
    } else{
        assemblyCmd += "\tsub EAX, EBX  \t\t\t; int1 - int2\n";
    }

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

long getCodeForOR(long addr1, long addr2){

    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de boolean 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de boolean 2 da memoria para o registrador EBX\n";
    // Not boolean 1
    assemblyCmd += "\tneg EAX \t\t\t; EAX = !EAX\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t; EAX = !EAX\n";
    // Not boolean 2
    assemblyCmd += "\tneg EBX \t\t\t; EBX = !EBX\n";
    assemblyCmd += "\tadd EBX, 1 \t\t\t; EBX = !EBX\n";
    // Not boolean 1 And Not boolean 2
    assemblyCmd += "\timul EBX \t\t\t; !EAX AND !EBX\n" ; 
    //  Not (Not boolean 1 And Not boolean 2) = Boolean 1 Or Boolean 2
    assemblyCmd += "\tneg EAX \t\t\t; !(!EAX AND !EBX) = EAX OR EBX\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t; !(!EAX AND !EBX) = EAX OR EBX\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";


    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

///

long getCodeCmpForFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para o registrador XMM1\n";

    assemblyCmd += getCmpCodeFloat(operation);
    
    return actualMemoryPosition;
}

long getCodeCmpForFloatAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em EAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t; Expande int para float\n";

    if(operation == Alphabet::PLUS){
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1 + float2(int)\n";
    } else{
        assemblyCmd += "\tsubss XMM0, XMM1  \t\t\t; float1 - float2(int)\n";
    }
    
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

long getCodeCmpForIntAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, EAX \t\t\t; Expande int para float\n";
    
    if(operation == Alphabet::PLUS){
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1(int) + float2\n";
    } else{
        assemblyCmd += "\tsubss XMM0, XMM1  \t\t\t; float1(int) - float2\n";
    }
    
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::FLOAT));
    return actualMemoryPosition;
}

long getCodeCmpForInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";

    if(operation == Alphabet::PLUS){
        assemblyCmd += "\tadd EAX, EBX  \t\t\t; int1 + int2\n";
    } else{
        assemblyCmd += "\tsub EAX, EBX  \t\t\t; int1 - int2\n";
    }

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

string getCmpCodeFloat(int operation){
    long actualMemoryPosition = assemblyTempCount;
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tcomiss XMM0, XMM1  \t\t\t; compara float1 com float2\n";
    // =
    if(operation == Alphabet::EQUAL){
        assemblyCmd += "\tje " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 = float2\n";
    } 
    // >
    else if (operation == Alphabet::GREATERTHAN){
        assemblyCmd += "\tja " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 > float2\n";
    }  
    // >=
    else if (operation == Alphabet::GREATEREQUAL){
        assemblyCmd += "\tjae " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 >= float2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN){
        assemblyCmd += "\tjb " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 < float2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL){
        assemblyCmd += "\tjbe " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 <= float2\n";
    }    
    // !=
    else if (operation == Alphabet::NOTEQUAL){
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 != float2\n";
    }  


    assemblyCmd += "mov EAX, 0 ; Define registrador como falso";
    assemblyCmd += "jmp " + labelEnd;

    assemblyCmd += labelTrue + ":";
    assemblyCmd += "mov EAX, 1 ; Define registrador como true";

    assemblyCmd += labelEnd + ":";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";


    newTemp(getTypeMemSize(ConstType::BOOLEAN));
}