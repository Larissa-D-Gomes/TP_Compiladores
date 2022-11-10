section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resd 1        ; Declaracao Inteiro em [65536]
	resd 1        ; Declaracao Float em [65540]
	resb 1        ; Declaracao Character em [65544]
	resb 100h        ; Declaracao String em [65545]
	resb 1        ; Declaracao Boolean em [65801]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
