section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resb 1			; Declaracao Var Boolean em [65536]
	db "str",0			; Declaracao String EXP em [65540]
	db "string1",0			; Declaracao String EXP em [65796]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov RSI, M + 65540 ; Movendo string1 da memória para registrador
	mov RAX, RSI ; Copiando endereço da string1 para um registrador de índice
	mov RDI, M + 65796 ; Movendo string2 da memória para registrador
	mov RBX, RDI ; Copiando endereço da string2 para um registrador de índice
L1: ; Inicio do loop
	mov AL, [RAX] ; Leitura de caracter na posicao RAX da memória
	mov BL, [RBX] ; Leitura de caracter na posicao RBX da memória
	cmp AL, 0 ; Verificação de flag de fim de string
	je L2 
 ; Se caracter lido = flag de fim de string finalizar loop
	cmp AL, BL ; Verificação de igualdade de caracter
	jne L3 
 ; Se string1[i] != string2[i]
	add RAX, 1 ; Incrementando numero de caracteres
	add RBX, 1 ; Incrementando numero de caracteres
	jmp L1  ; Se caracter lido != flag de fim de string continuar loop
L2: ; Fim do loop
	cmp BL, 0 ; Verificação de flag de fim de string
	je L4 
 ; Se caracter lido = flag de fim de string finalizar loop

L3:
	mov EAX, 0 ; Define registrador como falso
	jmp L5
L4:
	mov EAX, 1 ; Define registrador como true
L5:
	mov [ M + 0 ], EAX 			; Salva resultado em temporario
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
