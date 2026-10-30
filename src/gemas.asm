.model small
.stack 100h
.data  
    modoOriginal db ?
    include gemas.inc

.code
main proc
    mov ax, @data
    mov ds, ax

    ; Guardar el modo de video original
    mov ah, 0Fh
    int 10h
    mov modoOriginal, al

    ; Activar modo grafico 13h
    mov ax, 0013h
    int 10h

    call ConfigurarPaleta

    ; Dibujar la gema azul en la posicion (50, 50)
    push OFFSET spriteAzul
    push 16             ; Alto
    push 16             ; Ancho
    push 50             ; Coordenada Y
    push 50             ; Coordenada X
    call DibujarSprite

    ; Esperar una tecla antes de continuar
    mov ah, 00h
    int 16h

    ; Restaurar el modo de video original
    mov ah, 00h
    mov al, modoOriginal
    int 10h 

    mov ax, 4C00h
    int 21h 
main ENDP

; ====================================================================

DibujarSprite PROC
    push bp
    mov bp, sp

    push ax
    push bx
    push cx
    push dx
    push si
    push di
    push es

    ; Configurar memoria de video
    mov ax, 0A000h
    mov es, ax

    ; Calcular la posicion inicial: Y * 320 + X
    mov ax, [bp + 6]
    mov bx, 320
    mul bx
    add ax, [bp + 4]
    mov di, ax

    ; Cargar dimensiones y direccion del sprite
    mov bx, [bp + 8]
    mov dx, [bp + 10]
    mov si, [bp + 12]

FilaSprite:
    push dx
    mov cx, bx

PixelSprite:
    lodsb

    ; El color 0 es transparente
    or al, al
    jz PixelTransparente

    mov es:[di], al

PixelTransparente:
    inc di
    loop PixelSprite

    ; Avanzar hasta la misma columna de la siguiente fila
    add di, 320
    sub di, bx

    pop dx
    dec dx
    jnz FilaSprite

    pop es
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    ret 10
DibujarSprite ENDP

; ====================================================================

ConfigurarPaleta PROC
    push ax
    push cx
    push si
    push dx

    mov dx, 03C8h
    mov al, 1       ; Empezar en el indice 1
    out dx, al

    inc dx
    mov si, OFFSET paletaGemas
    mov cx, 150

CargarColorPaleta:
    lodsb
    out dx, al
    loop CargarColorPaleta

    pop dx
    pop si
    pop cx
    pop ax
    ret

ConfigurarPaleta ENDP

; ====================================================================

end main
