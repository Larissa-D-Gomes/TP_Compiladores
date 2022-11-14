section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resb 1			; Declaracao Var Boolean em [65536]
	resb 1			; Declaracao Var Boolean em [65540]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov EAX, [ M + 65536 ] 			; Move o valor de boolean 1 da memoria para o registrador EAX
	mov EBX, [ M + 65540 ] 			; Move o valor de boolean 2 da memoria para o registrador EBX
	neg EAX 			; EAX = !EAX
	add EAX, 1 			; EAX = !EAX
	neg EBX 			; EBX = !EBX
	add EBX, 1 			; EBX = !EBX
	imul EBX 			; !EAX AND !EBX
	neg EAX 			; !(!EAX AND !EBX) = EAX OR EBX
	add EAX, 1 			; !(!EAX AND !EBX) = EAX OR EBX
	mov [ M + 0 ], EAX 			; Salva resultado em temporario
	mov EAX, [ M + 0 ] 			; Move o valor de boolean 1 da memoria para o registrador EAX
	mov EBX, [ M + 65536 ] 			; Move o valor de boolean 2 da memoria para o registrador EBX
	neg EAX 			; EAX = !EAX
	add EAX, 1 			; EAX = !EAX
	neg EBX 			; EBX = !EBX
	add EBX, 1 			; EBX = !EBX
	imul EBX 			; !EAX AND !EBX
	neg EAX 			; !(!EAX AND !EBX) = EAX OR EBX
	add EAX, 1 			; !(!EAX AND !EBX) = EAX OR EBX
	mov [ M + 4 ], EAX 			; Salva resultado em temporario
	mov EAX, 1			; Move Boolean imediado para registrador eax
	mov [ M + 8 ], EAX 			; Move registrador para posicao atual de memoria em [8]
	mov EAX, [ M + 4 ] 			; Move o valor de boolean 1 da memoria para o registrador EAX
	mov EBX, [ M + 8 ] 			; Move o valor de boolean 2 da memoria para o registrador EBX
	neg EAX 			; EAX = !EAX
	add EAX, 1 			; EAX = !EAX
	neg EBX 			; EBX = !EBX
	add EBX, 1 			; EBX = !EBX
	imul EBX 			; !EAX AND !EBX
	neg EAX 			; !(!EAX AND !EBX) = EAX OR EBX
	add EAX, 1 			; !(!EAX AND !EBX) = EAX OR EBX
	mov [ M + 12 ], EAX 			; Salva resultado em temporario
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
