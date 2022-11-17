section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resd 1			; Declaracao Var Inteiro em [65536]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov RSI, M+0 ; Salva o endereco do buffer
	mov RDX, 100h ; Tamanho do buffer
	mov RAX, 0 ; Chamada para leitura
	mov RDI, 0 ; Leitura do teclado
	syscall

	mov EAX, 0 			;acumulador
	mov EBX, 0 			;caractere
	mov ECX, 10 			;base 10
	mov DX, 1 			;sinal
	mov RSI, M + 0 			;end. buffer
	mov BL, [RSI] 			;carrega caractere
	cmp BL, '-' 			;sinal - ?
	jne L1 			;se dif -, salta
	mov DX, -1 			;senão, armazena -
	add RSI, 1 			;inc. ponteiro string
	mov BL, [RSI] 			;carrega caractere
L1:
	push DX 			;empilha sinal
	mov EDX, 0 			;reg. multiplicação
L2:
	cmp BL, 0Ah 			;verifica fim string
	je L3 			;salta se fim string
	imul ECX 			;mult. EAX por 10
	sub BL, '0' 			;converte caractere
	add EAX, EBX 			;soma valor caractere
	add RSI, 1 			;incrementa base
	mov BL, [RSI] 			;carrega caractere
	jmp L2 			;loop
L3:
	pop CX 			;desempilha sinal
	cmp CX, 0
	jg L4
	neg EAX 			;mult. sinal
L4:
	mov [M+65536], EAX ; Carrega o valor para o indentificador
	mov EAX, [ M + 65536 ] 			; Move o valor de int 1 da memoria para o registrador EAX
	mov EBX, [ M + 65536 ] 			; Move o valor de int 2 da memoria para o registrador EBX
	add EAX, EBX  			; int1 + int2
	mov [ M + 0 ], EAX 			; Salva resultado em temporario
	mov eax, [ M + 0] 			; inteiro a ser ;convertido
	mov rsi, M + 4 			 ; end. string ou temp.
	mov rcx, 0 			 ; contador pilha
	mov rdi, 0 			 ; tam. string convertido
	cmp eax, 0 			 ; verifica sinal
	jge L5 			 ; salta se número positivo
	mov bl, '-' 			 ; senão, escreve sinal –
	mov [rsi], bl
	add rsi, 1 			 ; incrementa índice
	add rdi, 1 			 ; incrementa tamanho
	neg eax 			;toma módulo do número
	L5:
	mov ebx, 10 			;divisor
	L6:
	add rcx, 1 			;incrementa contador
	cdq 			;estende edx:eax p/ div.
	idiv ebx 			;divide edx;eax por ebx
	push dx 			;empilha valor do resto
	cmp eax, 0 			;verifica se quoc. é 0
	jne L6 			;se não é 0, continua
	add rdi,rcx 			;atualiza tam. string
	;agora, desemp. os valores e escreve o string
	L7:
	pop dx 			 ;desempilha valor
	add dl, '0' 			 ;transforma em caractere
	mov [rsi], dl 			 ;escreve caractere
	add rsi, 1 			 ;incrementa base
	sub rcx, 1 			 ;decrementa contador
	cmp rcx, 0 			 ;verifica pilha vazia
	jne L7			 ;se não pilha vazia, loop
	;  WRITE STRING

	mov RSI, M + 4 ; Copiando endereço da string para um registrador de índice
	mov RDX, 0 ; contador de caracteres = 0

	; Loop para calcular tamanho da string
L8: ; Inicio do loop
	mov AL, [RSI] ; Leitura de caracter na posicao rax da memória
	cmp AL, 0 ; Verificação de flag de fim de string
	je L9 
 ; Se caracter lido = flag de fim de string finalizar loop
	add RDX, 1 ; Incrementando numero de caracteres
	add RSI, 1 ; Incrementando indice da string
	jmp L8  ; Se caracter lido != flag de fim de string continuar loop
L9: ; Fim do loop
	mov RSI, M + 4 ; Copiando endereco inicial da string
	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
	mov RSI, M + 8
	mov [RSI], byte 10 ; Move caracter quebra de linha para o endereço armazenado em RSI
	mov RDX,1 			; ou buffer.tam
	mov RAX, 1 			; chamada para saida
	mov RDI, 1 			; saida para tela
	syscall
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
