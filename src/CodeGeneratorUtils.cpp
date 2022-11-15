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

void getCodeBreakLine()
{
    
}

// **** WRITE SECTION ****

void getCodeWriteStr(long addr)
{
    assemblyCmd += "\t;  WRITE STRING\n\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " ; Copiando endereço da string para um registrador de índice\n";
    assemblyCmd += "\tmov RDX, 0 ; contador de caracteres = 0\n\n";

    string labelStartLoop = getNextAssemblyLabel();
    string labeEndLoop = getNextAssemblyLabel();

    // Begin of loop to calculate the length of the string
    assemblyCmd += "\t; Loop para calcular tamanho da string\n";
    assemblyCmd += labelStartLoop + ": ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RSI] ; Leitura de caracter na posicao rax da memória\n";
    assemblyCmd += "\tcmp AL, 0 ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labeEndLoop + " \n ; Se caracter lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\tadd RDX, 1 ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tadd RSI, 1 ; Incrementando indice da string\n";
    assemblyCmd += "\tjmp " + labelStartLoop + "  ; Se caracter lido != flag de fim de string continuar loop\n";

    // End of loop
    assemblyCmd += labeEndLoop + ": ; Fim do loop\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " ; Copiando endereco inicial da string\n";
    assemblyCmd += "\tmov RAX, 1 ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";
}

void getCodeWriteChar(long addr)
{
    assemblyCmd += "\t;  WRITE CHAR\n\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " ; Copiando endereço do character para um registrador de índice\n";
    assemblyCmd += "\tmov RDX, 1 ; Quantidade de caracteres imprimiveis \n\n";

    assemblyCmd += "\tmov RAX, 1 ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";
}

void getCodeWriteInt(long addr)
{
    // assemblyCmd
    // mov eax, [qword M+Exp.end];inteiro a ser
    // ;convertido
    // mov rsi, M+buffer.end ;end. string ou temp.
    // mov rcx, 0 ;contador pilha
    // mov rdi, 0 ;tam. string convertido
    // cmp eax, 0 ;verifica sinal
    // jge Rot0 ;salta se número positivo
    // mov bl, ‘-‘ ;senão, escreve sinal –
    // mov [rsi], bl
    // add rsi, 1 ;incrementa índice
    // add rdi, 1 ;incrementa tamanhoneg eax ;toma módulo do número
    // Rot0:
    // mov ebx, 10 ;divisor
    // Rot1:
    // add rcx, 1 ;incrementa contador
    // cdq ;estende edx:eax p/ div.
    // idiv ebx ;divide edx;eax por ebx
    // push dx ;empilha valor do resto
    // cmp eax, 0 ;verifica se quoc. é 0
    // jne Rot1 ;se não é 0, continua
    // add rdi,rcx ;atualiza tam. string
    // ;agora, desemp. os valores e escreve o string
    // Rot2:
    // pop dx ;desempilha valor
    // add dl, ‘0’ ;transforma em caractere
    // mov [rsi], dl ;escreve caractere
    // add rsi, 1 ;incrementa base
    // sub rcx, 1 ;decrementa contador
    // cmp rcx, 0 ;verifica pilha vazia
    // jne Rot2 ;se não pilha vazia, loop
}

void getCodeWriteFloat(long addr)
{

}

void getCodeWriteBoolean(long addr)
{

}

void getCodeWrite(long addr, int type)
{
    if (type == ConstType::STRING)
    {
        getCodeWriteStr(addr);
    }
    else if (type == ConstType::CHAR)
    {
        getCodeWriteChar(addr);
    }
    else if (type == ConstType::INT)
    {
        getCodeWriteInt(addr);
    }
    else if (type == ConstType::FLOAT)
    {
        getCodeWriteFloat(addr);
    }
    else if (type == ConstType::BOOLEAN)
    {
        getCodeWriteBoolean(addr);
    }
}

// **** READ SECTION ****

/*
void getCodeRead()
{
    if (type == ConstType::STRING)
    {
        getCodeReadStr(addr);
    }
    else if (type == ConstType::CHAR)
    {
        getCodeReadChar(addr);
    }
    else if (type == ConstType::INT)
    {
        getCodeReadInt(addr);
    }
    else if (type == ConstType::FLOAT)
    {
        getCodeReadFloat(addr);
    }
    else if (type == ConstType::BOOLEAN)
    {
        getCodeReadBoolean(addr);
    }
}
*/

long getCodeDeconst(bool hasMinnus, int type, string stringValue)
{
    long actualMemoryPosition = assemblyTempCount;

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

// R OPERATIONS
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

// T OPERATIONS
long getCodePlusMinnusForFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para o registrador XMM1\n";

    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1 + float2\n";
    }
    else
    {
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

    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1 + float2(int)\n";
    }
    else
    {
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

    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\taddss XMM0, XMM1  \t\t\t; float1(int) + float2\n";
    }
    else
    {
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

    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\tadd EAX, EBX  \t\t\t; int1 + int2\n";
    }
    else
    {
        assemblyCmd += "\tsub EAX, EBX  \t\t\t; int1 - int2\n";
    }

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::INT));
    return actualMemoryPosition;
}

long getCodeForOR(long addr1, long addr2)
{

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
    assemblyCmd += "\timul EBX \t\t\t; !EAX AND !EBX\n";
    //  Not (Not boolean 1 And Not boolean 2) = Boolean 1 Or Boolean 2
    assemblyCmd += "\tneg EAX \t\t\t; !(!EAX AND !EBX) = EAX OR EBX\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t; !(!EAX AND !EBX) = EAX OR EBX\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

// EXP OPERATIONS
string getCmpCodeFloat(int operation)
{
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tcomiss XMM0, XMM1  \t\t\t; compara float1 com float2\n";
    // =
    if (operation == Alphabet::EQUAL)
    {
        assemblyCmd += "\tje " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 = float2\n";
    }
    // >
    else if (operation == Alphabet::GREATERTHAN)
    {
        assemblyCmd += "\tja " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 > float2\n";
    }
    // >=
    else if (operation == Alphabet::GREATEREQUAL)
    {
        assemblyCmd += "\tjae " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 >= float2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN)
    {
        assemblyCmd += "\tjb " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 < float2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL)
    {
        assemblyCmd += "\tjbe " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 <= float2\n";
    }
    // !=
    else if (operation == Alphabet::NOTEQUAL)
    {
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t; salta para " + labelTrue + " se float1 != float2\n";
    }

    assemblyCmd += "\tmov EAX, 0 ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";

    return assemblyCmd;
}

string getCmpCodeInt(int operation)
{
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tcmp EAX, EBX  \t\t\t; compara int1 com int2\n";
    // =
    if (operation == Alphabet::EQUAL)
    {
        assemblyCmd += "\tje " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 = int2\n";
    }
    // >
    else if (operation == Alphabet::GREATERTHAN)
    {
        assemblyCmd += "\tjg " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 > int2\n";
    }
    // >=
    else if (operation == Alphabet::GREATEREQUAL)
    {
        assemblyCmd += "\tjge " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 >= int2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN)
    {
        assemblyCmd += "\tjl " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 < int2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL)
    {
        assemblyCmd += "\tjle " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 <= int2\n";
    }
    // !=
    else if (operation == Alphabet::NOTEQUAL)
    {
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 != int2\n";
    }

    assemblyCmd += "\tmov EAX, 0 ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";

    return assemblyCmd;
}

long getCodeCmpForCharAndChar(long addr1, long addr2, int operation)
{

    long actualMemoryPosition = assemblyTempCount;

    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tmov AL, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de char 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov BL, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de char 2 da memoria para o registrador EBX\n";
    assemblyCmd += "\tcmp AL, BL \t\t\t ; compara caracteres";

    if (operation == Alphabet::EQUAL)
    {
        assemblyCmd += "\tje " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 = int2\n";
    }
    // >
    else if (operation == Alphabet::GREATERTHAN)
    {
        assemblyCmd += "\tjg " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 > int2\n";
    }
    // >=
    else if (operation == Alphabet::GREATEREQUAL)
    {
        assemblyCmd += "\tjge " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 >= int2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN)
    {
        assemblyCmd += "\tjl " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 < int2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL)
    {
        assemblyCmd += "\tjle " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 <= int2\n";
    }
    // !=
    else if (operation == Alphabet::NOTEQUAL)
    {
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t; salta para " + labelTrue + " se int1 != int2\n";
    }

    assemblyCmd += "\tmov EAX, 0 ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

long getCodeCmpForIntAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int 1 da memoria para o registrador EAX\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int 2 da memoria para o registrador EBX\n";
    getCmpCodeInt(operation);

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

long getCodeCmpForIntAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de int da memoria para o registrador EAX\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t; Expande int para float\n";
    getCmpCodeFloat(operation);

    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

long getCodeCmpForFloatAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmov RAX, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de int da memoria para o registrador RAX\n";

    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t; Converte int para float\n";
    getCmpCodeFloat(operation);

    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

long getCodeCmpForFloatAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para o registrador XMM1\n";

    getCmpCodeFloat(operation);

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

long getCodeCmpForStringAndString(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    string labelStartLoop = getNextAssemblyLabel();
    string labelEndLoop = getNextAssemblyLabel();
    string labelFalse = getNextAssemblyLabel();
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tmov RSI, M + " + to_string(addr1) + " ; Movendo string1 da memória para registrador\n";
    assemblyCmd += "\tmov RDI, M + " + to_string(addr2) + " ; Movendo string2 da memória para registrador\n";

    assemblyCmd += labelStartLoop + ": ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RSI] ; Leitura de caracter na posicao RAX da memória\n";
    assemblyCmd += "\tmov BL, [RDI] ; Leitura de caracter na posicao RBX da memória\n";
    assemblyCmd += "\tcmp AL, 0 ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labelEndLoop + " \n ; Se caracter lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\tcmp AL, BL ; Verificação de igualdade de caracter\n";
    assemblyCmd += "\tjne " + labelFalse + " \n ; Se string1[i] != string2[i]\n";
    assemblyCmd += "\tadd RSI, 1 ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tadd RDI, 1 ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tjmp " + labelStartLoop + "  ; Se caracter lido != flag de fim de string continuar loop\n";

    assemblyCmd += labelEndLoop + ": ; Fim do loop\n";
    assemblyCmd += "\tcmp BL, 0 ; Verificação de flag de fim de string\n";

    assemblyCmd += "\tje " + labelTrue + " \n ; Se caracter lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\n" + labelFalse + ":\n";
    assemblyCmd += "\tmov EAX, 0 ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva resultado em temporario\n";

    newTemp(getTypeMemSize(ConstType::BOOLEAN));
    return actualMemoryPosition;
}

long getCodeAccessStringPosition(long addrString, long addrIndex)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\tmov EBX, [ M +" + to_string(addrIndex) + " ] \t\t\t ; Recupera valor de indice da string da memoria\n";
    assemblyCmd += "\tadd EBX, M + " + to_string(addrString) + " \t\t\t ; Move endereco de memoria da string para registrador RSI\n";
    assemblyCmd += "\tmov EAX, [EBX] \t\t\t ; Escreve valor de caracter para o registrador AL\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t; Salva character em temporario\n";

    newTemp(getTypeMemSize(ConstType::CHAR));
    return actualMemoryPosition;
}

void getCodeAtribStringPos(long addrID, long addrIndex, long addrAtrib)
{
    assemblyCmd += "\tmov EAX, [ M +" + to_string(addrIndex) + " ] \t\t\t ; Recupera valor de indice da string da memoria\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addrAtrib) + " \t\t\t ; Move endereco de memoria da string para registrador RSI\n";
    assemblyCmd += "\tadd RSI, EAX \t\t\t ; Adiciona indice ao endereco inicial da memoria\n";
    assemblyCmd += "\tmov AL, [RSI] \t\t\t ; Escreve valor de caracter para o registrador AL\n";

    assemblyCmd += "\tmov [ M + " + to_string(addrID) + " ], AL \t\t\t; Salva character no endereco do identificador\n";
}

void getCodeAtribIntAndInt(long addr1, long addr2)
{
    assemblyCmd += "\tmov EAX, [ M +" + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tmov [M + " + to_string(addr1) + " ] , EAX \t\t\t ; Salva valor do registrador no endereco do ID\n";
}

void getCodeAtribFloatAndFloat(long addr1, long addr2)
{
    assemblyCmd += "\tmovss XMM0, [ M +" + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tmovss [M + " + to_string(addr1) + " ] , XMM0 \t\t\t ; Salva valor do registrador no endereco do ID\n";
}

void getCodeAtribFloatAndInt(long addr1, long addr2)
{
    assemblyCmd += "\tmov RAX, [ M +" + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t; Converte int para float\n";

    assemblyCmd += "\tmovss [ M + " + to_string(addr1) + " ], XMM0 \t\t\t;  Salva resultado no endereco do identificador\n";
}

void getCodeAtribStringAndString(long addr1, long addr2)
{
    assemblyCmd += "\tmov RSI, M + " + to_string(addr1) + " ; Copiando endereço da string para um registrador de índice\n";
    assemblyCmd += "\tmov RDI, M + " + to_string(addr2) + " ; Copiando endereço da string para um registrador de índice\n";

    string labelStartLoop = getNextAssemblyLabel();
    string labeEndLoop = getNextAssemblyLabel();

    // Begin of loop to calculate the length of the string
    assemblyCmd += "\t; Loop para calcular tamanho da string\n";
    assemblyCmd += labelStartLoop + ": ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RDI] ; Leitura de caracter na posicao RSI da memória\n";
    assemblyCmd += "\tmov [RSI], AL \t\t\t; Salva caracter no identificador\n";
    assemblyCmd += "\tcmp AL, 0 ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labeEndLoop + " \n ; Se caracter lido = flag de fim de string finalizar loop\n";
    assemblyCmd += "\tadd RDI, 1 ; Incrementando indice da string1 \n";
    assemblyCmd += "\tadd RSI, 1 ; Incrementando indice da string2 \n";
    assemblyCmd += "\tjmp " + labelStartLoop + "  ; Se caracter lido != flag de fim de string continuar loop\n";

    // End of loop
    assemblyCmd += labeEndLoop + ": ; Fim do loop\n";
}

void getCodeAtribBooleanAndBoolean(long addr1, long addr2)
{
    assemblyCmd += "\tmov EAX, [ M +" + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tmov [M + " + to_string(addr1) + " ] , EAX \t\t\t ; Salva valor do registrador no endereco do ID\n";
}

void getCodeAtribCharAndChar(long addr1, long addr2)
{
    assemblyCmd += "\tmov AL, [ M +" + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificadorg da memoria\n";
    assemblyCmd += "\tmov [ M + " + to_string(addr1) + " ] , AL \t\t\t ; Salva valor do registrador no endereco do ID\n";
}