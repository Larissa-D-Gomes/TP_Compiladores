section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
; *** Definicoes de variaveis e constantes ***
	resd 1			; Declaracao Var Inteiro em [65536]
	resd 1			; Declaracao Var Inteiro em [65540]
	resd 1			; Declaracao Var Inteiro em [65544]
	resd 1			; Declaracao Var Inteiro em [65548]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
; *** Comandos *** 

	; -- CONST INT -- 
	mov EAX, 10 			 ; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			 ; Move registrador para memoria temporaria
	; -- END CONST INT -- 

	; -- WRITE INT --
	mov EAX, 0 ; Zera registrador
	mov RSI, 0 ; Zera registrador
	mov EAX, [ M + 0 ] 			; Inteiro a ser ;convertido
	mov RSI, M + 4 			 ; End. string ou temp.
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 0 			 ; Tam. string convertido
	cmp EAX, 0 			 ; Verifica sinal
	jge L1 			 ; Salta se número positivo
	mov BL, '-' 			 ; Senão, escreve sinal –
	mov [RSI], BL
	add RSI, 1 			 ; Incrementa índice
	add RDI, 1 			 ; Incrementa tamanho
	neg EAX 			 ; Toma módulo do número
	L1:
	mov EBX, 10 			 ; Divisor
	L2:
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push RDX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L2 			 ; Se não é 0, continua
	add RDI, RCX 			 ; Atualiza tam. string
	; Agora, desemp. os valores e escreve o string
	L3:
	pop RDX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L3			 ; Se não pilha vazia, loop
	mov RSI, M + 4 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L4: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L5 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L4  ; Se caractere lido != flag de fim de string continuar loop
L5: ; Fim do loop
	mov RSI, M + 4 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE INT --
	; -- LINE BREAK -- 
	mov RSI, M + 8
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
