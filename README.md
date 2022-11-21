# Implementação: Compilador

###### `Developed by:`

`@author` Larissa Domingues Gomes

`@author` Lucas Bottrel Lopes de Moura

`@author` Vinícius Silva Mendes

---

> ### Trabalho prático desenvolvido na disciplina de **Compiladores**

O objetivo do trabalho prático é o desenvolvimento de um compilador completo que traduza programas escritos na linguagem fonte “L” para um subconjunto do ASSEMBLY x64. O compilador produz um arquivo texto que pode ser convertido em linguagem de máquina pelo montador NASM.

Abaixo está descrito a documentação do trabalho, descrevendo o funcionamento do analisador léxico, a gramática do analisador sintático e o esquema de tradução do analisador semântico.

#### `1.` Analisador Léxico
Segue abaixo o autômato finito determinístico que representa o Analisador Léxico do compilador:

![image](https://user-images.githubusercontent.com/65674658/203069954-dc8857c0-ec06-41c1-831b-9a04f7752f31.png)

#### `2.` Analisador Sintático

Segue abaixo a gramática da Linguagem L utilizada no Analizador Sintático do compilador:

![image](https://user-images.githubusercontent.com/65674658/203070012-ac2a2180-48fc-46b3-928a-df9f372b35ff.png)

#### `3.` Tradução de Código

Segue abaixo a gramática com ações semânticas e de geração de código do compilador:

![image](https://user-images.githubusercontent.com/65674658/203070049-59a9a725-e420-4efa-9a1a-1fce74b375b3.png)

![image](https://user-images.githubusercontent.com/65674658/203070092-5a1d2c50-0262-4144-8792-074f0c6d26ba.png)

---
### Build Project
###### Run

Para *buildar* o projeto, basta realizar a seguinte chamada na pasta raíz do projeto:

`$ make`

Pode ser necessário criar um diretório `obj` na raiz do projeto para o _make_ gerar os arquivos .o.
###### Run and Test

Para *buildar* e *testar* o projeto, basta criar um arquivo de teste .txt na pasta raíz, por exemplo. Nesse arquivo, deverá conter todo escopo de teste que é permitido pela linguagem.

Posteriormente, é possível buildar e testar o projeto pela seguinte linha de comando:

`$ make && ./Lcompiler.exe < (createdFile).txt`

###### Run, Test and Code Generator

Por fim, para *buildar*, *testar* e *gerarCódigoAssembly* no projeto, é necessário realizar o passo anterior e executar a linha de comando com os seguintes incrementos:

`$ make && ./Lcompiler.exe < (createdFile).txt  && nasm arq.asm -g -w-zeroing -f elf64 -o arq.o && ld arq.o -o arq && ./arq`

Observação: Ao executar a geração de código, acarretará na execução da função Peephole que fará a otimização do código assembly removendo instruções desnecessárias. 
