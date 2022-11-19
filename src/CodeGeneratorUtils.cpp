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
// True memory position for boolean print
long addrStringTrue = null;
// False memory position for boolean print
long addrStringFalse = null;
// Line break memory position for boolean print
long addrStringLineBr = null;

/**
 * @brief Allocate memory for a variable that contains
 * memSize bytes
 *
 * @param long memSize
 */
void memoryAlocation(long memSize)
{
    nextFreePosition += memSize;
}

/**
 * @brief Get current temp position and alloc for next use
 *
 * @param memSize
 * @return int
 */
int newTemp(int memSize)
{
    int aux = assemblyTempCount;
    assemblyTempCount += memSize;

    return aux;
}

/**
 * @brief Reset memory temp count
 *
 */
void resetTempCounter()
{
    assemblyTempCount = 0;
}

/**
 * @brief Get the Next Assembly Label object
 *
 * @return string
 */
string getNextAssemblyLabel()
{
    return "L" + to_string(++assemblyLabel);
}

/**
 * @brief Get the memory size for const type
 *
 * @param type const type
 * @return long size in bytes
 */
long getTypeMemSize(int type)
{
    // INT, FLOAT and BOOLEAN = 4 BYTES
    if (ConstType::INT == type || ConstType::FLOAT == type || ConstType::BOOLEAN == type)
    {
        return 4;
    }
    // STRING = 256 BYTES
    if (ConstType::STRING == type)
    {
        return 256;
    }
    return 1;
}

// **** WRITE SECTION ****
/**
 * @brief Write assembly code for write String
 *
 * @param addr Expression address to be write
 */
void getCodeWriteStr(long addr)
{
    // Labels
    string labelStartLoop = getNextAssemblyLabel();
    string labelEndLoop = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- WRITE STRING -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " \t\t\t ; Copiando endereço da string para um registrador de índice\n";
    assemblyCmd += "\tmov RDX, 0 \t\t\t ; contador de caracteres = 0\n";

    // Begin of loop to calculate the length of the string
    assemblyCmd += "\t; Loop para calcular tamanho da string\n";
    assemblyCmd += labelStartLoop + ": \t\t\t ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RSI] \t\t\t ; Leitura de caractere na posicao rax da memória\n";
    assemblyCmd += "\tcmp AL, 0 \t\t\t ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labelEndLoop + " \t\t\t ; Se caractere lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\tadd RDX, 1 \t\t\t ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementando indice da string\n";
    assemblyCmd += "\tjmp " + labelStartLoop + "  ; Se caractere lido != flag de fim de string continuar loop\n";

    // End of loop
    assemblyCmd += labelEndLoop + ": ; Fim do loop\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " \t\t\t ; Copiando endereço inicial da string\n";
    assemblyCmd += "\tmov RAX, 1 \t\t\t ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 \t\t\t ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";
    assemblyCmd += "\t; -- END WRITE STRING -- \n";
}

/**
 * @brief Write assembly code for write Char
 *
 * @param addr Expression address to be write
 */
void getCodeWriteChar(long addr)
{
    assemblyCmd += "\n\t; -- WRITE CHAR --\n\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " \t\t\t ; Copiando endereço do character para um registrador de índice\n";
    assemblyCmd += "\tmov RDX, 1 \t\t\t ; Quantidade de caracteres imprimíveis \n";

    assemblyCmd += "\tmov RAX, 1 \t\t\t ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 \t\t\t ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";
    assemblyCmd += "\t; -- END WRITE CHAR --\n\n";
}

/**
 * @brief Write assembly code for write Int
 *
 * @param addr Expression address to be write
 */
void getCodeWriteInt(long addr)
{
    // Get new temporary memory position
    long actualMemoryPosition = newTemp(ConstType::STRING);

    // Labels
    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();
    string labelStartLoop = getNextAssemblyLabel();
    string labelEndLoop = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- WRITE INT --\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr) + " ] \t\t\t; Inteiro a ser ;convertido\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(actualMemoryPosition) + " \t\t\t ; End. string ou temp.\n";
    assemblyCmd += "\tmov RCX, 0 \t\t\t ; Contador pilha\n";
    assemblyCmd += "\tmov RDI, 0 \t\t\t ; Tam. string convertido\n";
    assemblyCmd += "\tcmp EAX, 0 \t\t\t ; Verifica sinal\n";
    assemblyCmd += "\tjge " + label0 + " \t\t\t ; Salta se número positivo\n";
    assemblyCmd += "\tmov BL, \'-\' \t\t\t ; Senão, escreve sinal –\n";
    assemblyCmd += "\tmov [RSI], BL\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa índice\n";
    assemblyCmd += "\tadd RDI, 1 \t\t\t ; Incrementa tamanho\n";
    assemblyCmd += "\tneg EAX \t\t\t ; Toma módulo do número\n";
    assemblyCmd += "\t" + label0 + ":\n";
    assemblyCmd += "\tmov EBX, 10 \t\t\t ; Divisor\n";
    assemblyCmd += "\t" + label1 + ":\n";
    assemblyCmd += "\tadd RCX, 1 \t\t\t ; Incrementa contador\n";
    assemblyCmd += "\tcdq \t\t\t ; Estende EDX:EAX p/ div.\n";
    assemblyCmd += "\tidiv EBX \t\t\t ; Divide EDX;EAX por EBX\n";
    assemblyCmd += "\tpush DX \t\t\t ; Empilha valor do resto\n";
    assemblyCmd += "\tcmp EAX, 0 \t\t\t ; Verifica se quoc. é 0\n";
    assemblyCmd += "\tjne " + label1 + " \t\t\t ; Se não é 0, continua\n";
    assemblyCmd += "\tadd RDI, RCX \t\t\t ; Atualiza tam. string\n";
    assemblyCmd += "\t; Agora, desemp. os valores e escreve o string\n";
    assemblyCmd += "\t" + label2 + ":\n";
    assemblyCmd += "\tpop DX \t\t\t ; Desempilha valor\n";
    assemblyCmd += "\tadd DL, \'0\' \t\t\t ; Transforma em caractere\n";
    assemblyCmd += "\tmov [RSI], DL \t\t\t ; Escreve caractere\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa base\n";
    assemblyCmd += "\tsub RCX, 1 \t\t\t ; Decrementa contador\n";
    assemblyCmd += "\tcmp RCX, 0 \t\t\t ; Verifica pilha vazia\n";
    assemblyCmd += "\tjne " + label2 + "\t\t\t ; Se não pilha vazia, loop\n";

    // WRITE STRING CODE
    assemblyCmd += "\tmov RSI, M + " + to_string(actualMemoryPosition) + " \t\t\t ; Copiando endereço da string para um registrador de índice\n";
    assemblyCmd += "\tmov RDX, 0 \t\t\t ; contador de caracteres = 0\n";

    // Begin of loop to calculate the length of the string
    assemblyCmd += "\t; Loop para calcular tamanho da string\n";
    assemblyCmd += labelStartLoop + ": \t\t\t ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RSI] \t\t\t ; Leitura de caractere na posicao rax da memória\n";
    assemblyCmd += "\tcmp AL, 0 \t\t\t ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labelEndLoop + " \t\t\t ; Se caractere lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\tadd RDX, 1 \t\t\t ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementando indice da string\n";
    assemblyCmd += "\tjmp " + labelStartLoop + "  ; Se caractere lido != flag de fim de string continuar loop\n";

    // End of loop
    assemblyCmd += labelEndLoop + ": ; Fim do loop\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(actualMemoryPosition) + " \t\t\t ; Copiando endereço inicial da string\n";
    assemblyCmd += "\tmov RAX, 1 \t\t\t ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 \t\t\t ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";

    assemblyCmd += "\t; -- END WRITE INT --\n";
}

/**
 * @brief Write assembly code for write Float
 *
 * @param addr Expression address to be write
 */
void getCodeWriteFloat(long addr)
{
    // Get new temporary memory position
    long actualMemoryPosition = newTemp(ConstType::STRING);

    // Labels
    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();
    string label3 = getNextAssemblyLabel();
    string label4 = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- WRITE FLOAT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr) + " ] \t\t\t ; Real a ser convertido\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(actualMemoryPosition) + " \t\t\t ; End. temporário\n";
    assemblyCmd += "\tmov RCX, 0 \t\t\t ; Contador pilha\n";
    assemblyCmd += "\tmov RDI, 6 \t\t\t ; Precisão 6 casas compart\n";
    assemblyCmd += "\tmov RBX, 10 \t\t\t ; Divisor\n";
    assemblyCmd += "\tcvtsi2ss XMM2, RBX \t\t\t ; Divisor real\n";
    assemblyCmd += "\tsubss XMM1, XMM1 \t\t\t ; Zera registrador\n";
    assemblyCmd += "\tcomiss XMM0, XMM1 \t\t\t ; Verifica sinal\n";
    assemblyCmd += "\tjae " + label0 + " \t\t\t ; Salta se número positivo\n";
    assemblyCmd += "\tmov DL, \'-\' \t\t\t; Senão, escreve sinal –\n";
    assemblyCmd += "\tmov [RSI], DL\n";
    assemblyCmd += "\tmov RDX, -1 \t\t\t ; Carrega -1 em RDX\n";
    assemblyCmd += "\tcvtsi2ss XMM1, RDX \t\t\t ; Converte para real\n";
    assemblyCmd += "\tmulss XMM0, XMM1 \t\t\t ; Toma módulo\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa índice\n";
    assemblyCmd += label0 + ": \n";
    assemblyCmd += "\troundss XMM1, XMM0, 0b0011 \t\t\t ; Parte inteira XMM1\n";
    assemblyCmd += "\tsubss XMM0, XMM1 \t\t\t ; Parte frac XMM0\n";
    assemblyCmd += "\tcvtss2si rax, XMM1 \t\t\t ; Convertido para int\n";
    assemblyCmd += "\t; Converte parte inteira que está em rax\n";
    assemblyCmd += label1 + ": \n";
    assemblyCmd += "\tadd RCX, 1 \t\t\t ; Incrementa contador\n";
    assemblyCmd += "\tcdq \t\t\t ; Estende EDX:EAX p/ div.\n";
    assemblyCmd += "\tidiv EBX \t\t\t ; Divide EDX;EAX por EBX\n";
    assemblyCmd += "\tpush DX \t\t\t ; Empilha valor do resto\n";
    assemblyCmd += "\tcmp EAX, 0 \t\t\t ; Verifica se quoc. é 0\n";
    assemblyCmd += "\tjne " + label1 + " \t\t\t ; Se não é 0, continua\n";
    assemblyCmd += "\tsub RDI, RCX \t\t\t;decrementa precisao\n";
    assemblyCmd += "\t; Agora, desemp valores e escreve parte int\n";
    assemblyCmd += label2 + ":\n";
    assemblyCmd += "\tpop DX \t\t\t ; Desempilha valor\n";
    assemblyCmd += "\tadd DL, \'0\' \t\t\t ; Transforma em caractere\n";
    assemblyCmd += "\tmov [RSI], DL \t\t\t ; Escreve caractere\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa base\n";
    assemblyCmd += "\tsub RCX, 1 \t\t\t ; Decrementa contador\n";
    assemblyCmd += "\tcmp RCX, 0 \t\t\t ; Verifica pilha vazia\n";
    assemblyCmd += "\tjne " + label2 + " \t\t\t ; Se não pilha vazia, loop\n";
    assemblyCmd += "\tmov DL, \'.\' \t\t\t ; Escreve ponto decimal\n";
    assemblyCmd += "\tmov [RSI], DL\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa base\n";
    assemblyCmd += "\t; Converte parte fracionaria que está em XMM0\n";
    assemblyCmd += label3 + ":\n";
    assemblyCmd += "\tcmp RDI, 0 \t\t\t ; Verifica precisao\n";
    assemblyCmd += "\tjle " + label4 + " \t\t\t ; Terminou precisao ?\n";
    assemblyCmd += "\tmulss XMM0,XMM2 \t\t\t ; Desloca para esquerda\n";
    assemblyCmd += "\troundss XMM1,XMM0,0b0011 \t\t\t ; Parte inteira XMM1\n";
    assemblyCmd += "\tsubss XMM0,XMM1 \t\t\t ; Atualiza XMM0\n";
    assemblyCmd += "\tcvtss2si RDX, XMM1\t\t\t ; Convertido para int\n";
    assemblyCmd += "\tadd DL, \'0\' \t\t\t ; Transforma em caractere\n";
    assemblyCmd += "\tmov [RSI], DL \t\t\t ; Escreve caractere\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa base\n";
    assemblyCmd += "\tsub RDI, 1 \t\t\t ; Decrementa precisão\n";
    assemblyCmd += "\tjmp " + label3 + "\n";
    assemblyCmd += "\t; Impressão\n";
    assemblyCmd += label4 + ":\n";
    assemblyCmd += "\tmov DL, 0 \t\t\t ; Fim string, opcional\n";
    assemblyCmd += "\tmov [RSI], DL \t\t\t ; Escreve caractere\n";
    assemblyCmd += "\tmov RDX, RSI ; Calc tam str convertido\n";
    assemblyCmd += "\tmov RBX, M + " + to_string(actualMemoryPosition) + " \n";
    assemblyCmd += "\tsub RDX, RBX \t\t\t ; Tam=RSI-M-buffer.end\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(actualMemoryPosition) + " \t\t\t; Endereço do buffer\n";

    assemblyCmd += "\tmov RAX, 1 ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";
    assemblyCmd += "\t; -- END WRITE FLOAT -- \n";
}

/**
 * @brief Write assembly code for write Boolean
 *
 * @param addr Expression address to be write
 */
void getCodeWriteBoolean(long addr)
{
    // Labels
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    // Verify if true and false is already declared on .data section
    // And if not, do it
    if (addrStringTrue == null)
    {
        addrStringTrue = nextFreePosition;
        assemblyDec += "\tdb \"true\" , 0 \t\t\t ; Declaração String TRUE em [" + to_string(addrStringTrue) + "]\n";
        // Memory Allocation in data area for "true"
        memoryAlocation(5);

        addrStringFalse = nextFreePosition;
        assemblyDec += "\tdb \"false\" , 0 \t\t\t ; Declaração String FALSE em [" + to_string(addrStringFalse) + "]\n";
        // Memory Allocation in data area for "false"
        memoryAlocation(6);
    }

    assemblyCmd += "\n\t; -- WRITE BOOLEAN -- \n";
    assemblyCmd += "\tmov EAX , [ M + " + to_string(addr) + " ] \t\t\t ; Carrega valor de boolean\n";
    assemblyCmd += "\tcmp EAX , 0 \t\t\t ; Se boolean for falso\n";
    assemblyCmd += "\tjne " + labelTrue + "\t\t\t; Se boolean for verdadeiro -> label true\n";
    assemblyCmd += "\n\t; -- END WRITE BOOLEAN -- \n";

    // Print false
    getCodeWriteStr(addrStringFalse);
    assemblyCmd += "\tjmp " + labelEnd + "\t\t\t; Jump end\n";

    // Print true
    assemblyCmd += labelTrue + ": \n";
    getCodeWriteStr(addrStringTrue);

    assemblyCmd += labelEnd + ": \n";
}

/**
 * @brief Write assembly code for line break
 *
 */
void getCodeWriteLineBr()
{
    // Alloc temp memory for buffer (1 byte)
    long buffer = newTemp(ConstType::CHAR);

    assemblyCmd += "\t; -- LINE BREAK -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + "\n";
    assemblyCmd += "\tmov [RSI], byte 10 \t\t\t ; Move caracter quebra de linha para o endereço armazenado em RSI\n";
    assemblyCmd += "\tmov RDX, 1 \t\t\t; Ou buffer.tam\n";
    assemblyCmd += "\tmov RAX, 1 \t\t\t; Chamada para saida\n";
    assemblyCmd += "\tmov RDI, 1 \t\t\t; Saida para tela\n";
    assemblyCmd += "\tsyscall\n";
    assemblyCmd += "\t; -- END LINE BREAK -- \n";
}

/**
 * @brief Write assembly code for write type
 *
 * @param addr Expression address to be write
 * @param type ConsType value to print
 */
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

// **** DECONST SECTION ****

/**
 * @brief Write assembly code for DECONST grammar variable
 *
 * @param hasMinnus If the number is negative
 * @param type Const.type
 * @param stringValue Const.lex
 * @return long Memory position of const value
 */
long getCodeDeconst(bool hasMinnus, int type, string stringValue)
{
    // Alloc free memory
    long actualMemoryPosition = nextFreePosition;

    if (type == ConstType::CHAR)
    {
        assemblyDec += "\tdb " + stringValue + ", 1 \t\t\t ; Declaracao Const Char em [ " + to_string(actualMemoryPosition) + " ]\n";
    }
    else if (type == ConstType::INT)
    {
        assemblyDec += "\tdd ";

        // - CONST
        if (hasMinnus)
        {
            assemblyDec += "-";
        }

        assemblyDec += stringValue + " \t\t\t ; Declaracao Const Inteiro em [ " + to_string(actualMemoryPosition) + " ]\n";
    }
    else if (type == ConstType::STRING)
    {
        assemblyDec += "\tdb " + stringValue + ",0 \t\t\t ; Declaracao Const String em [ " + to_string(actualMemoryPosition) + " ]\n";
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

        assemblyDec += stringValue + "\t\t\t; Declaracao Const Float em [ " + to_string(actualMemoryPosition) + " ]\n";
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
        assemblyDec += "\tdd " + stringValue + "\t\t\t; Declaracao Const Boolean em [ " + to_string(actualMemoryPosition) + " ]\n";
    }

    // Alloc string size if type is String
    if (type != ConstType::STRING)
    {
        memoryAlocation(getTypeMemSize(type));
    }
    else
    {
        memoryAlocation(stringValue.length() - 1);
    }

    return actualMemoryPosition;
}

// **** M SECTION ****
/**
 * @brief Write assembly code for Const value in M  grammar variable
 *
 * @param stringValue Const.lex
 * @param type Const.type
 * @return long Memory position allocated
 */
long getCodeExpConst(string stringValue, int type)
{
    long actualMemoryPosition;

    // If type is String, main memory is allocated
    if (type == ConstType::STRING)
    {
        actualMemoryPosition = nextFreePosition;
        assemblyDec += "\tdb " + stringValue + ", 0 \t\t\t ; Declaracao String M em [ " + to_string(actualMemoryPosition) + " ]\n";
        // Memory Allocation in data area for String
        memoryAlocation(stringValue.length() - 1);
    }
    // If type is Float, main memory is allocated
    else if (type == ConstType::FLOAT)
    {
        actualMemoryPosition = nextFreePosition;

        assemblyDec += "\tdd ";
        if (stringValue[0] == '.')
        {
            assemblyDec += " 0";
        }

        assemblyDec += stringValue + " \t\t\t ; Declaracao Float M em [ " + to_string(actualMemoryPosition) + " ]\n";
        // Memory Allocation in data area for Float
        memoryAlocation(getTypeMemSize(type));
    }
    // For others types, temporary memory is allocated
    else
    {
        actualMemoryPosition = assemblyTempCount;

        if (type == ConstType::INT)
        {
            assemblyCmd += "\n\t; -- CONST INT -- \n";
            assemblyCmd += "\tmov EAX, " + stringValue + " \t\t\t ; Move Inteiro imediato para registrador\n";
            assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Move registrador para memoria temporaria\n";
            assemblyCmd += "\t; -- END CONST INT -- \n";
        }
        else if (type == ConstType::CHAR)
        {
            assemblyCmd += "\n\t; -- CONST CHAR -- \n";
            assemblyCmd += "\tmov AL, " + stringValue + " \t\t\t ; Move Char imediato para registrador\n";
            assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], AL \t\t\t ; Move registrador para memoria temporaria\n";
            assemblyCmd += "\t; -- END CONST CHAR -- \n";
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

            assemblyCmd += "\n\t; -- CONST BOOLEAN -- \n";
            assemblyCmd += "\tmov EAX, " + stringValue + "\t\t\t ; Move Boolean imediado para registrador EAX\n";
            assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Move valor do registrador para memoria temporaria\n";
            assemblyCmd += "\t; -- END CONST BOOLEAN -- \n";
        }

        // Temporary Memory alloc
        newTemp(getTypeMemSize(type));
    }

    return actualMemoryPosition;
}

/**
 * @brief Write assembly code for NOT Exp in M grammar variable
 *
 * @param addr Expression address to be write
 * @param type Expression type
 * @return long Temporary memory position
 */
long getCodeNotExp(long addr, int type)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- NOT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr) + " ] \t\t\t ; Move o valor da memoria para o registrador \n";
    assemblyCmd += "\tneg EAX \t\t\t ; Nega o valor que esta no registrador\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t ; Nega o valor que esta no registrador\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Move o valor do registrador para memoria temporaria\n";
    assemblyCmd += "\t; -- END NOT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(type));

    return actualMemoryPosition;
}

/**
 * @brief Write assembly code for String Position
 *
 * @param addrString String addr
 * @param addrIndex Index of string
 * @return long Temporary memory position for index value
 */
long getCodeAccessStringPosition(long addrString, long addrIndex)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- STRING POSITION -- \n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addrIndex) + " ] \t\t\t ; Recupera valor de indice da string da memoria\n";
    assemblyCmd += "\tadd EBX, M + " + to_string(addrString) + " \t\t\t ; Soma indice com valor de memoria da string\n";
    assemblyCmd += "\tmov EAX, [EBX] \t\t\t ; Escreve valor de caracter para o registrador AL\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva character em temporario\n";
    assemblyCmd += "\t; -- END STRING POSITION -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::CHAR));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for casting command - float(int) || int(float)
 *
 * @param addr Address of the variable to be converted
 * @param convType Type to be converted
 * @return long Converted number temporary position
 */
long getCodeCasting(long addr, int convType)
{

    long actualMemoryPosition = assemblyTempCount;

    // Casting float -> int
    if (convType == ConstType::INT)
    {

        assemblyCmd += "\n\t; -- CASTING INT -- \n";
        assemblyCmd += "\tsubss XMM0, XMM0 ; Zera o XMM0\n";
        assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr) + " ] \t\t\t; Move o valor de float da memoria para o registrador XMM0\n";
        assemblyCmd += "\tmov RAX, 0 ; Zera o RAX\n";

        assemblyCmd += "\tcvtss2si RAX, XMM0 \t\t\t; Converte int para float\n";
        assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], RAX \t\t\t ; Salva valor convertido para temporario\n";
        assemblyCmd += "\t; -- END CASTING INT -- \n";

        // Temporary Memory alloc
        newTemp(getTypeMemSize(ConstType::INT));

        // Casting int -> float
    }
    else if (convType == ConstType::FLOAT)
    {

        assemblyCmd += "\n\t; -- CASTING FLOAT -- \n";
        assemblyCmd += "\tmov EAX, 0 ; Zera o EAX\n";
        assemblyCmd += "\tmov RAX, 0 ; Zera o RAX\n";
        assemblyCmd += "\tmov EAX, [ M + " + to_string(addr) + " ] \t\t\t; Move o valor de int da memoria para o registrador RAX\n";
        assemblyCmd += "\tcdqe \t\t\t; Expandindo o sinal de valor em RAX\n";
        assemblyCmd += "\tsubss XMM0, XMM0 ; Zera o XMM0\n";
        assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t; Converte float para int\n";
        assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ; Salva valor convertido para temporario\n";
        assemblyCmd += "\t; -- END CASTING FLOAT -- \n";

        // Temporary Memory alloc
        newTemp(getTypeMemSize(ConstType::FLOAT));
    }

    return actualMemoryPosition;
}

// **** R SECTION ****

/**
 * @brief Generate code for times operation for two float
 * variables - (FLOAT * FLOAT)
 *
 * @param addr1 Float 1 address
 * @param addr2 Float 2 address
 * @return long Temporary variable address
 */
long getCodeTimesOperationtForFloat(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT * FLOAT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float 1 da memoria para registrador\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de float 2 da memoria para registrador\n";

    assemblyCmd += "\tmulss XMM0, XMM1  \t\t\t ; Float1 * Float2\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT * FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for times operation for one float and one integer
 * variables - (INT * FLOAT) || (FLOAT * INT)
 *
 * @param addr1 Float address
 * @param addr2 Integer address
 * @return long Temporary variable address
 */
long getCodeTimesOperationtForFloatAndInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT * INT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float da memoria para registrador\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int da memoria para registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em EAX\n";

    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t ; Expande int para float\n";
    assemblyCmd += "\tmulss XMM0, XMM1  \t\t\t ; Float * Float(int)\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT * INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for times operation for two int
 * variables - (INT * INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @return long Temporary variable address
 */
long getCodeTimesOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT * INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int 1 da memoria para registrador \n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int 2 da memoria para registrador \n";

    assemblyCmd += "\timul EBX \t\t\t ; Int1 * Int2\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT * INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::INT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for div operation for two int
 * variables - (INT DIV INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @return long Temporary variable address
 */
long getCodeDivOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT DIV INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int 1 da memoria para registrador \n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int 2 da memoria para registrador \n";

    assemblyCmd += "\tidiv EBX \t\t\t ; int1 div int2\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT DIV INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::INT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for two float
 * variables - (FLOAT / FLOAT)
 *
 * @param addr1 Float 1 address
 * @param addr2 Float 2 address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForFloat(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT / FLOAT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de float 1 da memoria para registrador\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de float 2 da memoria para registrador\n";

    assemblyCmd += "\tdivss XMM0, XMM1 \t\t\t; float1 / float2\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT / FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for two float
 * variables - (FLOAT / INT)
 *
 * @param addr1 Float address
 * @param addr2 Int address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForFloatAndInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT / INT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float da memoria para registrador\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int da memoria para registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em EAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t ; Expande int para float\n";
    assemblyCmd += "\tdivss XMM0, XMM1 \t\t\t ; float / float(int)\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT / INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for int and float
 * variables - (INT / FLOAT)
 *
 * @param addr1 Int address
 * @param addr2 Float address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForIntAndFloat(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT / FLOAT -- \n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de float da memoria para registrador\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int da memoria para registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em EAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t ; Expande int para float\n";
    assemblyCmd += "\tdivss XMM0, XMM1 \t\t\t ; float(int) / float\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT / FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for divide operation for two int
 * variables - (INT / INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @return long Temporary variable address
 */
long getCodeDivideOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT / INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int 1 da memoria para registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em EAX\n";
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t ; Expande int1 para float1\n";

    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int 2 da memoria para registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em EAX\n";
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t ; Expande int2 para float2\n";

    assemblyCmd += "\tdivss XMM0, XMM1 \t\t\t ; float(int1) / float(int2)\n";
    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT / INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::INT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for mod operation for two int
 * variables - (INT MOD INT)
 *
 * @param addr1 int 1 address
 * @param addr2 int 2 address
 * @return long Temporary variable address
 */
long getCodeModOperationtForInt(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT MOD INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int 1 da memoria para o registrador\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int 2 da memoria para o registrador\n";

    assemblyCmd += "\tidiv EBX \t\t\t ; int1 mod int2\n";
    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EDX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT MOD INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::INT));

    return actualMemoryPosition;
}

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
long getCodePlusMinnusForFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT (+|-) FLOAT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float 1 da memoria para registrador\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de float 2 da memoria para registrador\n";

    // +
    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\taddss XMM0, XMM1 \t\t\t ; float1 + float2\n";
    }
    // -
    else
    {
        assemblyCmd += "\tsubss XMM0, XMM1 \t\t\t ; float1 - float2\n";
    }

    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT (+|-) FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for plus and minnus operation for float and int
 * variables - (FLOAT +- INT)
 *
 * @param addr1 float 1 address
 * @param addr2 float 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForFloatAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT (+|-) INT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float da memoria para registrador\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int da memoria para o registrador\n";

    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em EAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t ; Expande int para float\n";

    // +
    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\taddss XMM0, XMM1 \t\t\t ; float1 + float2(int)\n";
    }
    // -
    else
    {
        assemblyCmd += "\tsubss XMM0, XMM1 \t\t\t ; float1 - float2(int)\n";
    }

    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT (+|-) INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for plus and minnus operation for int and float
 * variables - (INT +- FLOAT)
 *
 * @param addr1 Float 1 address
 * @param addr2 Float 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForIntAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT (+|-) FLOAT -- \n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de float da memoria para registrador\n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int da memoria para o registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, EAX \t\t\t ; Expande int para float\n";

    // +
    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\taddss XMM0, XMM1 \t\t\t ; float1(int) + float2\n";
    }
    // -
    else
    {
        assemblyCmd += "\tsubss XMM0, XMM1 \t\t\t ; float1(int) - float2\n";
    }

    assemblyCmd += "\tmovss [ M + " + to_string(actualMemoryPosition) + " ], XMM0 \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT (+|-) FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::FLOAT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for plus and minnus operation for ints
 * variables - (INT +- INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @param operation Compare operation
 * @return long Temporary variable address
 */
long getCodePlusMinnusForInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT (+|-) INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int 1 da memoria para registrador\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int 2 da memoria para registrador\n";

    // +
    if (operation == Alphabet::PLUS)
    {
        assemblyCmd += "\tadd EAX, EBX \t\t\t ; int1 + int2\n";
    }
    // -
    else
    {
        assemblyCmd += "\tsub EAX, EBX \t\t\t ; int1 - int2\n";
    }

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT (+|-) INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::INT));

    return actualMemoryPosition;
}

/**
 * @brief Generate code for logic OR between booleans - (BOOLEAN || BOOLEAN)
 *
 * @param addr1 Boolean 1 address
 * @param addr2 Boolean 2 address
 * @return long Temporary variable address
 */
long getCodeForOR(long addr1, long addr2)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- OR -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de boolean 1 da memoria para registrador\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de boolean 2 da memoria para registrador\n";
    // Not boolean 1
    assemblyCmd += "\tneg EAX \t\t\t ; EAX = !EAX\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t ; EAX = !EAX\n";
    // Not boolean 2
    assemblyCmd += "\tneg EBX \t\t\t ; EBX = !EBX\n";
    assemblyCmd += "\tadd EBX, 1 \t\t\t ; EBX = !EBX\n";
    // Not boolean 1 And Not boolean 2
    assemblyCmd += "\timul EBX \t\t\t ; !EAX AND !EBX\n";
    //  Not (Not boolean 1 And Not boolean 2) = Boolean 1 Or Boolean 2
    assemblyCmd += "\tneg EAX \t\t\t ; !(!EAX AND !EBX) = EAX OR EBX\n";
    assemblyCmd += "\tadd EAX, 1 \t\t\t ; !(!EAX AND !EBX) = EAX OR EBX\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END OR -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

// **** EXP SECTION ****

/**
 * @brief Get the compare code between floats - (FLOAT >|>=|=|<=|<|!= FLOAT)
 *
 * @param operation Comparation operation
 * @return string Assembly code for compare operation
 */
void getCmpCodeFloat(int operation)
{
    // Labels
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tcomiss XMM0, XMM1 \t\t\t ; Compara float1 com float2\n";

    // =
    if (operation == Alphabet::EQUAL)
    {
        assemblyCmd += "\tje " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se float1 = float2\n";
    }
    // >
    else if (operation == Alphabet::GREATERTHAN)
    {
        assemblyCmd += "\tja " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se float1 > float2\n";
    }
    // >=
    else if (operation == Alphabet::GREATEREQUAL)
    {
        assemblyCmd += "\tjae " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se float1 >= float2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN)
    {
        assemblyCmd += "\tjb " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se float1 < float2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL)
    {
        assemblyCmd += "\tjbe " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se float1 <= float2\n";
    }
    // !=
    else if (operation == Alphabet::NOTEQUAL)
    {
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se float1 != float2\n";
    }

    assemblyCmd += "\tmov EAX, 0 \t\t\t ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 \t\t\t ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";
}

/**
 * @brief Get the compare code between ints - (INT >|>=|=|<=|<|!= INT)
 *
 * @param operation Comparation operation
 * @return string Assembly code for compare operation
 */
void getCmpCodeInt(int operation)
{
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tcmp EAX, EBX \t\t\t ; Compara int1 com int2\n";
    // =
    if (operation == Alphabet::EQUAL)
    {
        assemblyCmd += "\tje " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 = int2\n";
    }
    // >
    else if (operation == Alphabet::GREATERTHAN)
    {
        assemblyCmd += "\tjg " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 > int2\n";
    }
    // >=
    else if (operation == Alphabet::GREATEREQUAL)
    {
        assemblyCmd += "\tjge " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 >= int2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN)
    {
        assemblyCmd += "\tjl " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 < int2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL)
    {
        assemblyCmd += "\tjle " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 <= int2\n";
    }
    // !=
    else if (operation == Alphabet::NOTEQUAL)
    {
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 != int2\n";
    }

    assemblyCmd += "\tmov EAX, 0 \t\t\t ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 \t\t\t ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";
}

/**
 * @brief Get the assembly compare code between Chars - (CHAR >|>=|=|<=|<|!= CHAR)
 *
 * @param addr1 Char 1 address
 * @param addr2 Char 2 address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForCharAndChar(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- CHAR CMP CHAR -- \n";
    assemblyCmd += "\tmov AL, [ M + " + to_string(addr1) + " ] \t\t\t; Move o valor de char 1 da memoria para registrador\n";
    assemblyCmd += "\tmov BL, [ M + " + to_string(addr2) + " ] \t\t\t; Move o valor de char 2 da memoria para registrador\n";
    assemblyCmd += "\tcmp AL, BL \t\t\t ; compara caracteres\n";

    if (operation == Alphabet::EQUAL)
    {
        assemblyCmd += "\tje " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 = int2\n";
    }
    // >
    else if (operation == Alphabet::GREATERTHAN)
    {
        assemblyCmd += "\tjg " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 > int2\n";
    }
    // >=
    else if (operation == Alphabet::GREATEREQUAL)
    {
        assemblyCmd += "\tjge " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 >= int2\n";
    }
    // <
    else if (operation == Alphabet::LESSTHAN)
    {
        assemblyCmd += "\tjl " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 < int2\n";
    }
    // <=
    else if (operation == Alphabet::LESSEQUAL)
    {
        assemblyCmd += "\tjle " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 <= int2\n";
    }
    // !=
    else if (operation == Alphabet::NOTEQUAL)
    {
        assemblyCmd += "\tjne " + labelTrue + "\t\t\t ; Salta para " + labelTrue + " se int1 != int2\n";
    }

    assemblyCmd += "\tmov EAX, 0 \t\t\t ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 \t\t\t ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END CHAR CMP CHAR -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

/**
 * @brief Get the assembly compare code between Ints - (INT >|>=|=|<=|<|!= INT)
 *
 * @param addr1 Int 1 address
 * @param addr2 Int 2 address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForIntAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT " + tokenToString(operation) + " INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int 1 da memoria para registrador\n";
    assemblyCmd += "\tmov EBX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int 2 da memoria para registrador\n";

    getCmpCodeInt(operation);

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT " + tokenToString(operation) + " INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

/**
 * @brief Get the assembly compare code between Int and Float - (INT >|>=|=|<=|<|!= FLOAT)
 *
 * @param addr1 Int address
 * @param addr2 Float address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForIntAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- INT " + tokenToString(operation) + " FLOAT -- \n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de float da memoria para registrador\n";
    assemblyCmd += "\tmov RAX, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de int da memoria para registrador\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t ; Expande int para float\n";

    getCmpCodeFloat(operation);

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END INT " + tokenToString(operation) + " FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

/**
 * @brief Get the assembly compare code between Float and Int - (FLOAT >|>=|=|<=|<|!= INT)
 *
 * @param addr1 Float address
 * @param addr2 Int address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForFloatAndInt(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT " + tokenToString(operation) + " INT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float da memoria para registrador \n";
    assemblyCmd += "\tmov RAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de int da memoria para registrador\n";

    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t ; Converte int para float\n";
    getCmpCodeFloat(operation);

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT " + tokenToString(operation) + " INT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

/**
 * @brief Get the assembly compare code between Floats - (FLOAT >|>=|=|<=|<|!= FLOAT)
 *
 * @param addr1 Float 1 addr
 * @param addr2 Float 2 addr
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForFloatAndFloat(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    assemblyCmd += "\n\t; -- FLOAT " + tokenToString(operation) + " FLOAT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr1) + " ] \t\t\t ; Move o valor de float 1 da memoria para o registrador XMM0\n";
    assemblyCmd += "\tmovss XMM1, [ M + " + to_string(addr2) + " ] \t\t\t ; Move o valor de float 2 da memoria para o registrador XMM1\n";

    getCmpCodeFloat(operation);

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ;  Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END FLOAT " + tokenToString(operation) + " FLOAT -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

/**
 * @brief Get the assembly compare code between Strings - (STRING = STRING)
 *
 * @param addr1 String 1 address
 * @param addr2 String 2 address
 * @param operation Comparation operation
 * @return long Temporary variable address
 */
long getCodeCmpForStringAndString(long addr1, long addr2, int operation)
{
    long actualMemoryPosition = assemblyTempCount;

    // Labels
    string labelStartLoop = getNextAssemblyLabel();
    string labelEndLoop = getNextAssemblyLabel();
    string labelFalse = getNextAssemblyLabel();
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- STRING " + tokenToString(operation) + " STRING -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr1) + " ; \t\t\t Movendo string1 da memória para registrador\n";
    assemblyCmd += "\tmov RDI, M + " + to_string(addr2) + " ; \t\t\t Movendo string2 da memória para registrador\n";

    assemblyCmd += labelStartLoop + ": \t\t\t ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RSI] \t\t\t ; Leitura de caracter na posicao RSI da memória\n";
    assemblyCmd += "\tmov BL, [RDI] \t\t\t ; Leitura de caracter na posicao RDI da memória\n";
    assemblyCmd += "\tcmp AL, 0 \t\t\t ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labelEndLoop + " \n \t\t\t ; Se caracter lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\tcmp AL, BL \t\t\t ; Verificação de igualdade de caracter\n";
    assemblyCmd += "\tjne " + labelFalse + " \n \t\t\t ; Se string1[i] != string2[i]\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tadd RDI, 1 \t\t\t ; Incrementando numero de caracteres\n";
    assemblyCmd += "\tjmp " + labelStartLoop + "  \t\t\t ; Se caracter lido != flag de fim de string continuar loop\n";

    assemblyCmd += labelEndLoop + ":  \t\t\t ; Fim do loop\n";
    assemblyCmd += "\tcmp BL, 0  \t\t\t ; Verificação de flag de fim de string\n";

    assemblyCmd += "\tje " + labelTrue + " \t\t\t ; Se caracter lido = flag de fim de string finalizar loop\n";

    assemblyCmd += "\n" + labelFalse + ":\n";
    assemblyCmd += "\tmov EAX, 0 \t\t\t ; Define registrador como falso\n";
    assemblyCmd += "\tjmp " + labelEnd;

    assemblyCmd += "\n" + labelTrue + ":\n";
    assemblyCmd += "\tmov EAX, 1 \t\t\t ; Define registrador como true\n";

    assemblyCmd += labelEnd + ":\n";

    assemblyCmd += "\tmov [ M + " + to_string(actualMemoryPosition) + " ], EAX \t\t\t ; Salva resultado em temporario\n";
    assemblyCmd += "\t; -- END STRING " + tokenToString(operation) + " STRING -- \n";

    // Temporary Memory alloc
    newTemp(getTypeMemSize(ConstType::BOOLEAN));

    return actualMemoryPosition;
}

// **** ATRIB SECTION ****

/**
 * @brief Generate code for atribuition of char to a string position - (STRING[X] := CHAR)
 *
 * @param addrID address of string that will receive the value
 * @param addrIndex address of the index value
 * @param addrAtrib address of chat contains the value
 */
void getCodeAtribStringPos(long addrID, long addrIndex, long addrAtrib)
{
    assemblyCmd += "\n\t; -- ATRIB STRING POS := CHAR -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addrIndex) + " ] \t\t\t ; Recupera valor de indice da string da memoria\n";
    assemblyCmd += "\tmov EBX, M + " + to_string(addrAtrib) + " \t\t\t ; Move endereco de memoria da string para registrador\n";
    assemblyCmd += "\tmov ECX, M + " + to_string(addrID) + " \t\t\t ; Move endereco de memoria da string para registrador\n";
    assemblyCmd += "\tadd EAX, ECX \t\t\t ; Adiciona indice ao endereco inicial da memoria\n";
    assemblyCmd += "\tmov BL, [EBX] \t\t\t ; Escreve valor de caracter para o registrador\n";

    assemblyCmd += "\tmov [ EAX ], BL \t\t\t; Salva character no endereco do identificador\n";
    assemblyCmd += "\t; -- END ATRIB STRING POS := CHAR -- \n";
}

/**
 * @brief Generate code for atribuition of int to int - (INT := INT)
 *
 * @param addr1 ID address (address of int that will receive the value)
 * @param addr2 EXP address (address of int contains the value)
 */
void getCodeAtribIntAndInt(long addr1, long addr2)
{
    assemblyCmd += "\n\t; -- ATRIB INT := INT -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tmov [ M + " + to_string(addr1) + " ] , EAX \t\t\t ; Salva valor do registrador no endereco do ID\n";
    assemblyCmd += "\t; -- END ATRIB INT := INT -- \n";
}

/**
 * @brief Generate code for atribuition of float to float - (FLOAT := FLOAT)
 *
 * @param addr1 ID address (address of float that will receive the value)
 * @param addr2 EXP address (address of float contains the value)
 */
void getCodeAtribFloatAndFloat(long addr1, long addr2)
{
    assemblyCmd += "\n\t; -- ATRIB FLOAT := FLOAT -- \n";
    assemblyCmd += "\tmovss XMM0, [ M + " + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tmovss [ M + " + to_string(addr1) + " ] , XMM0 \t\t\t ; Salva valor do registrador no endereco do ID\n";
    assemblyCmd += "\t; -- END ATRIB FLOAT := FLOAT -- \n";
}

/**
 * @brief Generate code for atribuition of int to float - (FLOAT := INT)
 *
 * @param addr1 ID address (address of float that will receive the value)
 * @param addr2 EXP address (address of int contains the value)
 */
void getCodeAtribFloatAndInt(long addr1, long addr2)
{
    assemblyCmd += "\n\t; -- ATRIB FLOAT := INT -- \n";
    assemblyCmd += "\tmov RAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tcdqe \t\t\t ; Expandindo o sinal de valor em RAX\n";
    // Type conversion
    assemblyCmd += "\tcvtsi2ss XMM0, RAX \t\t\t; Converte int para float\n";

    assemblyCmd += "\tmovss [ M + " + to_string(addr1) + " ], XMM0 \t\t\t;  Salva resultado no endereco do identificador\n";
    assemblyCmd += "\t; -- END ATRIB FLOAT := INT -- \n";
}

/**
 * @brief Generate code for atribuition of string to string - (STRING := STRING)
 *
 * @param addr1 ID address (address of string that will receive the value)
 * @param addr2 EXP address (address of string contains the value)
 */
void getCodeAtribStringAndString(long addr1, long addr2)
{
    assemblyCmd += "\n\t; -- ATRIB STRING := STRING -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr1) + " \t\t\t ; Copiando endereço da string para um registrador de índice\n";
    assemblyCmd += "\tmov RDI, M + " + to_string(addr2) + " \t\t\t ; Copiando endereço da string para um registrador de índice\n";

    string labelStartLoop = getNextAssemblyLabel();
    string labeEndLoop = getNextAssemblyLabel();

    // Begin of loop to calculate the length of the string
    assemblyCmd += "\t; Loop para calcular tamanho da string\n";
    assemblyCmd += labelStartLoop + ": \t\t\t ; Inicio do loop\n";
    assemblyCmd += "\tmov AL, [RDI] \t\t\t ; Leitura de caracter na posicao RSI da memória\n";
    assemblyCmd += "\tmov [RSI], AL \t\t\t; Salva caracter no identificador\n";
    assemblyCmd += "\tcmp AL, 0 \t\t\t ; Verificação de flag de fim de string\n";
    assemblyCmd += "\tje " + labeEndLoop + " \n ; Se caracter lido = flag de fim de string finalizar loop\n";
    assemblyCmd += "\tadd RDI, 1 \t\t\t ; Incrementando indice da string1 \n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementando indice da string2 \n";
    assemblyCmd += "\tjmp " + labelStartLoop + " \t\t\t ; Se caracter lido != flag de fim de string continuar loop\n";

    // End of loop
    assemblyCmd += labeEndLoop + ": ; Fim do loop\n";
    assemblyCmd += "\t; -- END ATRIB STRING := STRING -- \n";
}

/**
 * @brief Generate code for atribuition of boolean to boolean - (BOOLEAN := BOOLEAN)
 *
 * @param addr1 ID address (address of boolean that will receive the value)
 * @param addr2 EXP address (address of boolean contains the value)
 */
void getCodeAtribBooleanAndBoolean(long addr1, long addr2)
{
    assemblyCmd += "\n\t; -- ATRIB BOOLEAN := BOOLEAN -- \n";
    assemblyCmd += "\tmov EAX, [ M + " + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificador da memoria\n";
    assemblyCmd += "\tmov [ M + " + to_string(addr1) + " ] , EAX \t\t\t ; Salva valor do registrador no endereco do ID\n";
    assemblyCmd += "\t; -- END ATRIB BOOLEAN := BOOLEAN -- \n";
}

/**
 * @brief Generate code for atribuition of char to char - (CHAR := CHAR)
 *
 * @param addr1 ID address (address of char that will receive the value)
 * @param addr2 EXP address (address of char contains the value)
 */
void getCodeAtribCharAndChar(long addr1, long addr2)
{
    assemblyCmd += "\n\t; -- ATRIB CHAR := CHAR -- \n";
    assemblyCmd += "\tmov AL, [ M + " + to_string(addr2) + " ] \t\t\t ; Recupera valor do identificadorg da memoria\n";
    assemblyCmd += "\tmov [ M + " + to_string(addr1) + " ] , AL \t\t\t ; Salva valor do registrador no endereco do ID\n";
    assemblyCmd += "\t; -- END ATRIB CHAR := CHAR -- \n";
}

// **** READ SECTION ****

/**
 * @brief Generate code for read string command
 *
 * @param addr (address of the string variable to receive the read value)
 */
void getCodeReadStr(long addr)
{
    string labelLoop = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    // Init assembly code read (input)
    assemblyCmd += "\n\t; -- READ STR -- \n";
    assemblyCmd += "\tmov RSI, M+" + to_string(addr) + " \t\t\t ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RDX, 100h \t\t\t ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RAX, 0 \t\t\t ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 \t\t\t ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";

    assemblyCmd += "; Leitura string\n";
    assemblyCmd += "\tadd RAX, RSI\n";
    assemblyCmd += "\tsub RAX, 1\n";

    assemblyCmd += labelLoop + ":\n";
    assemblyCmd += "\tmov DL, [RSI] \t\t\t ; Move caractere para \n";
    assemblyCmd += "\tcmp DL, byte 10 \t\t\t ; Compara com quebra de linha\n";
    assemblyCmd += "\tje " + labelEnd + " \t\t\t ; Se for quebra de linha Salta\n";
    assemblyCmd += "\tcmp RSI, RAX\n";
    assemblyCmd += "\tje " + labelEnd + "\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa endereco da String\n";
    assemblyCmd += "\tjmp " + labelLoop + "\n\n";

    assemblyCmd += labelEnd + ":\n";
    assemblyCmd += "\tmov DL, 0 \t\t\t ; Substitui quebra de linha por fim de string\n";
    assemblyCmd += "\tmov [RSI], DL \t\t\t ; Move fim de string para o identificador\n";
    assemblyCmd += "\t; -- END READ STR -- \n";
}

/**
 * @brief Generate code for read int command
 *
 * @param long addr (address of the int variable to receive the read value)
 */
void getCodeReadInt(long addr)
{
    // Generete a new temporary (12 bytes)
    long buffer = newTemp(12);

    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    // Init assembly code read (input)
    assemblyCmd += "\n\t; -- READ INT -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + " \t\t\t ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RDX, 100h \t\t\t ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RAX, 0 \t\t\t ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 \t\t\t ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";

    // Processing string reading to int number
    assemblyCmd += "\tmov EAX, 0 \t\t\t ; Acumulador\n";
    assemblyCmd += "\tmov EBX, 0 \t\t\t ; Caractere\n";
    assemblyCmd += "\tmov ECX, 10 \t\t\t ; Base 10\n";
    assemblyCmd += "\tmov DX, 1 \t\t\t ; Sinal\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + " \t\t\t ; End. buffer\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";
    assemblyCmd += "\tcmp BL, \'-\' \t\t\t ; Sinal - ?\n";
    assemblyCmd += "\tjne " + label0 + " \t\t\t ; Se dif -, Salta\n";
    assemblyCmd += "\tmov DX, -1 \t\t\t ; Senão, armazena -\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Inc. ponteiro string\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";

    assemblyCmd += "" + label0 + ":\n";
    assemblyCmd += "\tpush DX \t\t\t ; Empilha sinal\n";
    assemblyCmd += "\tmov EDX, 0 \t\t\t ; Reg. multiplicação\n";

    assemblyCmd += "" + label1 + ":\n";
    assemblyCmd += "\tcmp BL, 0Ah \t\t\t ; Verifica fim string\n";
    assemblyCmd += "\tje " + label2 + " \t\t\t ; Salta se fim string\n";
    assemblyCmd += "\timul ECX \t\t\t ; Mult. EAX por 10\n";
    assemblyCmd += "\tsub BL, \'0\' \t\t\t ; Converte caractere\n";
    assemblyCmd += "\tadd EAX, EBX \t\t\t ; Soma valor caractere\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa base\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";
    assemblyCmd += "\tjmp " + label1 + " \t\t\t; Loop\n";

    assemblyCmd += label2 + ":\n";
    assemblyCmd += "\tpop CX \t\t\t ; Desempilha sinal\n";
    assemblyCmd += "\tcmp CX, 0\n";
    assemblyCmd += "\tjg " + labelEnd + "\n";
    assemblyCmd += "\tneg EAX \t\t\t ; Mult. sinal\n";

    assemblyCmd += labelEnd + ":\n";
    assemblyCmd += "\tmov [ M + " + to_string(addr) + " ], EAX \t\t\t; Carrega o valor para o indentificador\n";
    assemblyCmd += "\t; -- END READ INT -- \n";
}

/**
 * @brief Generate code for read float command
 *
 * @param long addr (address of the float variable to receive the read value)
 */
void getCodeReadFloat(long addr)
{
    // Generete a new temporary (12 bytes)
    long buffer = newTemp(12);

    // Declaration of labels
    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();
    string label3 = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    // Init assembly code read (input)
    assemblyCmd += "\n\t; -- READ FLOAT -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + " \t\t\t ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RDX, 100h \t\t\t ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RAX, 0 \t\t\t ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 \t\t\t ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";

    // processing string reading to float number
    assemblyCmd += "\tmov RAX, 0 \t\t\t ; Acumul. parte int. em RAX\n";
    assemblyCmd += "\tsubss XMM0, XMM0 \t\t\t ; Acumul. parte frac.\n";
    assemblyCmd += "\tmov RBX, 0 \t\t\t ; Caractere\n";
    assemblyCmd += "\tmov RCX, 10 \t\t\t ; Base 10\n";
    assemblyCmd += "\tcvtsi2ss XMM3, RCX \t\t\t ; Base 10\n";
    assemblyCmd += "\tmovss XMM2, XMM3 \t\t\t ; Potência de 10\n";
    assemblyCmd += "\tmov RDX, 1 \t\t\t ; Sinal\n";
    assemblyCmd += "\tmov RSI, M+" + to_string(buffer) + "\t\t\t ; End. buffer\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";
    assemblyCmd += "\tcmp BL, '-' \t\t\t ; Sinal - ?\n";
    assemblyCmd += "\tjne " + label0 + "\t\t\t ; Se dif -, Salta\n";
    assemblyCmd += "\tmov RDX, -1 \t\t\t ; Senão, armazena -\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Inc. ponteiro string\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";

    assemblyCmd += label0 + ":\n";
    assemblyCmd += "\tpush RDX \t\t\t ; Empilha sinal\n";
    assemblyCmd += "\tmov RDX, 0 \t\t\t ; Reg. multiplicação\n";

    assemblyCmd += label1 + ":\n";
    assemblyCmd += "\tcmp BL, 0Ah \t\t\t ; Verifica fim string\n";
    assemblyCmd += "\tje " + label2 + " \t\t\t ; Salta se fim string\n";
    assemblyCmd += "\tcmp BL, \'.\' \t\t\t ; Senão verifica ponto\n";
    assemblyCmd += "\tje " + label3 + "\t\t\t ; Salta se ponto\n";
    assemblyCmd += "\timul ECX \t\t\t ; Mult. EAX por 10\n";
    assemblyCmd += "\tsub BL, \'0\' \t\t\t ; Converte caractere\n";
    assemblyCmd += "\tadd EAX, EBX \t\t\t ; Soma valor caractere\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Incrementa base\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";
    assemblyCmd += "\tjmp " + label1 + "\t\t\t ; Loop\n";

    assemblyCmd += label3 + ":\n";
    assemblyCmd += "\t; Calcula parte fracionária em XMM0\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t ; Inc. ponteiro string\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t ; Carrega caractere\n";
    assemblyCmd += "\tcmp BL, 0Ah \t\t\t ; Verifica fim string\n";
    assemblyCmd += "\tje " + label2 + "\t\t\t ; Salta se fim string\n";
    assemblyCmd += "\tsub BL, \'0\' \t\t\t ; Converte caractere\n";
    assemblyCmd += "\tcvtsi2ss XMM1, RBX \t\t\t ; Conv real\n";
    assemblyCmd += "\tdivss XMM1, XMM2 \t\t\t ; Transf. casa decimal\n";
    assemblyCmd += "\taddss XMM0, XMM1 \t\t\t ; Soma acumul.\n";
    assemblyCmd += "\tmulss XMM2, XMM3 \t\t\t ; Atualiza potência\n";
    assemblyCmd += "\tjmp " + label3 + "\t\t\t ; Loop\n";
    assemblyCmd += label2 + ":\n";

    assemblyCmd += "\tcvtsi2ss XMM1, RAX \t\t\t ; Conv parte inteira\n";
    assemblyCmd += "\taddss XMM0, XMM1 \t\t\t ; Soma parte frac.\n";
    assemblyCmd += "\tpop RCX \t\t\t ; Desempilha sinal\n";
    assemblyCmd += "\tcvtsi2ss XMM1, RCX \t\t\t ; Conv sinal\n";
    assemblyCmd += "\tmulss XMM0, XMM1 \t\t\t ; Mult. sinal\n";

    assemblyCmd += labelEnd + ":\n";
    assemblyCmd += "\tmovss [ M + " + to_string(addr) + " ], XMM0 \t\t\t; Carrega o valor para o indentificador\n";
    assemblyCmd += "\t; -- END READ FLOAT -- \n";
}

/**
 * @brief Generate code for read char command
 *
 * @param long addr (address of the char variable to receive the read value)
 */
void getCodeReadChar(long addr)
{
    // Generete a new temporary (1 byte)
    long buffer = newTemp(1);

    string labelEnd = getNextAssemblyLabel();
    string cleanBufferLabel = getNextAssemblyLabel();

    // Init assembly code read (input)
    assemblyCmd += "\n\t; -- READ CHAR -- \n";
    assemblyCmd += "\tmov RSI, M + " + to_string(addr) + " \t\t\t ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RDX, 1 \t\t\t ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RAX, 0 \t\t\t ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 \t\t\t ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";

    assemblyCmd += "; Limpa o buffer para a proxima leitura\n";
    assemblyCmd += cleanBufferLabel + ":\n";
    assemblyCmd += "\tmov RDX, 1 \t\t\t ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + " \t\t\t ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RAX, 0 \t\t\t ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 \t\t\t ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";
    assemblyCmd += "\tmov AL,[ M + " + to_string(buffer) + " ]\n";
    assemblyCmd += "\tcmp AL, 0xA \t\t\t ; Verifica se e nova linha\n";
    assemblyCmd += "\tjne " + cleanBufferLabel + "\t\t\t ; Le o proximo se nao for nova linha\n\n";
    assemblyCmd += "\t; -- END READ CHAR -- \n";
}

/**
 * @brief Generate code for read command
 *
 * @param long addr (address of the variable to receive the read value), int type (type to be read)
 */
void getCodeRead(long addr, int type)
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
}

// **** IF SECTION ****

/**
 * @brief Generate code for begin of a if block
 *
 * @param long addr (address of the boolean expression used in if condition)
 * @return string end of if label
 */
string getCodeOpenIf(long addr)
{

    // Label false condition
    string labelFalse = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- IF -- \n";
    assemblyCmd += "\tmov EAX, [ M  + " + to_string(addr) + " ] \t\t\t ; Recupera valor de booleano da memoria\n";
    assemblyCmd += "\tcmp EAX, 1 \t\t\t ; Comparacao com booleano verdadeiro\n";
    assemblyCmd += "\tjne " + labelFalse + "\t\t\t ; Se valor nao for verdadeiro pular bloco do if\n";

    return labelFalse;
}

/**
 * @brief Generate code for end of a if block
 *
 * @param string end of if label
 */
void getCodeCloseBlockIf(string label)
{
    assemblyCmd += label + ": \t\t\t ; fecha bloco do if\n";
    assemblyCmd += "\t; -- END IF -- \n";
}

/**
 * @brief Generate code for begin of a else block
 *
 * @param long addr (address of the boolean expression used in if condition)
 * @return string end of else label
 */
string getCodeOpenElse(long addr)
{
    string labelFalse = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- ELSE -- \n";
    assemblyCmd += "\tmov EAX, [ M   + " + to_string(addr) + " ] \t\t\t ; Recupera valor de booleano da memoria\n";
    assemblyCmd += "\tcmp EAX, 0 \t\t\t ; Comparacao com booleano false\n";
    assemblyCmd += "\tjne " + labelFalse + "\t\t\t ; Se valor nao for falso pular bloco do else\n";
    assemblyCmd += "\t; -- END ELSE -- \n";

    return labelFalse;
}

// **** WHILE SECTION ****

/**
 * @brief Generate code for begin of a while block
 *
 * @param addr address of a boolean expression
 * @param labelFalse Label end of while
 * @param labelLoop Label begin of while
 */
void getCodeOpenWhile(long addr, string &labelFalse, string &labelLoop)
{

    // Label end of while
    labelFalse = getNextAssemblyLabel();
    // Label begin of while
    labelLoop = getNextAssemblyLabel();

    assemblyCmd += "\n\t; -- WHILE -- \n";
    assemblyCmd += labelLoop + ":  \t\t\t ; inicio while \n";
    assemblyCmd += "\tmov EAX, [ M  + " + to_string(addr) + " ] \t\t\t ; Recupera valor de booleano da memoria\n";
    assemblyCmd += "\tcmp EAX, 1 \t\t\t ; Comparacao com booleano verdadeiro\n";
    assemblyCmd += "\tjne " + labelFalse + "\t\t\t ; Se valor nao for verdadeiro pular bloco do while\n";
}

/**
 * @brief Generate code for end of while block
 *
 * @param string labelFalse (Label end of while) and string labelLoop (Label begin of while)
 */
void getCodeCloseBlockWhile(string labelFalse, string labelLoop)
{
    assemblyCmd += "\tjmp " + labelLoop + "\t\t\t ; Realiza o loop do while\n";
    assemblyCmd += labelFalse + ": \t\t\t ; Fim do while \n";
    assemblyCmd += "\t; -- END WHILE -- \n";
}