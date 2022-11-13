section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resd 1			; Declaracao Var Inteiro em [65536]
	resd 1			; Declaracao Var Inteiro em [65540]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov EAX, 13			; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov EAX, [ M + 65540 ] 			; Move o valor de int 1 da memoria para o registrador EAX
	mov EBX, [ M + 0 ] 			; Move o valor de int 2 da memoria para o registrador EBX
	imul EBX 			; int1 * int2
	mov [ M + 4 ], EAX 			; Salva resultado em temporario
	mov EAX, 1			; Move Inteiro imediato para registrador
	mov [ M + 8 ], EAX 			; Move registrador para posicao atual de memoria em [8]
	mov EAX, [ M + 4 ] 			; Move o valor de int 1 da memoria para o registrador EAX
	mov EBX, [ M + 8 ] 			; Move o valor de int 2 da memoria para o registrador EBX
	idiv EBX 			; int1 div int2
	mov [ M + 12 ], EAX 			; Salva resultado em temporario
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
