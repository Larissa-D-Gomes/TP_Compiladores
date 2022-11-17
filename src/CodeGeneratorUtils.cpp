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

long addrStringTrue = null;
long addrStringFalse = null;
long addrStringLineBr = null;


/**
 * @brief Allocate memory for a variable that contains
 * memSize bytes
 *
 * @param long memSize
 */
void memoryAlocation(long memSize)
{
    // cout << nextFreePosition << " " << memSize << endl;
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
    long actualMemoryPosition = newTemp(ConstType::STRING);
    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();

    assemblyCmd += "\tmov eax, [ M + " + to_string(addr) + "] \t\t\t; inteiro a ser ;convertido\n";
    assemblyCmd += "\tmov rsi, M + " + to_string(actualMemoryPosition) + " \t\t\t ; end. string ou temp.\n";
    assemblyCmd += "\tmov rcx, 0 \t\t\t ; contador pilha\n";
    assemblyCmd += "\tmov rdi, 0 \t\t\t ; tam. string convertido\n";
    assemblyCmd += "\tcmp eax, 0 \t\t\t ; verifica sinal\n";
    assemblyCmd += "\tjge " + label0 + " \t\t\t ; salta se número positivo\n";
    assemblyCmd += "\tmov bl, \'-\' \t\t\t ; senão, escreve sinal –\n";
    assemblyCmd += "\tmov [rsi], bl\n";
    assemblyCmd += "\tadd rsi, 1 \t\t\t ; incrementa índice\n";
    assemblyCmd += "\tadd rdi, 1 \t\t\t ; incrementa tamanho\n";
    assemblyCmd += "\tneg eax \t\t\t;toma módulo do número\n";
    assemblyCmd += "\t" + label0 + ":\n";
    assemblyCmd += "\tmov ebx, 10 \t\t\t;divisor\n";
    assemblyCmd += "\t" + label1 + ":\n";
    assemblyCmd += "\tadd rcx, 1 \t\t\t;incrementa contador\n";
    assemblyCmd += "\tcdq \t\t\t;estende edx:eax p/ div.\n";
    assemblyCmd += "\tidiv ebx \t\t\t;divide edx;eax por ebx\n";
    assemblyCmd += "\tpush dx \t\t\t;empilha valor do resto\n";
    assemblyCmd += "\tcmp eax, 0 \t\t\t;verifica se quoc. é 0\n";
    assemblyCmd += "\tjne " + label1 + " \t\t\t;se não é 0, continua\n";
    assemblyCmd += "\tadd rdi,rcx \t\t\t;atualiza tam. string\n";
    assemblyCmd += "\t;agora, desemp. os valores e escreve o string\n";
    assemblyCmd += "\t" + label2 + ":\n";
    assemblyCmd += "\tpop dx \t\t\t ;desempilha valor\n";
    assemblyCmd += "\tadd dl, \'0\' \t\t\t ;transforma em caractere\n";
    assemblyCmd += "\tmov [rsi], dl \t\t\t ;escreve caractere\n";
    assemblyCmd += "\tadd rsi, 1 \t\t\t ;incrementa base\n";
    assemblyCmd += "\tsub rcx, 1 \t\t\t ;decrementa contador\n";
    assemblyCmd += "\tcmp rcx, 0 \t\t\t ;verifica pilha vazia\n";
    assemblyCmd += "\tjne " + label2  + "\t\t\t ;se não pilha vazia, loop\n";

    getCodeWriteStr(actualMemoryPosition);
}

void getCodeWriteFloat(long addr)
{
    long actualMemoryPosition = newTemp(ConstType::STRING);

    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();
    string label3 = getNextAssemblyLabel();
    string label4 = getNextAssemblyLabel();

    assemblyCmd += "\tmovss xmm0, [ M + " + to_string(addr) + "] \t\t\t;real a ser;convertido\n";
    assemblyCmd += "\tmov rsi, M + " + to_string(actualMemoryPosition) + " \t\t\t;end. temporário\n";
    assemblyCmd += "\tmov rcx, 0 \t\t\t;contador pilha\n";
    assemblyCmd += "\tmov rdi, 6 \t\t\t;precisao 6 casas compart\n";
    assemblyCmd += "\tmov rbx, 10 \t\t\t;divisor\n";
    assemblyCmd += "\tcvtsi2ss xmm2, rbx \t\t\t;divisor real\n";
    assemblyCmd += "\tsubss xmm1, xmm1 \t\t\t;zera registrador\n";
    assemblyCmd += "\tcomiss xmm0, xmm1 \t\t\t;verifica sinal\n";
    assemblyCmd += "\tjae " + label0 + " \t\t\t;salta se número positivo\n";
    assemblyCmd += "\tmov dl, \'-\' \t\t\t;senão, escreve sinal –\n";
    assemblyCmd += "\tmov [rsi], dl\n";
    assemblyCmd += "\tmov rdx, -1 \t\t\t;Carrega -1 em RDX\n";
    assemblyCmd += "\tcvtsi2ss xmm1, rdx \t\t\t;Converte para real\n";
    assemblyCmd += "\tmulss xmm0, xmm1 \t\t\t;Toma módulo\n";
    assemblyCmd += "\tadd rsi, 1 \t\t\t;incrementa índice\n";
    assemblyCmd += label0  + ": \n";
    assemblyCmd += "\troundss xmm1, xmm0, 0b0011 \t\t\t;parte inteira xmm1\n";
    assemblyCmd += "\tsubss xmm0, xmm1 \t\t\t;parte frac xmm0\n";
    assemblyCmd += "\tcvtss2si rax, xmm1 \t\t\t;convertido para int\n";
    assemblyCmd += "\t;converte parte inteira que está em rax\n";
    assemblyCmd +=  label1  + ": \n";
    assemblyCmd += "\tadd rcx, 1 \t\t\t;incrementa contador\n";
    assemblyCmd += "\tcdq \t\t\t;estende edx:eax p/ div.\n";
    assemblyCmd += "\tidiv ebx \t\t\t;divide edx;eax por ebx\n";
    assemblyCmd += "\tpush dx \t\t\t;empilha valor do resto\n";
    assemblyCmd += "\tcmp eax, 0 \t\t\t;verifica se quoc. é 0\n";
    assemblyCmd += "\tjne " + label1 + " \t\t\t;se não é 0, continuasub rdi, rcx \t\t\t;decrementa precisao\n";
    assemblyCmd += "\t;agora, desemp valores e escreve parte int\n";
    assemblyCmd += label2 + ":\n";
    assemblyCmd += "\tpop dx \t\t\t;desempilha valor\n";
    assemblyCmd += "\tadd dl, \'0\' \t\t\t;transforma em caractere\n";
    assemblyCmd += "\tmov [rsi], dl \t\t\t;escreve caractere\n";
    assemblyCmd += "\tadd rsi, 1 \t\t\t;incrementa base\n";
    assemblyCmd += "\tsub rcx, 1 \t\t\t;decrementa contador\n";
    assemblyCmd += "\tcmp rcx, 0 \t\t\t;verifica pilha vazia\n";
    assemblyCmd += "\tjne " + label2 + " \t\t\t;se não pilha vazia, loop\n";
    assemblyCmd += "\tmov dl, \'.\' \t\t\t;escreve ponto decimal\n";
    assemblyCmd += "\tmov [rsi], dl\n";
    assemblyCmd += "\tadd rsi, 1 \t\t\t;incrementa base\n";
    assemblyCmd += "\t\t\t;converte parte fracionaria que está em xmm0\n";
    assemblyCmd += label3 + ":\n";
    assemblyCmd += "\tcmp rdi, 0 \t\t\t;verifica precisao\n";
    assemblyCmd += "\tjle " + label4 + " \t\t\t;terminou precisao ?\n";
    assemblyCmd += "\tmulss xmm0,xmm2 \t\t\t;desloca para esquerda\n";
    assemblyCmd += "\troundss xmm1,xmm0,0b0011 \t\t\t;parte inteira xmm1\n";
    assemblyCmd += "\tsubss xmm0,xmm1 \t\t\t;atualiza xmm0\n";
    assemblyCmd += "\tcvtss2si rdx, xmm1\t\t\t ;convertido para int\n";
    assemblyCmd += "\tadd dl, \'0\' \t\t\t;transforma em caractere\n";
    assemblyCmd += "\tmov [rsi], dl \t\t\t;escreve caractere\n";
    assemblyCmd += "\tadd rsi, 1 \t\t\t;incrementa base\n";
    assemblyCmd += "\tsub rdi, 1 \t\t\t;decrementa precisao\n";
    assemblyCmd += "\tjmp " + label3 + "\n";
    assemblyCmd += "\t\t\t; impressão\n";
    assemblyCmd += label4 + ":\n";
    assemblyCmd += "\tmov dl, 0 \t\t\t;fim string, opcional\n";
    assemblyCmd += "\tmov [rsi], dl \t\t\t;escreve caractere\n";
    assemblyCmd += "\tmov rdx, rsi ;calc tam str convertido\n";
    assemblyCmd += "\tmov rbx, M+" + to_string(actualMemoryPosition) + "\n";
    assemblyCmd += "\tsub rdx, rbx \t\t\t;tam=rsi-M-buffer.end\n";
    assemblyCmd += "\tmov rsi, M+" + to_string(actualMemoryPosition) + " \t\t\t; endereço do buffer\n";

    assemblyCmd += "\tmov RAX, 1 ; Chamada para saída\n";
    assemblyCmd += "\tmov RDI, 1 ; Chamada para tela\n";
    assemblyCmd += "\tsyscall\n";
}

void getCodeWriteBoolean(long addr)
{
    string labelTrue = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();
    
    if(addrStringTrue == null)
    {
        addrStringTrue = nextFreePosition;
        assemblyDec += "\tdb \"true\" ,0\t\t\t; Declaracao String EXP em [" + to_string(addrStringTrue) + "]\n";
        // Memory Allocation in data area
        memoryAlocation(5);

        addrStringFalse = nextFreePosition;
        assemblyDec += "\tdb \"false\" ,0\t\t\t; Declaracao String EXP em [" + to_string(addrStringFalse) + "]\n";
        // Memory Allocation in data area
        memoryAlocation(6);
    }

    assemblyCmd += "\tmov EAX , [ M + " + to_string(addr) + " ]\t\t\t; Carrega valor de boolean\n";
    assemblyCmd += "\tcmp EAX , 0 \t\t\t; Se boolean for falso\n";
    assemblyCmd += "\tjne " + labelTrue + "\t\t\t; Se boolean for verdadeiro -> label true\n";
    // Print false
    getCodeWriteStr(addrStringFalse);
    assemblyCmd += "\tjmp " + labelEnd + "\t\t\t; Jump end\n";

    assemblyCmd += labelTrue + ": \n";
    getCodeWriteStr(addrStringTrue);

    assemblyCmd += labelEnd + ": \n";
}

void getCodeWriteLineBr()
{
    long buffer = newTemp(ConstType::CHAR);

    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + "\n";
    assemblyCmd += "\tmov [RSI], byte 10 ; Move caracter quebra de linha para o endereço armazenado em RSI\n";
    assemblyCmd += "\tmov RDX,1 \t\t\t; ou buffer.tam\n";
    assemblyCmd += "\tmov RAX, 1 \t\t\t; chamada para saida\n";
    assemblyCmd += "\tmov RDI, 1 \t\t\t; saida para tela\n";
    assemblyCmd += "\tsyscall\n";

}

void getCodeWrite(long addr, int type)
{
    if (type == ConstType::STRING)
    {
        cout << to_string(addr) << endl;
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


void getCodeReadFloat(long addr)
{
    
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

    if(type != ConstType::STRING){
        memoryAlocation(getTypeMemSize(type));
    } else {
        memoryAlocation(stringValue.length() - 1);
    }
    

    return actualMemoryPosition;
}

long getCodeExpConst(string stringValue, int type)
{
    long actualMemoryPosition;

    if (type == ConstType::STRING)
    {
        actualMemoryPosition = nextFreePosition;
        cout << "Alocacao pos " << actualMemoryPosition << endl;
        assemblyDec += "\tdb " + stringValue + ",0\t\t\t; Declaracao String EXP em [" + to_string(actualMemoryPosition) + "]\n";
        // Memory Allocation in data area
        memoryAlocation(stringValue.length() - 1);
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
    assemblyCmd += "\tmov EBX, M + " + to_string(addrAtrib) + " \t\t\t ; Move endereco de memoria da string para registrador RSI\n";
    assemblyCmd += "\tmov ECX, M + " + to_string(addrID) + " \t\t\t ; Move endereco de memoria da string para registrador RSI\n";
    assemblyCmd += "\tadd EAX, ECX \t\t\t ; Adiciona indice ao endereco inicial da memoria\n";
    assemblyCmd += "\tmov BL, [EBX] \t\t\t ; Escreve valor de caracter para o registrador AL\n";

    assemblyCmd += "\tmov [ EAX ], BL \t\t\t; Salva character no endereco do identificador\n";
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

// **** READ SECTION ****

void getCodeReadStr(long addr)
{
    string labelLoop = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();

    assemblyCmd += "\tmov RSI, M+" + to_string(addr) + " ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RDX, 100h ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RAX, 0 ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";
    assemblyCmd += "; Leitura string\n";
    assemblyCmd += "\tadd RAX, RSI\n";
    assemblyCmd += "\tsub RAX, 1\n";
    assemblyCmd += labelLoop + ":\n";
    assemblyCmd += "\tmov DL, [RSI] ; Move caractere para DL\n";
    assemblyCmd += "\tcmp DL, byte 10 ; Compara com quebra de linha\n";
    assemblyCmd += "\tje " + labelEnd + " ; Se for quebra de linha salta\n";
    assemblyCmd += "\tcmp RSI, RAX\n";
    assemblyCmd += "\tje " + labelEnd + "\n";
    assemblyCmd += "\tadd RSI, 1 ; Incrementa endereco da String\n";
    assemblyCmd += "\tjmp " + labelLoop + "\n\n";
    assemblyCmd += labelEnd + ":\n";
    assemblyCmd += "\tmov DL, 0 ; Substitui quebra de linha por fim de string\n";
    assemblyCmd += "\tmov [RSI], DL ; Move fim de string para o identificador\n";
}

void getCodeReadInt(long addr)
{ 
    long buffer = newTemp(12);

    string label0 = getNextAssemblyLabel();
    string label1 = getNextAssemblyLabel();
    string label2 = getNextAssemblyLabel();
    string labelEnd = getNextAssemblyLabel();


    assemblyCmd += "\tmov RSI, M+" + to_string(buffer) + " ; Salva o endereco do buffer\n";
    assemblyCmd += "\tmov RDX, 100h ; Tamanho do buffer\n";
    assemblyCmd += "\tmov RAX, 0 ; Chamada para leitura\n";
    assemblyCmd += "\tmov RDI, 0 ; Leitura do teclado\n";
    assemblyCmd += "\tsyscall\n\n";
    assemblyCmd += "\tmov EAX, 0 \t\t\t;acumulador\n";
    assemblyCmd += "\tmov EBX, 0 \t\t\t;caractere\n";
    assemblyCmd += "\tmov ECX, 10 \t\t\t;base 10\n";
    assemblyCmd += "\tmov DX, 1 \t\t\t;sinal\n";
    assemblyCmd += "\tmov RSI, M + " + to_string(buffer) + " \t\t\t;end. buffer\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t;carrega caractere\n";
    assemblyCmd += "\tcmp BL, \'-\' \t\t\t;sinal - ?\n";
    assemblyCmd += "\tjne " + label0 + " \t\t\t;se dif -, salta\n";
    assemblyCmd += "\tmov DX, -1 \t\t\t;senão, armazena -\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t;inc. ponteiro string\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t;carrega caractere\n";
    assemblyCmd += "" + label0 + ":\n";
    assemblyCmd += "\tpush DX \t\t\t;empilha sinal\n";
    assemblyCmd += "\tmov EDX, 0 \t\t\t;reg. multiplicação\n";
    assemblyCmd += "" + label1 + ":\n";
    assemblyCmd += "\tcmp BL, 0Ah \t\t\t;verifica fim string\n";
    assemblyCmd += "\tje " + label2 + " \t\t\t;salta se fim string\n";
    assemblyCmd += "\timul ECX \t\t\t;mult. EAX por 10\n";
    assemblyCmd += "\tsub BL, \'0\' \t\t\t;converte caractere\n";
    assemblyCmd += "\tadd EAX, EBX \t\t\t;soma valor caractere\n";
    assemblyCmd += "\tadd RSI, 1 \t\t\t;incrementa base\n";
    assemblyCmd += "\tmov BL, [RSI] \t\t\t;carrega caractere\n";
    assemblyCmd += "\tjmp " + label1 + " \t\t\t;loop\n";
    assemblyCmd += label2 + ":\n";
    assemblyCmd += "\tpop CX \t\t\t;desempilha sinal\n";
    assemblyCmd += "\tcmp CX, 0\n";
    assemblyCmd += "\tjg " + labelEnd + "\n";
    assemblyCmd += "\tneg EAX \t\t\t;mult. sinal\n";
    assemblyCmd += labelEnd + ":\n";
    assemblyCmd += "\tmov [M+" + to_string(addr) + "], EAX ; Carrega o valor para o indentificador\n";
}

void getCodeRead(long addr, int type)
{
    if (type == ConstType::STRING)
    {
        getCodeReadStr(addr);
    }
    // else if (type == ConstType::CHAR)
    // {
    //     getCodeReadChar(addr);
    // }
    else if (type == ConstType::INT)
    {
        getCodeReadInt(addr);
    }
    // else if (type == ConstType::FLOAT)
    // {
    //     getCodeReadFloat(addr);
    // }
}