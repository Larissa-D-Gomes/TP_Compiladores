section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resd 1			; Declaracao Var Inteiro em [65536]
	resb 100h			; Declaracao Var String em [65540]
	db "digite seu nome: ",0			; Declaracao String EXP em [65796]
	resd 1			; Declaracao Var Float em [65814]
	db ": ola' ",0			; Declaracao String EXP em [65818]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov EAX, 1			; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov EAX, [ M +0 ] 			 ; Recupera valor do identificador da memoria
	mov [M + 65536 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	mov EAX, 10			; Move Inteiro imediato para registrador
	mov [ M + 4 ], EAX 			; Move registrador para posicao atual de memoria em [4]
	;  WRITE STRING

	mov RSI, M + 65796 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 ; contador de caracteres = 0

	; Loop para calcular tamanho da string
L1: ; Inicio do loop
	mov AL, [RSI] ; Leitura de caracter na posicao rax da memória
	cmp AL, 0 ; Verificação de flag de fim de string
	je L2 
 ; Se caracter lido = flag de fim de string finalizar loop
	add RDX, 1 ; Incrementando numero de caracteres
	add RSI, 1 ; Incrementando indice da string
	jmp L1  ; Se caracter lido != flag de fim de string continuar loop
L2: ; Fim do loop
	mov RSI, M + 65796 ; Copiando endereco inicial da string
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	mov RSI, M+65540 			 ; Salva o endereco do buffer
	mov RDX, 100h 			 ; Tamanho do buffer
	mov RAX, 0 			 ; Chamada para leitura
	mov RDI, 0 			 ; Leitura do teclado
	syscall

; Leitura string
	add RAX, RSI
	sub RAX, 1
L3:
	mov DL, [RSI] 			 ; Move caractere para DL
	cmp DL, byte 10 			 ; Compara com quebra de linha
	je L4 			 ; Se for quebra de linha salta
	cmp RSI, RAX
	je L4
	add RSI, 1 			 ; Incrementa endereco da String
	jmp L3

L4:
	mov DL, 0 			 ; Substitui quebra de linha por fim de string
	mov [RSI], DL 			 ; Move fim de string para o identificador
	mov EAX, [ M + 65536 ] 			; Move o valor de int 1 da memoria para o registrador EAX
	mov EBX, [ M + 4 ] 			; Move o valor de int 2 da memoria para o registrador EBX
	cmp EAX, EBX  			; compara int1 com int2
	jle L5			; salta para L5 se int1 <= int2
	mov EAX, 0 ; Define registrador como falso
	jmp L6
L5:
	mov EAX, 1 ; Define registrador como true
L6:
	mov [ M + 0 ], EAX 			; Salva resultado em temporario
L8: 
	mov EAX, [ M   +0 ] 			 ; Recupera valor de booleano da memoria
	cmp EAX, 1 			 ; Comparacao com booleano verdadeiro
	jne L7			 ; Se valor nao for verdadeiro pular bloco do while
	mov EAX, 0 ; Zera o EAX
	mov RAX, 0 ; Zera o RAX
	mov EAX, [ M + 65536 ] 			; Move o valor de int da memoria para o registrador RAX
	cdqe 			; Expandindo o sinal de valor em RAX
	subss XMM0, XMM0 ; Zera o XMM0
	cvtsi2ss XMM0, RAX 			; Converte float para int
	movss [ M + 0 ], XMM0 			 ; Salva valor convertido para temporario
	movss XMM0, [ M + 0 ] 			; Move o valor de float da memoria para o registrador XMM0
	mov EAX, [ M + 4 ] 			; Move o valor de int da memoria para o registrador EAX
	cdqe 			; Expandindo o sinal de valor em EAX
	cvtsi2ss XMM1, RAX 			; Expande int para float
	divss XMM0, XMM1  			; float / float(int)
	movss [ M + 4 ], XMM0 			;  Salva resultado em temporario
	movss XMM0, [ M +4 ] 			 ; Recupera valor do identificador da memoria
	movss [M + 65814 ] , XMM0 			 ; Salva valor do registrador no endereco do ID
; WRITE FLOAT
	movss XMM0, [ M + 65814] 			;real a ser convertido
	mov RSI, M + 0 			; end. temporário
	mov RCX, 0 			;contador pilha
	mov rdi, 6 			;precisao 6 casas compart
	mov rbx, 10 			;divisor
	cvtsi2ss xmm2, rbx 			;divisor real
	subss xmm1, xmm1 			;zera registrador
	comiss xmm0, xmm1 			;verifica sinal
	jae L9 			;salta se número positivo
	mov dl, '-' 			;senão, escreve sinal –
	mov [RSI], dl
	mov rdx, -1 			;Carrega -1 em RDX
	cvtsi2ss xmm1, rdx 			;Converte para real
	mulss xmm0, xmm1 			;Toma módulo
	add RSI, 1 			;incrementa índice
L9: 
	roundss xmm1, xmm0, 0b0011 			;parte inteira xmm1
	subss xmm0, xmm1 			;parte frac xmm0
	cvtss2si rax, xmm1 			;convertido para int
	;converte parte inteira que está em rax
L10: 
	add RCX, 1 			;incrementa contador
	cdq 			;estende edx:eax p/ div.
	idiv ebx 			;divide edx;eax por ebx
	push dx 			;empilha valor do resto
	cmp eax, 0 			;verifica se quoc. é 0
	jne L10 			;se não é 0, continuasub rdi, RCX 			;decrementa precisao
	;agora, desemp valores e escreve parte int
L11:
	pop dx 			;desempilha valor
	add dl, '0' 			;transforma em caractere
	mov [RSI], dl 			;escreve caractere
	add RSI, 1 			;incrementa base
	sub RCX, 1 			;decrementa contador
	cmp RCX, 0 			;verifica pilha vazia
	jne L11 			;se não pilha vazia, loop
	mov dl, '.' 			;escreve ponto decimal
	mov [RSI], dl
	add RSI, 1 			;incrementa base
			;converte parte fracionaria que está em xmm0
L12:
	cmp rdi, 0 			;verifica precisao
	jle L13 			;terminou precisao ?
	mulss xmm0,xmm2 			;desloca para esquerda
	roundss xmm1,xmm0,0b0011 			;parte inteira xmm1
	subss xmm0,xmm1 			;atualiza xmm0
	cvtss2si rdx, xmm1			 ;convertido para int
	add dl, '0' 			;transforma em caractere
	mov [RSI], dl 			;escreve caractere
	add RSI, 1 			;incrementa base
	sub rdi, 1 			;decrementa precisao
	jmp L12
			; impressão
L13:
	mov dl, 0 			;fim string, opcional
	mov [RSI], dl 			;escreve caractere
	mov rdx, RSI ;calc tam str convertido
	mov rbx, M+0
	sub rdx, rbx 			;tam=RSI-M-buffer.end
	mov RSI, M+0 			; endereço do buffer
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	;  WRITE STRING

	mov RSI, M + 65818 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 ; contador de caracteres = 0

	; Loop para calcular tamanho da string
L14: ; Inicio do loop
	mov AL, [RSI] ; Leitura de caracter na posicao rax da memória
	cmp AL, 0 ; Verificação de flag de fim de string
	je L15 
 ; Se caracter lido = flag de fim de string finalizar loop
	add RDX, 1 ; Incrementando numero de caracteres
	add RSI, 1 ; Incrementando indice da string
	jmp L14  ; Se caracter lido != flag de fim de string continuar loop
L15: ; Fim do loop
	mov RSI, M + 65818 ; Copiando endereco inicial da string
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	;  WRITE STRING

	mov RSI, M + 65540 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 ; contador de caracteres = 0

	; Loop para calcular tamanho da string
L16: ; Inicio do loop
	mov AL, [RSI] ; Leitura de caracter na posicao rax da memória
	cmp AL, 0 ; Verificação de flag de fim de string
	je L17 
 ; Se caracter lido = flag de fim de string finalizar loop
	add RDX, 1 ; Incrementando numero de caracteres
	add RSI, 1 ; Incrementando indice da string
	jmp L16  ; Se caracter lido != flag de fim de string continuar loop
L17: ; Fim do loop
	mov RSI, M + 65540 ; Copiando endereco inicial da string
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	mov RSI, M + 0
	mov [RSI], byte 10 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX,1 			; ou buffer.tam
	mov RAX, 1 			; chamada para saida
	mov RDI, 1 			; saida para tela
	syscall
	mov EAX, 1			; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov EAX, [ M + 65536 ] 			; Move o valor de int 1 da memoria para o registrador EAX
	mov EBX, [ M + 0 ] 			; Move o valor de int 2 da memoria para o registrador EBX
	add EAX, EBX  			; int1 + int2
	mov [ M + 4 ], EAX 			; Salva resultado em temporario
	mov EAX, [ M +4 ] 			 ; Recupera valor do identificador da memoria
	mov [M + 65536 ] , EAX 			 ; Salva valor do registrador no endereco do ID
	jmp L8			; Realiza o loop do while
L7: 			 ; Fim do while 
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
