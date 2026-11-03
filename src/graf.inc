; ============================================================

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
    mov cx, 153

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

; ============================================================
; MOSTRAR TEXTO
; ============================================================

DIBUJA_TEXTO MACRO cadenaParam, xParam, yParam, colorParam
    mov ax, colorParam
    push ax
    mov ax, yParam
    push ax
    mov ax, xParam
    push ax
    mov ax, OFFSET cadenaParam
    push ax
    call MostrarTexto
ENDM

; ============================================================

MostrarTexto PROC
    push bp
    mov bp, sp

    push ax
    push si

    mov si, [bp+4]
    mov ax, [bp+6]
    mov textoX, ax
    mov ax, [bp+8]
    mov textoY, ax
    mov ax, [bp+10]
    mov textoColor, al

    mov ax, 0A000h
    mov es, ax

MT_Siguiente:
    lodsb

    cmp al, '$'
    jne MT_Validar
    jmp MT_Fin

MT_Validar:
    cmp al, 32
    jae MT_MinimoCorrecto
    jmp MT_Desconocido

MT_MinimoCorrecto:
    cmp al, 126
    jbe MT_ObtenerPatron
    jmp MT_Desconocido

MT_ObtenerPatron:
    sub al, 32
    xor ah, ah
    mov bx, 8
    mul bx
    add ax, OFFSET tablaLetrasGeneral
    mov textoPtr, ax

    call DibujarCaracter
    add word ptr textoX, 8
    jmp MT_Siguiente

MT_Desconocido:
    add word ptr textoX, 8
    jmp MT_Siguiente

MT_Fin:
    pop si
    pop ax
    pop bp
    ret 8
MostrarTexto ENDP


; ============================================================

DibujarCaracter PROC
    push ax
    push bx
    push cx
    push dx
    push di

    mov textoFila, 0

DC_Fila:
    mov ax, textoY
    mov bx, 320
    mul bx
    add ax, textoX
    mov di, ax

    xor ax, ax
    mov al, textoFila
    mov bx, 320
    mul bx
    add di, ax

    mov bx, textoPtr
    xor ax, ax
    mov al, textoFila
    add bx, ax
    mov dl, [bx]

    mov bl, 80h
    mov cx, 8

DC_Columna:
    test dl, bl
    jz DC_SiguientePixel

    mov al, textoColor
    mov es:[di], al

DC_SiguientePixel:
    inc di
    shr bl, 1
    loop DC_Columna

    inc textoFila
    cmp textoFila, 8
    jb DC_Fila

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret
DibujarCaracter ENDP

; ============================================================
; MOSTRAR TITULOS 
; ============================================================

DIBUJA_TITULO MACRO cadenaParam, xParam, yParam, colorParam
    mov ax, colorParam
    push ax
    mov ax, yParam
    push ax
    mov ax, xParam
    push ax
    mov ax, OFFSET cadenaParam
    push ax
    call MostrarTitulo
ENDM

; ============================================================

MostrarTitulo PROC
    push bp
    mov bp, sp

    push ax
    push si

    mov si, [bp+4]
    mov ax, [bp+6]
    mov textoX, ax
    mov ax, [bp+8]
    mov textoY, ax
    mov ax, [bp+10]
    mov textoColor, al

    mov ax, 0A000h
    mov es, ax

MTI_Siguiente:
    lodsb

    cmp al, '$'
    jne MTI_Validar
    jmp MTI_Fin

MTI_Validar:
    cmp al, 32
    jae MTI_MinimoCorrecto
    jmp MTI_Desconocido

MTI_MinimoCorrecto:
    cmp al, 126
    jbe MTI_ObtenerPatron
    jmp MTI_Desconocido

MTI_ObtenerPatron:
    sub al, 32
    xor ah, ah
    mov bx, 8
    mul bx
    add ax, OFFSET tablaLetrasGeneral
    mov textoPtr, ax

    call DibujarCaracterGrande
    add word ptr textoX, 16
    jmp MTI_Siguiente

MTI_Desconocido:
    add word ptr textoX, 16
    jmp MTI_Siguiente

MTI_Fin:
    pop si
    pop ax
    pop bp
    ret 8
MostrarTitulo ENDP

; ============================================================

DibujarCaracterGrande PROC
    push ax
    push bx
    push cx
    push dx
    push di

    mov textoFila, 0

DCG_Fila:
    mov ax, textoY
    mov bx, 320
    mul bx
    add ax, textoX
    mov di, ax

    xor ax, ax
    mov al, textoFila
    mov bx, 640
    mul bx
    add di, ax

    mov bx, textoPtr
    xor ax, ax
    mov al, textoFila
    add bx, ax
    mov dl, [bx]

    mov bl, 80h
    mov cx, 8

DCG_Columna:
    test dl, bl
    jz DCG_SiguientePixel

    mov al, textoColor
    mov es:[di], al
    mov es:[di+1], al
    mov es:[di+320], al
    mov es:[di+321], al

DCG_SiguientePixel:
    add di, 2
    shr bl, 1
    loop DCG_Columna

    inc textoFila
    cmp textoFila, 8
    jb DCG_Fila

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret
DibujarCaracterGrande ENDP

; ============================================================
; MOSTRAR SPRITES
; ============================================================

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

; ============================================================
; DIBUJAR OCHO SPRITES EN UNA FILA
; ============================================================

DibujarOchoSprites PROC

    push bp
    mov bp, sp

    push ax
    push bx
    push cx
    push dx
    push si
    push di

    ; Obtener parametros
    mov di, [bp+4]     ; Coordenada X inicial
    mov dx, [bp+6]     ; Coordenada Y
    mov bx, [bp+8]     ; Separacion entre fichas

    xor si, si         ; Indice del sprite

DOF_Bucle:

    cmp si, 8
    jae DOF_Fin

    ; Dibujar ficha
    push si
    push di
    push dx
    call DibujarFicha

    ; Avanzar a la siguiente posicion
    add di, bx
    inc si

    jmp DOF_Bucle

DOF_Fin:

    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    ret 6

DibujarOchoSprites ENDP

; ============================================================

DibujarBorde PROC

    push bp
    mov bp, sp

    push ax
    push bx
    push cx
    push dx
    push di
    push es

    mov ax, 0A000h
    mov es, ax

    ; Borde superior: Y - 1, X - 1
    mov ax, [bp+6]
    dec ax
    mov bx, 320
    mul bx

    mov bx, [bp+4]
    dec bx
    add ax, bx
    mov di, ax

    mov cx, 18

BordeSuperior:
    mov al, colorBorde
    mov es:[di], al
    inc di
    loop BordeSuperior

    ; Borde inferior: Y + 16, X - 1
    mov ax, [bp+6]
    add ax, 16
    mov bx, 320
    mul bx

    mov bx, [bp+4]
    dec bx
    add ax, bx
    mov di, ax

    mov cx, 18

BordeInferior:
    mov al, colorBorde
    mov es:[di], al
    inc di
    loop BordeInferior

    ; Borde izquierdo: Y, X - 1
    mov ax, [bp+6]
    mov bx, 320
    mul bx

    mov bx, [bp+4]
    dec bx
    add ax, bx
    mov di, ax

    mov cx, 16

BordeIzquierdo:
    mov al, colorBorde
    mov es:[di], al
    add di, 320
    loop BordeIzquierdo

    ; Borde derecho: Y, X + 16
    mov ax, [bp+6]
    mov bx, 320
    mul bx

    mov bx, [bp+4]
    add bx, 16
    add ax, bx
    mov di, ax

    mov cx, 16

BordeDerecho:
    mov al, colorBorde
    mov es:[di], al
    add di, 320
    loop BordeDerecho

    pop es
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    ret 4

DibujarBorde ENDP

; ============================================================
; BORRAR BORDE DE SELECCION ANTERIOR
; Parametros: X, Y (mismo orden que DibujarBorde)
; Restaura el color de fondo sin borrar la gema.
; ============================================================

BorrarBorde PROC

    push bp
    mov bp, sp
    push ax

    ; Guardar el color actual y dibujar el borde con color 0.
    mov ah, colorBorde
    mov colorBorde, 0

    push word ptr [bp+6] ; Y
    push word ptr [bp+4] ; X
    call DibujarBorde

    mov colorBorde, ah

    pop ax
    pop bp
    ret 4

BorrarBorde ENDP

; ============================================================
; BORRAR GEMA DE UNA CASILLA (16 x 16)
; ============================================================

BorrarGema PROC

    push bp
    mov bp, sp
    push ax

    ; BorrarRectangulo recibe Y en [bp+4] y X en [bp+6].
    ; Por eso se apilan primero X y luego Y.
    push word ptr [bp+4] ; X
    push word ptr [bp+6] ; Y
    call BorrarRectangulo

    pop ax
    pop bp
    ret 4

BorrarGema ENDP

; ============================================================
; LIMPIAR PANTALLA VGA
; ============================================================

LimpiarPantalla PROC
    push ax
    push cx
    push di
    push es

    mov ax, 0A000h
    mov es, ax

    xor di, di
    xor ax, ax
    mov cx, 32000

    cld
    rep stosw

    pop es
    pop di
    pop cx
    pop ax
    ret
LimpiarPantalla ENDP

; ============================================================
; BORRAR RECTANGULO DE 16 X 16 PIXELES
; ============================================================

BorrarRectangulo PROC
    push bp
    mov bp, sp

    push ax
    push bx
    push cx
    push dx
    push di
    push es

    mov ax, 0A000h
    mov es, ax

    ; Calcular Y * 320 + X.
    mov ax, [bp+4]
    mov bx, 320
    mul bx
    add ax, [bp+6]
    mov di, ax

    mov dx, 16

BR_Fila:
    push dx

    mov cx, 16
    xor al, al
    cld
    rep stosb

    add di, 304

    pop dx
    dec dx
    jnz BR_Fila

    pop es
    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    pop bp

    ret 4
BorrarRectangulo ENDP

; ============================================================
; BORRAR BARRA INFERIOR
; ============================================================

BorrarBarra PROC
    push ax
    push cx
    push di
    push es

    mov ax, 0A000h
    mov es, ax

    mov di, 58880
    mov cx, 2560
    xor ax, ax

    cld
    rep stosw

    pop es
    pop di
    pop cx
    pop ax
    ret
BorrarBarra ENDP

; ============================================================
; MOSTRAR NUMERO DE DOS DIGITOS
; ============================================================

MostrarDigito PROC
    push bp
    mov bp, sp

    push ax
    push bx
    push dx

    ; Separar decenas y unidades.
    mov ax, [bp+8]
    xor dx, dx
    mov bx, 10
    div bx

    ; Guardar decenas.
    add al, '0'
    mov numeroBuffer, al

    ; Guardar unidades.
    mov al, dl
    add al, '0'
    mov numeroBuffer+1, al

    ; Asegurar el final de la cadena.
    mov numeroBuffer+2, '$'

    ; Mostrar el resultado.
    mov ax, 51
    push ax

    mov ax, [bp+4]
    push ax

    mov ax, [bp+6]
    push ax

    mov ax, OFFSET numeroBuffer
    push ax

    call MostrarTexto

    pop dx
    pop bx
    pop ax
    pop bp

    ret 6
MostrarDigito ENDP