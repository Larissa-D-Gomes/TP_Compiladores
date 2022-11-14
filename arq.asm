section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resb 1			; Declaracao Var Boolean em [65536]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov EAX, 1			; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov EAX, 2			; Move Inteiro imediato para registrador
	mov [ M + 4 ], EAX 			; Move registrador para posicao atual de memoria em [4]
	mov EAX, [ M + 0 ] 			; Move o valor de int 1 da memoria para o registrador EAX
	mov EBX, [ M + 4 ] 			; Move o valor de int 2 da memoria para o registrador EBX
	comiss EAX, EBX  			; compara int1 com int2
	jg L1			; salta para L1 se int1 > int2
	mov EAX, 0 ; Define registrador como falso
	jmp L2
L1:
	mov EAX, 1 ; Define registrador como true
L2:
	mov [ M + 8 ], EAX 			; Salva resultado em temporario
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
