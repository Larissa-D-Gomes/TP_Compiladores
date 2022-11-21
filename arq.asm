section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
; *** Definicoes de variaveis e constantes ***
	resd 1			; Declaracao Var Float em [65536]
	resd 1			; Declaracao Var Float em [65540]
	resd 1			; Declaracao Var Float em [65544]
	resd 1			; Declaracao Var Float em [65548]
	resb 1			; Declaracao Var Character em [65552]
	db "DIGITE O PRIMEIRO NUMERO REAL: ", 0 			 ; Declaracao String M em [ 65553 ]
	db "Digite a operacao (+,-,*,/): ", 0 			 ; Declaracao String M em [ 65585 ]
	db "Digite o primeiro numero real: ", 0 			 ; Declaracao String M em [ 65615 ]
	db "Operacao por 0", 0 			 ; Declaracao String M em [ 65647 ]
	db "Resultado: ", 0 			 ; Declaracao String M em [ 65662 ]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
; *** Comandos *** 
L2:  			 ; inicio while 

	; -- CONST BOOLEAN -- 
	mov EAX, 1			 ; Move Boolean imediado para registrador EAX
	mov [ M + 0 ], EAX 			 ; Move valor do registrador para memoria temporaria

	; -- CMP WHILE -- 
	mov EAX, [ M  + 0 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L1			 ; Se valor nao for verdadeiro pular bloco do while

	; -- WRITE STRING -- 
	mov RSI, M + 65553 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L3: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L4 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L3  ; Se caractere lido != flag de fim de string continuar loop
L4: ; Fim do loop
	mov RSI, M + 65553 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	mov RSI, M + 0 			 ; Salva o endereco do buffer
	mov RDX, 100h 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

	mov RAX, 0 			 ; Acumul. parte int. em RAX
	subss XMM0, XMM0 			 ; Acumul. parte frac.
	mov RBX, 0 			 ; Caractere
	mov RCX, 10 			 ; Base 10
	cvtsi2ss XMM3, RCX 			 ; Base 10
	movss XMM2, XMM3 			 ; Potência de 10
	mov RDX, 1 			 ; Sinal
	mov RSI, M+0			 ; End. buffer
	mov BL, [RSI] 			 ; Carrega caractere
	cmp BL, '-' 			 ; Sinal - ?
	jne L5			 ; Se dif -, Salta
	mov RDX, -1 			 ; Senão, armazena -
	add RSI, 1 			 ; Inc. ponteiro string
	mov BL, [RSI] 			 ; Carrega caractere
L5:
	push RDX 			 ; Empilha sinal
	mov RDX, 0 			 ; Reg. multiplicação
L6:
	cmp BL, 0Ah 			 ; Verifica fim string
	je L7 			 ; Salta se fim string
	cmp BL, '.' 			 ; Senão verifica ponto
	je L8			 ; Salta se ponto
	imul ECX 			 ; Mult. EAX por 10
	sub BL, '0' 			 ; Converte caractere
	add EAX, EBX 			 ; Soma valor caractere
	add RSI, 1 			 ; Incrementa base
	mov BL, [RSI] 			 ; Carrega caractere
	jmp L6			 ; Loop
L8:
	; Calcula parte fracionária em XMM0
	add RSI, 1 			 ; Inc. ponteiro string
	mov BL, [RSI] 			 ; Carrega caractere
	cmp BL, 0Ah 			 ; Verifica fim string
	je L7			 ; Salta se fim string
	sub BL, '0' 			 ; Converte caractere
	cvtsi2ss XMM1, RBX 			 ; Conv real
	divss XMM1, XMM2 			 ; Transf. casa decimal
	addss XMM0, XMM1 			 ; Soma acumul.
	mulss XMM2, XMM3 			 ; Atualiza potência
	jmp L8			 ; Loop
L7:
	cvtsi2ss XMM1, RAX 			 ; Conv parte inteira
	addss XMM0, XMM1 			 ; Soma parte frac.
	pop RCX 			 ; Desempilha sinal
	cvtsi2ss XMM1, RCX 			 ; Conv sinal
	mulss XMM0, XMM1 			 ; Mult. sinal
L9:
	movss [ M + 65540 ], XMM0 			; Carrega o valor para o indentificador

	; -- WRITE STRING -- 
	mov RSI, M + 65585 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L10: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L11 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L10  ; Se caractere lido != flag de fim de string continuar loop
L11: ; Fim do loop
	mov RSI, M + 65585 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	mov RSI, M + 65552 			 ; Salva o endereco do buffer
	mov RDX, 1 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

; Limpa o buffer para a proxima leitura
L13:
	mov RDX, 1 			 ; Tamanho do buffer
	mov RSI, M + 0 			 ; Salva o endereco do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

	mov AL,[ M + 0 ]
	cmp AL, 0xA 			 ; Verifica se e nova linha
	jne L13			 ; Le o proximo se nao for nova linha


	; -- WRITE STRING -- 
	mov RSI, M + 65615 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L14: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L15 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L14  ; Se caractere lido != flag de fim de string continuar loop
L15: ; Fim do loop
	mov RSI, M + 65615 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	mov RSI, M + 0 			 ; Salva o endereco do buffer
	mov RDX, 100h 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

	mov RAX, 0 			 ; Acumul. parte int. em RAX
	subss XMM0, XMM0 			 ; Acumul. parte frac.
	mov RBX, 0 			 ; Caractere
	mov RCX, 10 			 ; Base 10
	cvtsi2ss XMM3, RCX 			 ; Base 10
	movss XMM2, XMM3 			 ; Potência de 10
	mov RDX, 1 			 ; Sinal
	mov RSI, M+0			 ; End. buffer
	mov BL, [RSI] 			 ; Carrega caractere
	cmp BL, '-' 			 ; Sinal - ?
	jne L16			 ; Se dif -, Salta
	mov RDX, -1 			 ; Senão, armazena -
	add RSI, 1 			 ; Inc. ponteiro string
	mov BL, [RSI] 			 ; Carrega caractere
L16:
	push RDX 			 ; Empilha sinal
	mov RDX, 0 			 ; Reg. multiplicação
L17:
	cmp BL, 0Ah 			 ; Verifica fim string
	je L18 			 ; Salta se fim string
	cmp BL, '.' 			 ; Senão verifica ponto
	je L19			 ; Salta se ponto
	imul ECX 			 ; Mult. EAX por 10
	sub BL, '0' 			 ; Converte caractere
	add EAX, EBX 			 ; Soma valor caractere
	add RSI, 1 			 ; Incrementa base
	mov BL, [RSI] 			 ; Carrega caractere
	jmp L17			 ; Loop
L19:
	; Calcula parte fracionária em XMM0
	add RSI, 1 			 ; Inc. ponteiro string
	mov BL, [RSI] 			 ; Carrega caractere
	cmp BL, 0Ah 			 ; Verifica fim string
	je L18			 ; Salta se fim string
	sub BL, '0' 			 ; Converte caractere
	cvtsi2ss XMM1, RBX 			 ; Conv real
	divss XMM1, XMM2 			 ; Transf. casa decimal
	addss XMM0, XMM1 			 ; Soma acumul.
	mulss XMM2, XMM3 			 ; Atualiza potência
	jmp L19			 ; Loop
L18:
	cvtsi2ss XMM1, RAX 			 ; Conv parte inteira
	addss XMM0, XMM1 			 ; Soma parte frac.
	pop RCX 			 ; Desempilha sinal
	cvtsi2ss XMM1, RCX 			 ; Conv sinal
	mulss XMM0, XMM1 			 ; Mult. sinal
L20:
	movss [ M + 65544 ], XMM0 			; Carrega o valor para o indentificador

	; -- CONST CHAR -- 
	mov AL, 'd' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L21			 ; Salta para L21 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L22
L21:
	mov EAX, 1 			 ; Define registrador como true
L22:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario

	; -- CONST CHAR -- 
	mov AL, 'm' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L23			 ; Salta para L23 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L24
L23:
	mov EAX, 1 			 ; Define registrador como true
L24:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario

	; -- CONST INT -- 
	mov EAX, 0 			 ; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			 ; Move registrador para memoria temporaria

	; -- FLOAT = INT -- 
	movss XMM0, [ M + 65544 ] 			 ; Move o valor de float da memoria para registrador 
	mov RAX, [ M + 0 ] 			 ; Move o valor de int da memoria para registrador
	cdqe 			 ; Expandindo o sinal de valor
	cvtsi2ss XMM1, RAX 			 ; Converte int para float
	comiss XMM0, XMM1 			 ; Compara float1 com float2
	je L25			 ; Salta para L25 se float1 = float2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L26
L25:
	mov EAX, 1 			 ; Define registrador como true
L26:
	mov [ M + 4 ], EAX 			 ;  Salva resultado em temporario
	; -- END FLOAT = INT -- 

	; -- INT * INT -- 
	mov EAX, [ M + 1 ] 			 ; Move o valor de int 1 da memoria para registrador 
	mov EBX, [ M + 4 ] 			 ; Move o valor de int 2 da memoria para registrador 
	imul EBX 			 ; Int1 * Int2
	mov [ M + 8 ], EAX 			 ; Salva resultado em temporario

	; -- OR -- 
	mov EAX, [ M + 1 ] 			 ; Move o valor de boolean 1 da memoria para registrador
	mov EBX, [ M + 8 ] 			 ; Move o valor de boolean 2 da memoria para registrador
	neg EAX 			 ; EAX = !EAX
	add EAX, 1 			 ; EAX = !EAX
	neg EBX 			 ; EBX = !EBX
	add EBX, 1 			 ; EBX = !EBX
	imul EBX 			 ; !EAX AND !EBX
	neg EAX 			 ; !(!EAX AND !EBX) = EAX OR EBX
	add EAX, 1 			 ; !(!EAX AND !EBX) = EAX OR EBX
	mov [ M + 12 ], EAX 			 ; Salva resultado em temporario

	; -- IF -- 
	mov EAX, [ M  + 12 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L27			 ; Se valor nao for verdadeiro pular bloco do if

	; -- WRITE STRING -- 
	mov RSI, M + 65647 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L28: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L29 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L28  ; Se caractere lido != flag de fim de string continuar loop
L29: ; Fim do loop
	mov RSI, M + 65647 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	mov RSI, M + 0
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
L27: 			 ; fecha bloco do if
	mov EAX, [ M   + 12 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L30			 ; Se valor nao for falso pular bloco do else

	; -- CONST CHAR -- 
	mov AL, '+' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L31			 ; Salta para L31 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L32
L31:
	mov EAX, 1 			 ; Define registrador como true
L32:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L33			 ; Se valor nao for verdadeiro pular bloco do if

	; -- FLOAT (+|-) FLOAT -- 
	movss XMM0, [ M + 65540 ] 			 ; Move o valor de float 1 da memoria para registrador
	movss XMM1, [ M + 65544 ] 			 ; Move o valor de float 2 da memoria para registrador
	addss XMM0, XMM1 			 ; float1 + float2
	movss [ M + 0 ], XMM0 			 ;  Salva resultado em temporario

	; -- ATRIB FLOAT := FLOAT -- 
	movss XMM0, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	movss [ M + 65548 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB FLOAT := FLOAT -- 
L33: 			 ; fecha bloco do if
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L34			 ; Se valor nao for falso pular bloco do else

	; -- CONST CHAR -- 
	mov AL, '-' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L35			 ; Salta para L35 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L36
L35:
	mov EAX, 1 			 ; Define registrador como true
L36:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L37			 ; Se valor nao for verdadeiro pular bloco do if

	; -- FLOAT (+|-) FLOAT -- 
	movss XMM0, [ M + 65540 ] 			 ; Move o valor de float 1 da memoria para registrador
	movss XMM1, [ M + 65544 ] 			 ; Move o valor de float 2 da memoria para registrador
	subss XMM0, XMM1 			 ; float1 - float2
	movss [ M + 0 ], XMM0 			 ;  Salva resultado em temporario

	; -- ATRIB FLOAT := FLOAT -- 
	movss XMM0, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	movss [ M + 65548 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB FLOAT := FLOAT -- 
L37: 			 ; fecha bloco do if
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L38			 ; Se valor nao for falso pular bloco do else

	; -- CONST CHAR -- 
	mov AL, '*' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L39			 ; Salta para L39 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L40
L39:
	mov EAX, 1 			 ; Define registrador como true
L40:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L41			 ; Se valor nao for verdadeiro pular bloco do if

	; -- FLOAT * FLOAT -- 
	movss XMM0, [ M + 65540 ] 			 ; Move o valor de float 1 da memoria para registrador
	movss XMM1, [ M + 65544 ] 			 ; Move o valor de float 2 da memoria para registrador
	mulss XMM0, XMM1  			 ; Float1 * Float2
	movss [ M + 0 ], XMM0 			 ; Salva resultado em temporario

	; -- ATRIB FLOAT := FLOAT -- 
	movss XMM0, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	movss [ M + 65548 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB FLOAT := FLOAT -- 
L41: 			 ; fecha bloco do if
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L42			 ; Se valor nao for falso pular bloco do else

	; -- CONST CHAR -- 
	mov AL, '/' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L43			 ; Salta para L43 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L44
L43:
	mov EAX, 1 			 ; Define registrador como true
L44:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L45			 ; Se valor nao for verdadeiro pular bloco do if

	; -- FLOAT / FLOAT -- 
	movss XMM0, [ M + 65540 ] 			; Move o valor de float 1 da memoria para registrador
	movss XMM1, [ M + 65544 ] 			; Move o valor de float 2 da memoria para registrador
	divss XMM0, XMM1 			; float1 / float2
	movss [ M + 0 ], XMM0 			;  Salva resultado em temporario

	; -- ATRIB FLOAT := FLOAT -- 
	movss XMM0, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	movss [ M + 65548 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB FLOAT := FLOAT -- 
L45: 			 ; fecha bloco do if
L42: 			 ; fecha bloco do if
L38: 			 ; fecha bloco do if
L34: 			 ; fecha bloco do if
L30: 			 ; fecha bloco do if

	; -- WRITE STRING -- 
	mov RSI, M + 65662 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L46: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L47 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L46  ; Se caractere lido != flag de fim de string continuar loop
L47: ; Fim do loop
	mov RSI, M + 65662 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	movss XMM0, [ M + 65548 ] 			 ; Real a ser convertido
	mov RSI, M + 0 			 ; End. temporário
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 6 			 ; Precisão 6 casas compart
	mov RBX, 10 			 ; Divisor
	cvtsi2ss XMM2, RBX 			 ; Divisor real
	subss XMM1, XMM1 			 ; Zera registrador
	comiss XMM0, XMM1 			 ; Verifica sinal
	jae L48 			 ; Salta se número positivo
	mov DL, '-' 			; Senão, escreve sinal –
	mov [RSI], DL
	mov RDX, -1 			 ; Carrega -1 em RDX
	cvtsi2ss XMM1, RDX 			 ; Converte para real
	mulss XMM0, XMM1 			 ; Toma módulo
	add RSI, 1 			 ; Incrementa índice
L48: 
	roundss XMM1, XMM0, 0b0011 			 ; Parte inteira XMM1
	subss XMM0, XMM1 			 ; Parte frac XMM0
	cvtss2si rax, XMM1 			 ; Convertido para int
	; Converte parte inteira que está em rax
L49: 
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push RDX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L49 			 ; Se não é 0, continua
	sub RDI, RCX 			;decrementa precisao
	; Agora, desemp valores e escreve parte int
L50:
	pop RDX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L50 			 ; Se não pilha vazia, loop
	mov DL, '.' 			 ; Escreve ponto decimal
	mov [RSI], DL
	add RSI, 1 			 ; Incrementa base
	; Converte parte fracionaria que está em XMM0
L51:
	cmp RDI, 0 			 ; Verifica precisao
	jle L52 			 ; Terminou precisao ?
	mulss XMM0,XMM2 			 ; Desloca para esquerda
	roundss XMM1,XMM0,0b0011 			 ; Parte inteira XMM1
	subss XMM0,XMM1 			 ; Atualiza XMM0
	cvtss2si RDX, XMM1			 ; Convertido para int
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RDI, 1 			 ; Decrementa precisão
	jmp L51
	; Impressão
L52:
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
	jmp L2			 ; Realiza o loop do while
L1: 			 ; Fim do while 
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
