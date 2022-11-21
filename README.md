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

![Imagem que mostra o autômato do Analisador Léxico do compilador](https://drive.google.com/file/d/15vmWZ1AbF8sg4jwpHHwqavoAgViSDA0k/view?usp=sharing)

#### `2.` Analisador Sintático

Segue abaixo a gramática da Linguagem L utilizada no Analizador Sintático do compilador:

![Imagem que mostra a gramática do Analisador Sintático do compilador](https://drive.google.com/file/d/1JakZ_YdHZbyRMjesZIEDHun2i6d-78dy/view?usp=sharing)

#### `3.` Tradução de Código

Segue abaixo a gramática com ações semânticas e de geração de código do compilador:

![Imagem que mostra a gramática do Analisador Semântico do compilador](https://drive.google.com/file/d/1-jup7iTSqr8z4FluxB6CiYIunB3LNpSH/view?usp=sharing)

![Imagem que mostra as ações semânticas e geração de código do compilador](https://drive.google.com/file/d/17Z5REKWg0IUhC7DDDK7gPk3L4NMSnDz8/view?usp=sharing)

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