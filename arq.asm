section .data                 ; Sessão de dados
M:                            ; Rotulo para demarcar o
                              ; inicio da sessao de dados
	resb 0x10000              ; Reserva de temporarios
   ; ***Definicoes de variaveis e constantes
	resb 100h			; Declaracao Var String em [65536]
	db "123",0			; Declaracao String EXP em [65792]
section .text                 ; Sessao de codigo
global _start                 ; Ponto inicial do programa
_start:                       ; Inicio do programa
   ; ***Comandos
	mov RSI, M + 65536 ; Copiando endereço da string para um registrador de índice
	mov RDI, M + 65792 ; Copiando endereço da string para um registrador de índice
	; Loop para calcular tamanho da string
L1: ; Inicio do loop
	mov AL, [RDI] ; Leitura de caracter na posicao RSI da memória
	mov [RSI], AL 			; Salva caracter no identificador
	cmp AL, 0 ; Verificação de flag de fim de string
	je L2 
 ; Se caracter lido = flag de fim de string finalizar loop
	add RDI, 1 ; Incrementando indice da string1 
	add RSI, 1 ; Incrementando indice da string2 
	jmp L1  ; Se caracter lido != flag de fim de string continuar loop
L2: ; Fim do loop
	mov EAX, 4			; Move Inteiro imediato para registrador
	mov [ M + 0 ], EAX 			; Move registrador para posicao atual de memoria em [0]
	mov EBX, [ M +0 ] 			 ; Recupera valor de indice da string da memoria
	add EBX, M + 65536 			 ; Move endereco de memoria da string para registrador RSI
	mov EAX, [EBX] 			 ; Escreve valor de caracter para o registrador AL
	mov [ M + 4 ], EAX 			; Salva character em temporario
	;  WRITE CHAR

	mov RSI, M + 4 ; Copiando endereço do character para um registrador de índice
	mov RDX, 1 ; Quantidade de caracteres imprimiveis 

	mov RAX, 1 ; Chamada para saída
	mov RDI, 1 ; Chamada para tela
	syscall
; Halt
mov rax, 60                   ; Chamada de saida
mov rdi, 0                    ; Codigo de saida sem erros
syscall                       ; Chama o kernel
