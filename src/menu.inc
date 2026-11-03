
; ============================================================
; PANTALLA DE PRESENTACION
; ============================================================

PantallaPresentacion PROC

    call LimpiarPantalla
    call ConfigurarPaleta

    DIBUJA_TITULO textoTitulo, 48, 35, 51

    DIBUJA_TEXTO txtCreditos1, 60, 90, 51
    DIBUJA_TEXTO txtPulsaEnter, 52, 112, 51
    DIBUJA_TEXTO txtEquipo, 52, 132, 51

    ; Animacion de la gema
    mov animFrame, 0

PP_Animar:
    mov ax, 152
    push ax
    mov ax, 120
    push ax
    call BorrarRectangulo

    xor ax, ax
    mov al, animFrame
    and al, 1
    push ax

    mov ax, 152
    push ax
    mov ax, 120
    push ax
    call DibujarFicha

    call EsperarTick

    inc animFrame
    cmp animFrame, 6
    jb PP_Animar

    ; Esperar una tecla
    mov ah, 00h
    int 16h

    ret

PantallaPresentacion ENDP

; ============================================================
; DIBUJAR INDICADOR DE SELECCION
; ============================================================

CalcularPosicionIndicador PROC

    push bp
    mov bp, sp
    push bx

    xor ax, ax
    mov al, byte ptr [bp+4]

    mov bx, 20
    mul bx
    add ax, 75

    mov posicionIndicador, ax

    pop bx
    pop bp
    ret 2

CalcularPosicionIndicador ENDP

DibujarIndicador PROC

    push bp
    mov bp, sp
    push ax

    mov ax, [bp+6]
    mov posicionIndicadorX, ax

    push word ptr [bp+4]
    call CalcularPosicionIndicador

    DIBUJA_TEXTO textoIndicador, posicionIndicadorX, posicionIndicador, 51

    pop ax
    pop bp
    ret 4

DibujarIndicador ENDP

BorrarIndicador PROC

    push bp
    mov bp, sp
    push ax

    mov ax, [bp+6]
    mov posicionIndicadorX, ax

    push word ptr [bp+4]
    call CalcularPosicionIndicador

    DIBUJA_TEXTO textoIndicador, posicionIndicadorX, posicionIndicador, 0

    pop ax
    pop bp
    ret 4

BorrarIndicador ENDP


SeleccionarOpcion PROC

    push bp
    mov bp, sp

    push bx
    push cx
    push dx
    push si

    mov al, byte ptr [bp+4]
    mov cantidadOpciones, al

    mov al, byte ptr [bp+6]
    mov opcionTemporal, al

    ; Dibujar indicador inicial
    push word ptr [bp+8]
    xor ax, ax
    mov al, opcionTemporal
    push ax
    call DibujarIndicador

SO_Bucle:
    mov ah, 00h
    int 16h

    ; ESC: cancelar
    cmp al, 1Bh
    jne SO_NoEsc

    mov al, 0FFh
    jmp SO_Fin

SO_NoEsc:
    ; ENTER: aceptar
    cmp al, 0Dh
    jne SO_NoEnter

    mov al, opcionTemporal
    jmp SO_Fin

SO_NoEnter:
    ; Flecha arriba
    cmp ah, 48h
    jne SO_NoArriba

    ; Borrar indicador anterior
    push word ptr [bp+8]
    xor ax, ax
    mov al, opcionTemporal
    push ax
    call BorrarIndicador

    ; Si estaba en la primera opcion, ir a la ultima
    cmp opcionTemporal, 0
    jne SO_Subir

    mov al, cantidadOpciones
    mov opcionTemporal, al

SO_Subir:
    dec opcionTemporal
    jmp SO_Actualizar

SO_NoArriba:
    ; Flecha abajo
    cmp ah, 50h
    jne SO_Bucle

    ; Borrar indicador anterior
    push word ptr [bp+8]
    xor ax, ax
    mov al, opcionTemporal
    push ax
    call BorrarIndicador

    inc opcionTemporal

    mov al, opcionTemporal
    cmp al, cantidadOpciones
    jb SO_Actualizar

    mov opcionTemporal, 0

SO_Actualizar:
    ; Dibujar indicador en la nueva posicion
    push word ptr [bp+8]
    xor ax, ax
    mov al, opcionTemporal
    push ax
    call DibujarIndicador

    jmp SO_Bucle

SO_Fin:
    pop si
    pop dx
    pop cx
    pop bx
    pop bp

    ret 6

SeleccionarOpcion ENDP

; ============================================================
; MENU PRINCIPAL
; ============================================================

DibujarOpciones PROC

    DIBUJA_TEXTO textoJugar, 120, 75, 63
    DIBUJA_TEXTO textoDificultad, 120, 95, 63
    DIBUJA_TEXTO textoInstrucciones, 120, 115, 63
    DIBUJA_TEXTO textoCreditos, 120, 135, 63
    DIBUJA_TEXTO textoSalir, 120, 155, 63

    ret

DibujarOpciones ENDP

MostrarMenu PROC
MenuRedibujar:
    call LimpiarPantalla
    call ConfigurarPaleta

    DIBUJA_TITULO textoTitulo, 40, 28, 51
    call DibujarOpciones

    mov opcionSeleccionada, 0

    ; Parametros: X = 100, indice inicial = 0,
    ; cantidad de opciones = 5
    mov ax, 100
    push ax

    mov ax, 0
    push ax

    mov ax, 5
    push ax

    call SeleccionarOpcion

    ; ESC: salir del menu
    cmp al, 0FFh
    je MenuSalir

    mov opcionSeleccionada, al

    cmp al, 0
    je MenuJugar

    cmp al, 1
    je MenuDificultad

    cmp al, 2
    je MenuInstrucciones

    cmp al, 3
    je MenuCreditos

    cmp al, 4
    je MenuSalir

    jmp MenuRedibujar

MenuJugar:
    call IniciarPartida
    jmp MenuRedibujar

MenuDificultad:
    call PantallaDificultad
    jmp MenuRedibujar

MenuInstrucciones:
    call PantallaInstrucciones
    jmp MenuRedibujar

MenuCreditos:
    call PantallaCreditos
    jmp MenuRedibujar

MenuSalir:
    ret

MostrarMenu ENDP

; ============================================================
; PANTALLA DE SELECCION DE DIFICULTAD
; ============================================================

PantallaDificultad PROC

    call LimpiarPantalla
    call ConfigurarPaleta

    DIBUJA_TITULO textoDificultad, 35, 25, 51

    DIBUJA_TEXTO txtFacil, 24, 75, 51
    DIBUJA_TEXTO txtNormal, 24, 95, 51
    DIBUJA_TEXTO txtDificil, 24, 115, 51

    ; Parametros: X = 8, indice inicial = nivelMenu,
    ; cantidad de opciones = 3
    mov ax, 8
    push ax

    xor ax, ax
    mov al, nivelMenu
    push ax

    mov ax, 3
    push ax

    call SeleccionarOpcion

    ; ESC: regresar sin modificar la dificultad
    cmp al, 0FFh
    je PD_Salir

    mov nivelMenu, al
    mov dificultad, al

PD_Salir:
    ret

PantallaDificultad ENDP

; ============================================================
; PANTALLA DE INSTRUCCIONES
; ============================================================

PantallaInstrucciones PROC

    call LimpiarPantalla

    DIBUJA_TEXTO textoInstrucciones, 20, 10, 51

    DIBUJA_TEXTO txtInstruccion1, 20, 35, 51
    DIBUJA_TEXTO txtInstruccion2, 8, 53, 51
    DIBUJA_TEXTO txtInstruccion3, 8, 68, 51
    DIBUJA_TEXTO txtInstruccion4, 8, 83, 51
    DIBUJA_TEXTO txtInstruccion5, 8, 98, 51
    DIBUJA_TEXTO txtInstruccion6, 8, 113, 51
    DIBUJA_TEXTO txtInstruccion7, 8, 128, 51
    DIBUJA_TEXTO txtInstruccion8, 8, 143, 51
    DIBUJA_TEXTO txtInstruccion9, 8, 158, 51

    DIBUJA_TEXTO txtPulsaEnter, 30, 175, 51

    mov ah, 00h
    int 16h

    ret

PantallaInstrucciones ENDP

; ============================================================
; PANTALLA DE CREDITOS
; ============================================================

PantallaCreditos PROC

    call LimpiarPantalla

    DIBUJA_TITULO textoCreditos, 40, 35, 51

    DIBUJA_TEXTO txtCreditos1, 40, 85, 51
    DIBUJA_TEXTO txtCreditos2, 28, 105, 51
    DIBUJA_TEXTO txtCreditos3, 28, 125, 51
    DIBUJA_TEXTO txtPulsaEnter, 28, 175, 51

    mov ah, 00h
    int 16h

    ret

PantallaCreditos ENDP
