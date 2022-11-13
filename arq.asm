section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resd 1			; Declaracao Var Float em [65536]
	resd 1			; Declaracao Var Inteiro em [65540]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	movss XMM0, [ M + 65536 ] 			; Move o valor de float da memoria para o registrador XMM0
	mov EAX, [ M + 65540 ] 			; Move o valor de int da memoria para o registrador EAX
	cvtsi2ss XMM1, EAX 			; Expande int para float
	divss XMM0, XMM1  			; float / float(int)
	movss [ M + 0 ], XMM0 			;  Salva resultado em temporario
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
