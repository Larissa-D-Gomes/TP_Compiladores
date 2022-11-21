section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
; *** Definicoes de variaveis e constantes ***
	resd 1			; Declaracao Var Float em [65536]
	dd -10.0 			 ; Declaracao Float M em [ 65540 ]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
; *** Comandos *** 
	movss XMM0, [ M + 65540 ] 			 ; Recupera valor do identificador da memoria
	movss [ M + 65536 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
	movss XMM0, [ M + 65536 ] 			 ; Real a ser convertido
	mov RSI, M + 0 			 ; End. temporário
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 6 			 ; Precisão 6 casas compart
	mov RBX, 10 			 ; Divisor
	cvtsi2ss XMM2, RBX 			 ; Divisor real
	subss XMM1, XMM1 			 ; Zera registrador
	comiss XMM0, XMM1 			 ; Verifica sinal
	jae L1 			 ; Salta se número positivo
	mov DL, '-' 			; Senão, escreve sinal –
	mov [RSI], DL
	mov RDX, -1 			 ; Carrega -1 em RDX
	cvtsi2ss XMM1, RDX 			 ; Converte para real
	mulss XMM0, XMM1 			 ; Toma módulo
	add RSI, 1 			 ; Incrementa índice
L1: 
	roundss XMM1, XMM0, 0b0011 			 ; Parte inteira XMM1
	subss XMM0, XMM1 			 ; Parte frac XMM0
	cvtss2si rax, XMM1 			 ; Convertido para int
	; Converte parte inteira que está em rax
L2: 
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push RDX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L2 			 ; Se não é 0, continua
	sub RDI, RCX 			;decrementa precisao
	; Agora, desemp valores e escreve parte int
L3:
	pop RDX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L3 			 ; Se não pilha vazia, loop
	mov DL, '.' 			 ; Escreve ponto decimal
	mov [RSI], DL
	add RSI, 1 			 ; Incrementa base
	; Converte parte fracionaria que está em XMM0
L4:
	cmp RDI, 0 			 ; Verifica precisao
	jle L5 			 ; Terminou precisao ?
	mulss XMM0,XMM2 			 ; Desloca para esquerda
	roundss XMM1,XMM0,0b0011 			 ; Parte inteira XMM1
	subss XMM0,XMM1 			 ; Atualiza XMM0
	cvtss2si RDX, XMM1			 ; Convertido para int
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RDI, 1 			 ; Decrementa precisão
	jmp L4
	; Impressão
L5:
	mov DL, 0 			 ; Fim string, opcional
	mov [RSI], DL 			 ; Escreve caractere
	mov RDX, RSI ; Calc tam str convertido
	mov RBX, M + 0 
	sub RDX, RBX 			 ; Tam=RSI-M-buffer.end
	mov RSI, M + 0 			; Endereço do buffer
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	mov RSI, M + 4
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
