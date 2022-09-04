section .data                                               ; Sessão de dados
dados:                                                      ; Rótulo para demarcar o início da sessão de dados
    resb 0x4000                                             ; Reserva de temporários
    resq 1                                                  ; Espaço na memória para a variável double

section .text                                               ; Sessão de código
    global _start                                           ; Indicação do ponto inicial do programa

_start:                                                     ; Início do programa
; Executa a leitura de N caracteres e armazena-os no endereço especificado
    mov RAX, 0                                              ; Chamada de leitura
    mov RDI, 0                                              ; Ler da entrada padrão
    mov RSI, dados+0                                        ; Endereço inicial da string (315 bytes reservados)
    mov RDX, 315                                            ; Tamanho da string (incluindo o delimitador)
    syscall                                                 ; Chama o kernel

; Verifica se é necessário limpar o buffer do sistema
    add RAX, dados+0-1                                      ; RAX contém o número de caracteres lidos. Então, ao somar o endereço de memória - 1, obtém-se o endereço do último caractere (tipicamente a quebra de linha)
    mov RBX, RAX                                            ; Armazena este endereço em RBX, pois RAX será reutilizado posteriormente
    mov AL, [RBX]                                           ; Lê o caractere na última posição
    cmp AL, 0xa                                             ; Verifica se ele é uma quebra de linha (\n)
    je fimLeitura                                           ; Caso não for, o buffer do sistema ainda possui caracteres restantes

; Realiza a limpeza do buffer de leitura do sistema operacional 1 byte por vez
limpaBuffer:
    mov RAX, 0                                              ; Chamada de leitura
    mov RDI, 0                                              ; Ler da entrada padrão
    mov RSI, dados+0x13b                                    ; Primeira posição livre dos temporários (1 byte reservado)
    mov RDX, 1                                              ; Ler 1 byte
    syscall                                                 ; Chama o kernel

    mov AL, [dados+0x13b]                                   ; Carrega o caractere lido
    cmp AL, 0xa                                             ; Compara com a quebra de linha
    jne limpaBuffer                                         ; Caso seja diferente, ainda restam caracteres no buffer

fimLeitura:
; Preenche o caractere final da string com o delimitador
    mov AL, 0                                               ; Carrega o delimitador em al
    mov [RBX], AL                                           ; Armazena o delimitador na memória

; Conversão da parte inteira da string para número
    mov RAX, 10                                             ; Carrega a base 10
    cvtsi2sd XMM3, RAX                                      ; Converte para double, para ser utilizado como divisor
    subsd XMM0, XMM0                                        ; Zera o acumulador

    mov RAX, 0                                              ; RAX será utilizado como acumulador
    mov RBX, 0                                              ; RBX será utilizado para ler os caracteres
    mov RCX, 1                                              ; RCX será utilizado para armazenar o sinal
    mov RSI, dados+0                                        ; Endereço da string (na região dos temporários)
    mov BL, [RSI]                                           ; Carrega o primeiro caractere
    cmp BL, '-'                                             ; Verifica se é um sinal -
    jne loopConversao                                       ; Caso não seja, inicia a conversão

    mov RCX, -1                                             ; Armazena o sinal negativo em RCX
    inc RSI                                                 ; Incrementa o ponteiro da string

loopConversao:
    mov BL, [RSI]                                           ; Lê o próximo caractere da string
    cmp BL, 0                                               ; Verifica se ele é o fim de string
    je fimConversao                                         ; Caso seja, acabou a conversão

    cmp BL, '.'                                             ; Verifica se encontrou o final da parte inteira
    je conversaoFracionaria                                 ; Caso tenha encontrado, vai para a conversão da parte fracionária

    mov RDX, 10                                             ; Carrega a base
    imul RDX                                                ; Multiplica RDX:RAX por RDX (o valor em RDX será ignorado)
    sub BL, '0'                                             ; Converte o dígito para seu valor numérico
    add RAX, RBX                                            ; Adiciona o valor recuperado ao acumulador
    inc RSI                                                 ; Incrementa o ponteiro da string

    jmp loopConversao                                       ; Continua a conversão

; Conversão da parte fracionária para número
conversaoFracionaria:
    inc RSI                                                 ; Incrementa o ponteiro, para pular o .
    mov RDX, 0                                              ; Zera o RDX  

; Armazena os dígitos fracionários na pilha para depois recuperá-los na ordem reversa
loopConversaoFracionaria1:
    mov BL, [RSI]                                           ; Lê o próximo caractere da string
    cmp BL, 0                                               ; Verifica se ele é o fim de string
    je loopConversaoFracionaria2                            ; Caso seja, acabou a conversão

    sub BL, '0'                                             ; Converte o caractere para número            
    push BX                                                 ; Armazena o dígito na pilha        
    inc RDX                                                 ; Incrementa o contador        
    inc RSI                                                 ; Incrementa o ponteiro        

    jmp loopConversaoFracionaria1                           ; Continua o loop

; Recupera os dígitos da pilha e os transforma na parte fracionária do número; dividindo por 10 a cada novo dígito
loopConversaoFracionaria2:
    cmp RDX, 0                                              ; Verifica se recuperou todos os dígitos
    je fimConversao                                                   

    pop BX                                                  ; Carrega o próximo dígito  
    cvtsi2sd XMM1, RBX                                      ; Converte o número inteiro em ponto flutuante              
    addsd XMM0, XMM1                                        ; Adiciona o dígito ao acumulador
    divsd XMM0, XMM3                                        ; Faz um Shift à direita em base 10 (divide por 10)
    dec RDX                                                 ; Decrementa o contador

    jmp loopConversaoFracionaria2                           ; Continua o loop

; Junta as partes inteira e fracionária do número e o armazena na memória
fimConversao:
    cvtsi2sd XMM2, RAX                                      ; Converte a parte inteira em ponto flutuante
    addsd XMM0, XMM2                                        ; Soma a parte inteira com a fracionária
    cvtsi2sd XMM1, RCX                                      ; Converte o sinal em ponto flututante
    mulsd XMM0, XMM1                                        ; Multiplica o número pelo sinal
    movsd [dados+0x4000], XMM0                              ; Armazena o número na memória

; Fim do programa
    mov RAX, 60                                             ; Chamada de saída
    mov RDI, 0                                              ; Código de saida (0 = não houveram erros)
    syscall                                                 ; Chama o kernel
