section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	dd 10			; Declaracao Const Inteiro em [65536]
	resd 1			; Declaracao Var Inteiro em [65540]
	dd 15.0			; Declaracao Const Float em [65544]
	resd 1			; Declaracao Var Float em [65548]
	db 'a', 1			; Declaracao Const Char em [65552]
	resb 1			; Declaracao Var Character em [65553]
	db "string",0			; Declaracao Const String em [65554]
	resb 100h			; Declaracao Var String em [65810]
	dd 1			; Declaracao Const Boolean em [66066]
	resb 1			; Declaracao Var Boolean em [66070]
	dd 15.0			; Declaracao Float EXP em [66074]
	dd  0.50			; Declaracao Float EXP em [66078]
	db "euqerobolean",0			; Declaracao String EXP em [66082]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov EAX, 3			; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov AL, 'a'			; Move Char imediato para registrador
	mov [ M + 0 ], AL 			; Move registrador para posicao atual de memoria em [0]
	mov EAX, 0			; Move Boolean imediado para registrador eax
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov RAX, [ M + 66070 ] 			; Move o valor da memoria para o registrador 
	neg RAX 			; Nega o valor que esta no registrador
	add RAX, 1 			; Nega o valor que esta no registrador
	mov [ M + 0 ], RAX 			; Move o valor do registrador para a posicao de memoria [0]
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
