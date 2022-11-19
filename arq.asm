section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
; *** Definicoes de variaveis e constantes ***
	resd 1			; Declaracao Var Inteiro em [65536]
	dd 1 			 ; Declaracao Const Inteiro em [ 65540 ]
	resb 100h			; Declaracao Var String em [65544]
	dd 10 			 ; Declaracao Const Inteiro em [ 65800 ]
	db "digite seu nome: ", 0 			 ; Declaracao String M em [ 65804 ]
	resd 1			; Declaracao Var Float em [65822]
	db ": ola' ", 0 			 ; Declaracao String M em [ 65826 ]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
; *** Comandos *** 

	; -- ATRIB STRING := STRING -- 
	mov RSI, M + 65536 			 ; Copiando endereço da string para um registrador de índice
	mov RDI, M + 65792 			 ; Copiando endereço da string para um registrador de índice
	; Loop para calcular tamanho da string
L1: 			 ; Inicio do loop
	mov AL, [RDI] 			 ; Leitura de caracter na posicao RSI da memória
	mov [RSI], AL 			; Salva caracter no identificador
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L2 
 ; Se caracter lido = flag de fim de string finalizar loop
	add RDI, 1 			 ; Incrementando indice da string1 
	add RSI, 1 			 ; Incrementando indice da string2 
	jmp L1 			 ; Se caracter lido != flag de fim de string continuar loop
L2: ; Fim do loop
	; -- END ATRIB STRING := STRING -- 

	; -- CONST INT -- 
	mov EAX, 2 			 ; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			 ; Move registrador para memoria temporaria
	; -- END CONST INT -- 

	; -- STRING POSITION -- 
	mov EBX, [ M + 0 ] 			 ; Recupera valor de indice da string da memoria
	add EBX, M + 65536 			 ; Soma indice com valor de memoria da string
	mov EAX, [EBX] 			 ; Escreve valor de caracter para o registrador AL
	mov [ M + 4 ], EAX 			 ; Salva character em temporario
	; -- END STRING POSITION -- 

	; -- WRITE CHAR --

	mov RSI, M + 4 			 ; Copiando endereço do character para um registrador de índice
	mov RDX, 1 			 ; Quantidade de caracteres imprimíveis 
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE CHAR --

	; -- LINE BREAK -- 
	mov RSI, M + 5
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 

	; -- CONST CHAR -- 
	mov AL, 'a' 			 ; Move Char imediato para registrador
	mov [ M + 0 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CONST CHAR -- 
	mov AL, 'b' 			 ; Move Char imediato para registrador
	mov [ M + 1 ], AL 			 ; Move registrador para memoria temporaria
	; -- END CONST CHAR -- 

	; -- CHAR CMP CHAR -- 
	mov AL, [ M + 0 ] 			; Move o valor de char 1 da memoria para registrador
	mov BL, [ M + 1 ] 			; Move o valor de char 2 da memoria para registrador
	cmp AL, BL 			 ; compara caracteres
	jg L3			 ; Salta para L3 se int1 > int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L4
L3:
	mov EAX, 1 			 ; Define registrador como true
L4:
	mov [ M + 2 ], EAX 			 ; Salva resultado em temporario
	; -- END CHAR CMP CHAR -- 

	; -- WRITE BOOLEAN -- 
	mov EAX , [ M + 2 ] 			 ; Carrega valor de boolean
	cmp EAX , 0 			 ; Se boolean for falso
	jne L5			; Se boolean for verdadeiro -> label true

	; -- END WRITE BOOLEAN -- 

	; -- WRITE STRING -- 
	mov RSI, M + 65804 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L1: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L2 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L1  ; Se caractere lido != flag de fim de string continuar loop
L2: ; Fim do loop
	mov RSI, M + 65804 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 

	; -- READ STR -- 
	mov RSI, M+65544 			 ; Salva o endereco do buffer
	mov RDX, 100h 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

; Leitura string
	add RAX, RSI
	sub RAX, 1
L3:
	mov DL, [RSI] 			 ; Move caractere para 
	cmp DL, byte 10 			 ; Compara com quebra de linha
	je L4 			 ; Se for quebra de linha Salta
	cmp RSI, RAX
	je L4
	add RSI, 1 			 ; Incrementa endereco da String
	jmp L3

L4:
	mov DL, 0 			 ; Substitui quebra de linha por fim de string
	mov [RSI], DL 			 ; Move fim de string para o identificador
	; -- END READ STR -- 

	; -- WHILE -- 
L6:  			 ; inicio while 

	; -- INT <= INT -- 
	mov EAX, [ M + 65536 ] 			 ; Move o valor de int 1 da memoria para registrador
	mov EBX, [ M + 65800 ] 			 ; Move o valor de int 2 da memoria para registrador
	cmp EAX, EBX 			 ; Compara int1 com int2
	jle L7			 ; Salta para L7 se int1 <= int2
	mov EAX, 0 			 ; Define registrador como falso
	jmp L8
L7:
	mov EAX, 1 			 ; Define registrador como true
L8:
	mov [ M + 0 ], EAX 			 ; Salva resultado em temporario
	; -- END INT <= INT -- 

	; -- CMP WHILE -- 
	mov EAX, [ M  + 0 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L5			 ; Se valor nao for verdadeiro pular bloco do while

	; -- CASTING FLOAT -- 
	mov EAX, 0 ; Zera o EAX
	mov RAX, 0 ; Zera o RAX
	mov EAX, [ M + 65536 ] 			; Move o valor de int da memoria para o registrador RAX
	cdqe 			; Expandindo o sinal de valor em RAX
	subss XMM0, XMM0 ; Zera o XMM0
	cvtsi2ss XMM0, RAX 			; Converte float para int
	movss [ M + 0 ], XMM0 			 ; Salva valor convertido para temporario
	; -- END CASTING FLOAT -- 

	; -- FLOAT / INT -- 
	movss XMM0, [ M + 0 ] 			 ; Move o valor de float da memoria para registrador
	mov EAX, [ M + 65800 ] 			 ; Move o valor de int da memoria para registrador
	cdqe 			 ; Expandindo o sinal de valor em EAX
	cvtsi2ss XMM1, RAX 			 ; Expande int para float
	divss XMM0, XMM1 			 ; float / float(int)
	movss [ M + 4 ], XMM0 			 ;  Salva resultado em temporario
	; -- END FLOAT / INT -- 

	; -- ATRIB FLOAT := FLOAT -- 
	movss XMM0, [ M + 4 ] 			 ; Recupera valor do identificador da memoria
	movss [M + 65822 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB FLOAT := FLOAT -- 

	; -- WRITE FLOAT -- 
	movss XMM0, [ M + 65822 ] 			 ; Real a ser convertido
	mov RSI, M + 0 			 ; End. temporário
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 6 			 ; Precisão 6 casas compart
	mov RBX, 10 			 ; Divisor
	cvtsi2ss XMM2, RBX 			 ; Divisor real
	subss XMM1, XMM1 			 ; Zera registrador
	comiss XMM0, XMM1 			 ; Verifica sinal
	jae L9 			 ; Salta se número positivo
	mov DL, '-' 			; Senão, escreve sinal –
	mov [RSI], DL
	mov RDX, -1 			 ; Carrega -1 em RDX
	cvtsi2ss XMM1, RDX 			 ; Converte para real
	mulss XMM0, XMM1 			 ; Toma módulo
	add RSI, 1 			 ; Incrementa índice
L9: 
	roundss XMM1, XMM0, 0b0011 			 ; Parte inteira XMM1
	subss XMM0, XMM1 			 ; Parte frac XMM0
	cvtss2si rax, XMM1 			 ; Convertido para int
	; Converte parte inteira que está em rax
L10: 
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push DX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L10 			 ; Se não é 0, continua
	sub RDI, RCX 			;decrementa precisao
	; Agora, desemp valores e escreve parte int
L11:
	pop DX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L11 			 ; Se não pilha vazia, loop
	mov DL, '.' 			 ; Escreve ponto decimal
	mov [RSI], DL
	add RSI, 1 			 ; Incrementa base
	; Converte parte fracionaria que está em XMM0
L12:
	cmp RDI, 0 			 ; Verifica precisao
	jle L13 			 ; Terminou precisao ?
	mulss XMM0,XMM2 			 ; Desloca para esquerda
	roundss XMM1,XMM0,0b0011 			 ; Parte inteira XMM1
	subss XMM0,XMM1 			 ; Atualiza XMM0
	cvtss2si RDX, XMM1			 ; Convertido para int
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RDI, 1 			 ; Decrementa precisão
	jmp L12
	; Impressão
L13:
	mov DL, 0 			 ; Fim string, opcional
	mov [RSI], DL 			 ; Escreve caractere
	mov RDX, RSI ; Calc tam str convertido
	mov RBX, M + 0 
	sub RDX, RBX 			 ; Tam=RSI-M-buffer.end
	mov RSI, M + 0 			; Endereço do buffer
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	; -- END WRITE FLOAT -- 

	; -- WRITE STRING -- 
	mov RSI, M + 65826 			 ; Copiando endereço da string para um registrador de índice
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
	mov RSI, M + 65826 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 

	; -- WRITE STRING -- 
	mov RSI, M + 65544 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L16: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L17 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L16  ; Se caractere lido != flag de fim de string continuar loop
L17: ; Fim do loop
	mov RSI, M + 65544 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE STRING -- 
	; -- LINE BREAK -- 
	mov RSI, M + 6
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 

	; -- CONST INT -- 
	mov EAX, 4021521521 			 ; Move Inteiro imediato para registrador
	mov [ M + 7 ], EAX 			 ; Move registrador para memoria temporaria
	; -- END CONST INT -- 

	; -- ATRIB INT := INT -- 
	mov EAX, [ M + 7 ] 			 ; Recupera valor do identificador da memoria
	mov [ M + 65810 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	; -- END ATRIB INT := INT -- 

	; -- WRITE FLOAT -- 
	movss XMM0, [ M + 65814 ] 			 ; Real a ser convertido
	mov RSI, M + 0 			 ; End. temporário
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 6 			 ; Precisão 6 casas compart
	mov RBX, 10 			 ; Divisor
	cvtsi2ss XMM2, RBX 			 ; Divisor real
	subss XMM1, XMM1 			 ; Zera registrador
	comiss XMM0, XMM1 			 ; Verifica sinal
	jae L11 			 ; Salta se número positivo
	mov DL, '-' 			; Senão, escreve sinal –
	mov [RSI], DL
	mov RDX, -1 			 ; Carrega -1 em RDX
	cvtsi2ss XMM1, RDX 			 ; Converte para real
	mulss XMM0, XMM1 			 ; Toma módulo
	add RSI, 1 			 ; Incrementa índice
L11: 
	roundss XMM1, XMM0, 0b0011 			 ; Parte inteira XMM1
	subss XMM0, XMM1 			 ; Parte frac XMM0
	cvtss2si rax, XMM1 			 ; Convertido para int
	; Converte parte inteira que está em rax
L12: 
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push DX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L12 			 ; Se não é 0, continua
	sub RDI, RCX 			;decrementa precisao
	; Agora, desemp valores e escreve parte int
L13:
	pop DX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L13 			 ; Se não pilha vazia, loop
	mov DL, '.' 			 ; Escreve ponto decimal
	mov [RSI], DL
	add RSI, 1 			 ; Incrementa base
	; Converte parte fracionaria que está em XMM0
L14:
	cmp RDI, 0 			 ; Verifica precisao
	jle L15 			 ; Terminou precisao ?
	mulss XMM0,XMM2 			 ; Desloca para esquerda
	roundss XMM1,XMM0,0b0011 			 ; Parte inteira XMM1
	subss XMM0,XMM1 			 ; Atualiza XMM0
	cvtss2si RDX, XMM1			 ; Convertido para int
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RDI, 1 			 ; Decrementa precisão
	jmp L14
	; Impressão
L15:
	mov DL, 0 			 ; Fim string, opcional
	mov [RSI], DL 			 ; Escreve caractere
	mov RDX, RSI ; Calc tam str convertido
	mov RBX, M + 0 
	sub RDX, RBX 			 ; Tam=RSI-M-buffer.end
	mov RSI, M + 0 			; Endereço do buffer
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	; -- END WRITE FLOAT -- 

	; -- WRITE FLOAT -- 
	movss XMM0, [ M + 65818 ] 			 ; Real a ser convertido
	mov RSI, M + 0 			 ; End. temporário
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 6 			 ; Precisão 6 casas compart
	mov RBX, 10 			 ; Divisor
	cvtsi2ss XMM2, RBX 			 ; Divisor real
	subss XMM1, XMM1 			 ; Zera registrador
	comiss XMM0, XMM1 			 ; Verifica sinal
	jae L16 			 ; Salta se número positivo
	mov DL, '-' 			; Senão, escreve sinal –
	mov [RSI], DL
	mov RDX, -1 			 ; Carrega -1 em RDX
	cvtsi2ss XMM1, RDX 			 ; Converte para real
	mulss XMM0, XMM1 			 ; Toma módulo
	add RSI, 1 			 ; Incrementa índice
L16: 
	roundss XMM1, XMM0, 0b0011 			 ; Parte inteira XMM1
	subss XMM0, XMM1 			 ; Parte frac XMM0
	cvtss2si rax, XMM1 			 ; Convertido para int
	; Converte parte inteira que está em rax
L17: 
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push DX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L17 			 ; Se não é 0, continua
	sub RDI, RCX 			;decrementa precisao
	; Agora, desemp valores e escreve parte int
L18:
	pop DX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L18 			 ; Se não pilha vazia, loop
	mov DL, '.' 			 ; Escreve ponto decimal
	mov [RSI], DL
	add RSI, 1 			 ; Incrementa base
	; Converte parte fracionaria que está em XMM0
L19:
	cmp RDI, 0 			 ; Verifica precisao
	jle L20 			 ; Terminou precisao ?
	mulss XMM0,XMM2 			 ; Desloca para esquerda
	roundss XMM1,XMM0,0b0011 			 ; Parte inteira XMM1
	subss XMM0,XMM1 			 ; Atualiza XMM0
	cvtss2si RDX, XMM1			 ; Convertido para int
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RDI, 1 			 ; Decrementa precisão
	jmp L19
	; Impressão
L20:
	mov DL, 0 			 ; Fim string, opcional
	mov [RSI], DL 			 ; Escreve caractere
	mov RDX, RSI ; Calc tam str convertido
	mov RBX, M + 0 
	sub RDX, RBX 			 ; Tam=RSI-M-buffer.end
	mov RSI, M + 0 			; Endereço do buffer
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	; -- END WRITE FLOAT -- 
	; -- LINE BREAK -- 
	mov RSI, M + 4
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 

	; -- CONST INT -- 
	mov EAX, 1 			 ; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			 ; Move registrador para memoria temporaria
	; -- END CONST INT -- 

	; -- INT (+|-) INT -- 
	mov EAX, [ M + 65536 ] 			 ; Move o valor de int 1 da memoria para registrador
	mov EBX, [ M + 0 ] 			 ; Move o valor de int 2 da memoria para registrador
	add EAX, EBX 			 ; int1 + int2
	mov [ M + 4 ], EAX 			 ; Salva resultado em temporario
	; -- END INT (+|-) INT -- 

	; -- WRITE INT --
	mov EAX, [ M + 8 ] 			; Inteiro a ser ;convertido
	mov RSI, M + 12 			 ; End. string ou temp.
	mov RCX, 0 			 ; Contador pilha
	mov RDI, 0 			 ; Tam. string convertido
	cmp EAX, 0 			 ; Verifica sinal
	jge L24 			 ; Salta se número positivo
	mov BL, '-' 			 ; Senão, escreve sinal –
	mov [RSI], BL
	add RSI, 1 			 ; Incrementa índice
	add RDI, 1 			 ; Incrementa tamanho
	neg EAX 			 ; Toma módulo do número
	L24:
	mov EBX, 10 			 ; Divisor
	L25:
	add RCX, 1 			 ; Incrementa contador
	cdq 			 ; Estende EDX:EAX p/ div.
	idiv EBX 			 ; Divide EDX;EAX por EBX
	push DX 			 ; Empilha valor do resto
	cmp EAX, 0 			 ; Verifica se quoc. é 0
	jne L25 			 ; Se não é 0, continua
	add RDI, RCX 			 ; Atualiza tam. string
	; Agora, desemp. os valores e escreve o string
	L26:
	pop DX 			 ; Desempilha valor
	add DL, '0' 			 ; Transforma em caractere
	mov [RSI], DL 			 ; Escreve caractere
	add RSI, 1 			 ; Incrementa base
	sub RCX, 1 			 ; Decrementa contador
	cmp RCX, 0 			 ; Verifica pilha vazia
	jne L26			 ; Se não pilha vazia, loop
	mov RSI, M + 12 			 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 			 ; contador de caracteres = 0
	; Loop para calcular tamanho da string
L27: 			 ; Inicio do loop
	mov AL, [RSI] 			 ; Leitura de caractere na posicao rax da memória
	cmp AL, 0 			 ; Verificação de flag de fim de string
	je L28 			 ; Se caractere lido = flag de fim de string finalizar loop
	add RDX, 1 			 ; Incrementando numero de caracteres
	add RSI, 1 			 ; Incrementando indice da string
	jmp L27  ; Se caractere lido != flag de fim de string continuar loop
L28: ; Fim do loop
	mov RSI, M + 12 			 ; Copiando endereço inicial da string
	mov RAX, 1 			 ; Chamada para saída
	mov RDI, 1 			 ; Chamada para tela
	syscall
	; -- END WRITE INT --
	; -- LINE BREAK -- 
	mov RSI, M + 16
	mov [RSI], byte 10 			 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX, 1 			; Ou buffer.tam
	mov RAX, 1 			; Chamada para saida
	mov RDI, 1 			; Saida para tela
	syscall
	; -- END LINE BREAK -- 
L23: 			 ; fecha bloco do if
	; -- END IF -- 

; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
