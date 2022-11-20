section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
; *** Definicoes de variaveis e constantes ***
	resd 1			; Declaracao Var Inteiro em [65536]
	resd 1			; Declaracao Var Inteiro em [65540]
	resd 1			; Declaracao Var Inteiro em [65544]
	resd 1			; Declaracao Var Inteiro em [65548]
	resb 1			; Declaracao Var Character em [65552]
	db "digite o primeiro numero inteiro: ", 0 			 ; Declaracao String M em [ 65553 ]
	db "digite a operacao (+,-,*,d,m,/): ", 0 			 ; Declaracao String M em [ 65588 ]
	db "digite o primeiro numero inteiro: ", 0 			 ; Declaracao String M em [ 65622 ]
	db "operacao por 9", 0 			 ; Declaracao String M em [ 65657 ]
	db "resultado: ", 0 			 ; Declaracao String M em [ 65672 ]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
; *** Comandos *** 

	; -- WHILE -- 
L2:  			 ; inicio while 

	; -- CONST BOOLEAN -- 
	mov EAX, 1			 ; Move Boolean imediado para registrador EAX
	mov [ M + 0 ], EAX 			 ; Move valor do registrador para memoria temporaria
	; -- END CONST BOOLEAN -- 

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

	; -- READ INT -- 
	mov RSI, M + 0 			 ; Salva o endereco do buffer
	mov RDX, 100h 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

	mov EAX, 0 			 ; Acumulador
	mov EBX, 0 			 ; Caractere
	mov ECX, 10 			 ; Base 10
	mov RDX, 1 			 ; Sinal
	mov RSI, M + 0 			 ; End. buffer
	mov BL, [RSI] 			 ; Carrega caractere
	cmp BL, '-' 			 ; Sinal - ?
	jne L5 			 ; Se dif -, Salta
	mov RDX, -1 			 ; Senão, armazena -
	add RSI, 1 			 ; Inc. ponteiro string
	mov BL, [RSI] 			 ; Carrega caractere
L5:
	push RDX 			 ; Empilha sinal
	mov EDX, 0 			 ; Reg. multiplicação
L6:
	cmp BL, 0Ah 			 ; Verifica fim string
	je L7 			 ; Salta se fim string
	imul ECX 			 ; Mult. EAX por 10
	sub BL, '0' 			 ; Converte caractere
	add EAX, EBX 			 ; Soma valor caractere
	add RSI, 1 			 ; Incrementa base
	mov BL, [RSI] 			 ; Carrega caractere
	jmp L6 			; Loop
L7:
	pop CX 			 ; Desempilha sinal
	cmp CX, 0
	jg L8
	neg EAX 			 ; Mult. sinal
L8:
	mov [ M + 65540 ], EAX 			; Carrega o valor para o indentificador
	; -- END READ INT -- 

	; -- WRITE STRING -- 
	mov RSI, M + 65588 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L9: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L10 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L9  ; Se caractere lido != flag de fim de string continuar loop
L10: ; Fim do loop
	mov RSI, M + 65588 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 

	; -- READ CHAR -- 
	mov RSI, M + 65552 			 ; Salva o endereco do buffer
	mov RDX, 1 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

; Limpa o buffer para a proxima leitura
L12:
	mov RDX, 1 			 ; Tamanho do buffer
	mov RSI, M + 0 			 ; Salva o endereco do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

	mov AL,[ M + 0 ]
	cmp AL, 0xA 			 ; Verifica se e nova linha
	jne L12			 ; Le o proximo se nao for nova linha

	; -- END READ CHAR -- 

	; -- WRITE STRING -- 
	mov RSI, M + 65622 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L13: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L14 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L13  ; Se caractere lido != flag de fim de string continuar loop
L14: ; Fim do loop
	mov RSI, M + 65622 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 

	; -- READ INT -- 
	mov RSI, M + 0 			 ; Salva o endereco do buffer
	mov RDX, 100h 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

	mov EAX, 0 			 ; Acumulador
	mov EBX, 0 			 ; Caractere
	mov ECX, 10 			 ; Base 10
	mov RDX, 1 			 ; Sinal
	mov RSI, M + 0 			 ; End. buffer
	mov BL, [RSI] 			 ; Carrega caractere
	cmp BL, '-' 			 ; Sinal - ?
	jne L15 			 ; Se dif -, Salta
	mov RDX, -1 			 ; Senão, armazena -
	add RSI, 1 			 ; Inc. ponteiro string
	mov BL, [RSI] 			 ; Carrega caractere
L15:
	push RDX 			 ; Empilha sinal
	mov EDX, 0 			 ; Reg. multiplicação
L16:
	cmp BL, 0Ah 			 ; Verifica fim string
	je L17 			 ; Salta se fim string
	imul ECX 			 ; Mult. EAX por 10
	sub BL, '0' 			 ; Converte caractere
	add EAX, EBX 			 ; Soma valor caractere
	add RSI, 1 			 ; Incrementa base
	mov BL, [RSI] 			 ; Carrega caractere
	jmp L16 			; Loop
L17:
	pop CX 			 ; Desempilha sinal
	cmp CX, 0
	jg L18
	neg EAX 			 ; Mult. sinal
L18:
	mov [ M + 65544 ], EAX 			; Carrega o valor para o indentificador
	; -- END READ INT -- 

	; -- CONST CHAR -- 
	mov AL, 'd' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L19			 ; Salta para L19 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L20
L19:
	mov EAX, 1 			 ; Define registrador como true
L20:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- CONST CHAR -- 
	mov AL, 'm' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

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
	; -- END CHAR CMP CHAR -- 

	; -- CONST INT -- 
	mov EAX, 0 			 ; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			 ; Move registrador para memoria temporaria
	; -- END CONST INT -- 

	; -- INT = INT -- 
	mov EAX, [ M + 65544 ] 			 ; Move o valor de int 1 da memoria para registrador
	mov EBX, [ M + 0 ] 			 ; Move o valor de int 2 da memoria para registrador
	cmp EAX, EBX 			 ; Compara int1 com int2
	je L23			 ; Salta para L23 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L24
L23:
	mov EAX, 1 			 ; Define registrador como true
L24:
	mov [ M + 4 ], EAX 			 ; Salva resultado em temporario
	; -- END INT = INT -- 

	; -- INT * INT -- 
	mov EAX, [ M + 1 ] 			 ; Move o valor de int 1 da memoria para registrador 
	mov EBX, [ M + 4 ] 			 ; Move o valor de int 2 da memoria para registrador 
	imul EBX 			 ; Int1 * Int2
	mov [ M + 8 ], EAX 			 ; Salva resultado em temporario
	; -- END INT * INT -- 

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
	; -- END OR -- 

	; -- IF -- 
	mov EAX, [ M  + 12 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L25			 ; Se valor nao for verdadeiro pular bloco do if

	; -- WRITE STRING -- 
	mov RSI, M + 65657 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L26: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L27 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L26  ; Se caractere lido != flag de fim de string continuar loop
L27: ; Fim do loop
	mov RSI, M + 65657 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	; -- LINE BREAK -- 
	mov RSI, M + 0
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 
L25: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- ELSE -- 
	mov EAX, [ M   + 12 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L28			 ; Se valor nao for falso pular bloco do else
	; -- END ELSE -- 

	; -- CONST CHAR -- 
	mov AL, '+' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L29			 ; Salta para L29 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L30
L29:
	mov EAX, 1 			 ; Define registrador como true
L30:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L31			 ; Se valor nao for verdadeiro pular bloco do if

	; -- INT (+|-) INT -- 
	mov EAX, [ M + 65540 ] 			 ; Move o valor de int 1 da memoria para registrador
	mov EBX, [ M + 65544 ] 			 ; Move o valor de int 2 da memoria para registrador
	add EAX, EBX 			 ; int1 + int2
	mov [ M + 0 ], EAX 			 ; Salva resultado em temporario
	; -- END INT (+|-) INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65548 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 
L31: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- ELSE -- 
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L32			 ; Se valor nao for falso pular bloco do else
	; -- END ELSE -- 

	; -- CONST CHAR -- 
	mov AL, '-' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L33			 ; Salta para L33 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L34
L33:
	mov EAX, 1 			 ; Define registrador como true
L34:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L35			 ; Se valor nao for verdadeiro pular bloco do if

	; -- INT (+|-) INT -- 
	mov EAX, [ M + 65540 ] 			 ; Move o valor de int 1 da memoria para registrador
	mov EBX, [ M + 65544 ] 			 ; Move o valor de int 2 da memoria para registrador
	sub EAX, EBX 			 ; int1 - int2
	mov [ M + 0 ], EAX 			 ; Salva resultado em temporario
	; -- END INT (+|-) INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65548 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 
L35: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- ELSE -- 
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L36			 ; Se valor nao for falso pular bloco do else
	; -- END ELSE -- 

	; -- CONST CHAR -- 
	mov AL, '*' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L37			 ; Salta para L37 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L38
L37:
	mov EAX, 1 			 ; Define registrador como true
L38:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L39			 ; Se valor nao for verdadeiro pular bloco do if

	; -- INT * INT -- 
	mov EAX, [ M + 65540 ] 			 ; Move o valor de int 1 da memoria para registrador 
	mov EBX, [ M + 65544 ] 			 ; Move o valor de int 2 da memoria para registrador 
	imul EBX 			 ; Int1 * Int2
	mov [ M + 0 ], EAX 			 ; Salva resultado em temporario
	; -- END INT * INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65548 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 
L39: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- ELSE -- 
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L40			 ; Se valor nao for falso pular bloco do else
	; -- END ELSE -- 

	; -- CONST CHAR -- 
	mov AL, 'd' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L41			 ; Salta para L41 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L42
L41:
	mov EAX, 1 			 ; Define registrador como true
L42:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L43			 ; Se valor nao for verdadeiro pular bloco do if

	; -- INT DIV INT -- 
	mov EAX, [ M + 65540 ] 			 ; Move o valor de int 1 da memoria para registrador 
	mov EBX, [ M + 65544 ] 			 ; Move o valor de int 2 da memoria para registrador 
	idiv EBX 			 ; int1 div int2
	mov [ M + 0 ], EAX 			 ; Salva resultado em temporario
	; -- END INT DIV INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65548 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 
L43: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- ELSE -- 
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L44			 ; Se valor nao for falso pular bloco do else
	; -- END ELSE -- 

	; -- CONST CHAR -- 
	mov AL, 'm' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L45			 ; Salta para L45 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L46
L45:
	mov EAX, 1 			 ; Define registrador como true
L46:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L47			 ; Se valor nao for verdadeiro pular bloco do if

	; -- INT MOD INT -- 
	mov EAX, [ M + 65540 ] 			 ; Move o valor de int 1 da memoria para o registrador
	mov EBX, [ M + 65544 ] 			 ; Move o valor de int 2 da memoria para o registrador
	idiv EBX 			 ; int1 mod int2
	mov [ M + 0 ], EDX 			 ; Salva resultado em temporario
	; -- END INT MOD INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 0 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65548 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 
L47: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- ELSE -- 
	mov EAX, [ M   + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 0 			 ; Comparacao com booleano false
	jne L48			 ; Se valor nao for falso pular bloco do else
	; -- END ELSE -- 

	; -- CONST CHAR -- 
	mov AL, '/' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 65552 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 0 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	je L49			 ; Salta para L49 se int1 = int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L50
L49:
	mov EAX, 1 			 ; Define registrador como true
L50:
	mov [ M + 1 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- IF -- 
	mov EAX, [ M  + 1 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L51			 ; Se valor nao for verdadeiro pular bloco do if

	; -- INT / INT -- 
	mov EAX, [ M + 65540 ] 			 ; Move o valor de int 1 da memoria para registrador
	cdqe 			 ; Expandindo o sinal de valor em EAX
	cvtsi2ss XMM0, RAX 			 ; Expande int1 para float1
	mov EAX, [ M + 65544 ] 			 ; Move o valor de int 2 da memoria para registrador
	cdqe 			 ; Expandindo o sinal de valor em EAX
	cvtsi2ss XMM1, RAX 			 ; Expande int2 para float2
	divss XMM0, XMM1 			 ; float(int1) / float(int2)
	movss [ M + 0 ], XMM0 			;  Salva resultado em temporario
	; -- END INT / INT -- 

	; -- CASTING INT -- 
	subss XMM0, XMM0 ; Zera o XMM0
	movss XMM0, [ M + 0 ] 			; Move o valor de float da memoria para o registrador XMM0
	mov RAX, 0 ; Zera o RAX
	cvtss2si RAX, XMM0 			; Converte int para float
	mov [ M + 4 ], RAX 			 ; Salva valor convertido para temporario
	; -- END CASTING INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 4 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65548 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 
L51: 			 ; fecha bloco do if
	; -- END IF -- 
L48: 			 ; fecha bloco do if
	; -- END IF -- 
L44: 			 ; fecha bloco do if
	; -- END IF -- 
L40: 			 ; fecha bloco do if
	; -- END IF -- 
L36: 			 ; fecha bloco do if
	; -- END IF -- 
L32: 			 ; fecha bloco do if
	; -- END IF -- 
L28: 			 ; fecha bloco do if
	; -- END IF -- 

	; -- WRITE STRING -- 
	mov RSI, M + 65672 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L52: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L53 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L52  ; Se caractere lido != flag de fim de string continuar loop
L53: ; Fim do loop
	mov RSI, M + 65672 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 

	; -- WRITE INT --
	mov EAX, 0 			 ; Zera registrador
	mov RDI, 0 			 ; Zera registrador
	mov EAX, [ M + 65548 ] 			; Inteiro a ser convertido
	mov RDI, M + 0 			 ; End. string ou temp.
	mov RCX, 0 			 ; Contador pilha
	mov RSI, 0 			 ; Tam. string convertido
	cmp EAX, 0 			 ; Verifica sinal
	jge L54 			 ; Salta se número positivo
	mov BL, '-' 			 ; Senão, escreve sinal – 
	mov [RDI], BL 			 ; Armazena o sinal no inicio do buffer 
	add RDI, 1 			 ; Incrementa tamanho
	neg EAX 			 ; Toma módulo do número
L54:
	mov EBX, 10 			 ; Divisor
	mov EDX, 0 			 ; Limpa a parte alta do dividendo
	idiv EBX 			 ; Divide EDX:EAX por EBX
	push DX 			 ; Empilha valor do resto
	add RCX, 1 			 ; Incrementa o contador
	cmp EAX, 0 			 ; Verifica se ainda resta valor para converter
	jne L54 			 ; Continua a conversao caso necessario
L55:
	pop AX 			 ; Desempilha valor
	add AX, '0' 			 ; Transforma em caractere
	mov [RDI], AL 			 ; Escreve caractere
	add RDI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jg L55			 ; Se não pilha vazia, loop
	mov [RDI], byte 0 ; Escreve o marcador de fim de string no buffer

	sub RDI, M + 0 ; Tamanho da string real
	mov RSI, M + 0 ; ou buffer.end
	mov RDX, RDI ; ou buffer.tam
	mov RAX, 1 ; chamada para saida
	mov RDI, 1 ; saida para tela
	syscall
	; -- END WRITE INT --
	; -- LINE BREAK -- 
	mov RSI, M + 4
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 
	jmp L2			 ; Realiza o loop do while
L1: 			 ; Fim do while 
	; -- END WHILE -- 

	; -- CONST INT -- 
	mov EAX, 10 			 ; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			 ; Move registrador para memoria temporaria
	; -- END CONST INT -- 

	; -- WRITE INT --
	mov EAX, 0 			 ; Zera registrador
	mov RDI, 0 			 ; Zera registrador
	mov EAX, [ M + 0 ] 			; Inteiro a ser convertido
	mov RDI, M + 4 			 ; End. string ou temp.
	mov RCX, 0 			 ; Contador pilha
	mov RSI, 0 			 ; Tam. string convertido
	cmp EAX, 0 			 ; Verifica sinal
	jge L56 			 ; Salta se número positivo
	mov BL, '-' 			 ; Senão, escreve sinal – 
	mov [RDI], BL 			 ; Armazena o sinal no inicio do buffer 
	add RDI, 1 			 ; Incrementa tamanho
	neg EAX 			 ; Toma módulo do número
L56:
	mov EBX, 10 			 ; Divisor
	mov EDX, 0 			 ; Limpa a parte alta do dividendo
	idiv EBX 			 ; Divide EDX:EAX por EBX
	push DX 			 ; Empilha valor do resto
	add RCX, 1 			 ; Incrementa o contador
	cmp EAX, 0 			 ; Verifica se ainda resta valor para converter
	jne L56 			 ; Continua a conversao caso necessario
L57:
	pop AX 			 ; Desempilha valor
	add AX, '0' 			 ; Transforma em caractere
	mov [RDI], AL 			 ; Escreve caractere
	add RDI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jg L57			 ; Se não pilha vazia, loop
	mov [RDI], byte 0 ; Escreve o marcador de fim de string no buffer

	sub RDI, M + 4 ; Tamanho da string real
	mov RSI, M + 4 ; ou buffer.end
	mov RDX, RDI ; ou buffer.tam
	mov RAX, 1 ; chamada para saida
	mov RDI, 1 ; saida para tela
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
