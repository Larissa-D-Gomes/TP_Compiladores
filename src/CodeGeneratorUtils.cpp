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
#include "headers/SymbolTable.hpp"
#include "headers/LexicalRegister.hpp"
#include "headers/ConstType.hpp"

// Global next memory free position
long nextFreePosition = 0x10000;
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

/**
 * @brief Allocate memory for a variable tha contains 
 * memSize bytes
 * 
 * @param long memSize
 */
void memoryAlocation(long memSize){
    cout << nextFreePosition << " " << memSize << endl;
    nextFreePosition += memSize;
}

string getNextAssemblyLabel(){
    return "L" + to_string(++assemblyLabel);
}

string getCodeWriteStr(long addr, int type){
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

string getCodeWrite(long addr, int type){

    if(type == ConstType::STRING){
        return getCodeWriteStr(addr, type);
    }


    return ";=-=-=-=-=-= FIM WRITELN =-=-=-=-=-=\n";
}