; ============================================================
; CRYSTAL QUEST
; JUEGO PRINCIPAL
; ============================================================


; ============================================================
; 1. CONFIGURACION DE DIFICULTAD
; ============================================================

ConfigurarDificultad PROC

    cmp dificultad, 0
    jne CD_Normal

    mov cantidadGemas, 4
    mov cantidadIntentos, 12
    mov cantidadColores, 6
    ret

CD_Normal:
    cmp dificultad, 1
    jne CD_Dificil

    mov cantidadGemas, 4
    mov cantidadIntentos, 10
    mov cantidadColores, 6
    ret

CD_Dificil:
    mov cantidadGemas, 5
    mov cantidadIntentos, 10
    mov cantidadColores, 8
    ret

ConfigurarDificultad ENDP


; ============================================================
; 2. GENERAR COMBINACION SECRETA
; ============================================================

GenerarCombinacion PROC

    push ax
    push bx
    push cx
    push dx
    push si
    push di

GC_Nueva:

    ; Obtener semilla del reloj.
    mov ah, 00h
    int 1Ah

    mov ax, dx
    xor ax, cx
    add ax, contadorPartidas
    inc contadorPartidas

    mov semilla, ax

    xor si, si

GC_Generar:

    xor ax, ax
    mov al, cantidadGemas
    cmp si, ax
    jae GC_ComprobarAnterior

    ; Generar siguiente numero pseudoaleatorio.
    mov ax, semilla
    mov bx, 25173
    mul bx
    add ax, 13849
    mov semilla, ax

    ; Obtener color entre 0 y cantidadColores-1.
    xor dx, dx
    xor bx, bx
    mov bl, cantidadColores
    div bx

    mov al, dl

    ; En facil no se permiten colores repetidos.
    cmp dificultad, 0
    jne GC_Guardar

    xor di, di

GC_Revisar:

    cmp di, si
    jae GC_Guardar

    cmp combinacionSecreta[di], al
    jne GC_Siguiente

    ; Si se repite, avanzar al siguiente color.
    inc al
    cmp al, cantidadColores
    jb GC_ReiniciarRevision

    xor al, al

GC_ReiniciarRevision:
    xor di, di
    jmp GC_Revisar

GC_Siguiente:
    inc di
    jmp GC_Revisar

GC_Guardar:

    mov combinacionSecreta[si], al

    inc si
    jmp GC_Generar


; Evitar repetir exactamente la combinacion anterior.
GC_ComprobarAnterior:

    cmp hayCombinacionAnterior, 0
    je GC_GuardarAnterior

    xor si, si

GC_Comparar:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae GC_Repetida

    mov al, combinacionSecreta[si]
    cmp al, ultimaCombinacion[si]
    jne GC_GuardarAnterior

    inc si
    jmp GC_Comparar

GC_Repetida:
    jmp GC_Nueva


; Guardar la combinacion generada.
GC_GuardarAnterior:

    xor si, si

GC_Copiar:

    cmp si, 5
    jae GC_Fin

    mov al, combinacionSecreta[si]
    mov ultimaCombinacion[si], al

    inc si
    jmp GC_Copiar

GC_Fin:

    mov hayCombinacionAnterior, 1

    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax

    ret

GenerarCombinacion ENDP


; ============================================================
; 3. LIMPIAR DATOS DE LA PARTIDA
; ============================================================

LimpiarDatosPartida PROC

    push ax
    push si

    mov intentosUsados, 0
    mov posicionSeleccionada, 0
    mov colorActual, 0

    mov bordeExiste, 0
    mov colorBorde, 51

    xor si, si

LDP_Jugador:

    cmp si, 5
    jae LDP_Historial

    mov combinacionJugador[si], 0
    mov posicionesLlenas[si], 0

    mov marcaResultado[si], 0
    mov marcaSecreto[si], 0
    mov marcaJugador[si], 0

    inc si
    jmp LDP_Jugador


LDP_Historial:

    xor si, si

LDP_Gemas:

    cmp si, 60
    jae LDP_Resultados

    mov historialGemas[si], 0
    mov historialMarcas[si], 0

    inc si
    jmp LDP_Gemas


LDP_Resultados:

    xor si, si

LDP_LoopResultados:

    cmp si, 12
    jae LDP_Fin

    mov historialT[si], 0
    mov historialV[si], 0

    inc si
    jmp LDP_LoopResultados


LDP_Fin:

    pop si
    pop ax

    ret

LimpiarDatosPartida ENDP


; ============================================================
; 4. INICIAR PARTIDA
; ============================================================

IniciarPartida PROC

IP_NuevaPartida:

    call ConfigurarDificultad
    call LimpiarDatosPartida
    call GenerarCombinacion

    mov modoPrueba, 0

    call LimpiarPantalla
    call ConfigurarPaleta
    call DibujarTablero

    call ControlarGemas

    ; AL = 1 reinicia; AL = 2 regresa al menu.
    cmp al, 1
    je IP_NuevaPartida

    ret

IniciarPartida ENDP


; ============================================================
; 5. DIBUJAR UNA GEMA
; Parametros: color, X, Y
; ============================================================

DibujarFicha PROC

    push bp
    mov bp, sp

    push ax
    push dx

    mov ax, [bp+8]

    cmp al, 0
    jne DF_Naranja

    mov dx, OFFSET spriteAzul
    jmp DF_Dibujar

DF_Naranja:

    cmp al, 1
    jne DF_Turquesa

    mov dx, OFFSET spriteNaranja
    jmp DF_Dibujar

DF_Turquesa:

    cmp al, 2
    jne DF_Verde

    mov dx, OFFSET spriteTurquesa
    jmp DF_Dibujar

DF_Verde:

    cmp al, 3
    jne DF_Rosa

    mov dx, OFFSET spriteVerde
    jmp DF_Dibujar

DF_Rosa:

    cmp al, 4
    jne DF_Morada

    mov dx, OFFSET spriteRosa
    jmp DF_Dibujar

DF_Morada:

    cmp al, 5
    jne DF_Roja

    mov dx, OFFSET spriteMorada
    jmp DF_Dibujar

DF_Roja:

    cmp al, 6
    jne DF_Amarilla

    mov dx, OFFSET spriteRoja
    jmp DF_Dibujar

DF_Amarilla:

    mov dx, OFFSET spriteAmarilla


DF_Dibujar:

    push dx

    mov ax, 16
    push ax
    push ax

    mov ax, [bp+4]
    push ax

    mov ax, [bp+6]
    push ax

    call DibujarSprite

    pop dx
    pop ax
    pop bp

    ret 6

DibujarFicha ENDP


; ============================================================
; 6. DIBUJAR ENCABEZADO
; ============================================================

DibujarEncabezado PROC

    DIBUJA_TEXTO textoTitulo, 4, 4, 51
    DIBUJA_TEXTO txtActual, 190, 4, 51

    ret

DibujarEncabezado ENDP


; ============================================================
; 7. DIBUJAR FILA DEL HISTORIAL
; ============================================================

DibujarFilaHistorial PROC

    push bp
    mov bp, sp

    push ax
    push bx
    push si

    ; Indice inicial de las gemas.
    mov ax, [bp+4]
    mov bx, 5
    mul bx

    mov indiceTemp, ax

    ; Coordenada vertical.
    mov ax, [bp+4]
    cmp ax, 10
    jb DFH_FilaVisible
    sub ax, 10

DFH_FilaVisible:
    xor bx, bx
    mov bl, separacionY
    mul bx

    xor bx, bx
    mov bl, tableroY
    add ax, bx
    mov coordYTemp, ax

    xor si, si

DFH_Gemas:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae DFH_Resultados

    mov bx, indiceTemp
    add bx, si

    ; Dibujar gema del historial.
    xor ax, ax
    mov al, historialGemas[bx]

    push ax

    mov ax, si
    xor bx, bx
    mov bl, separacionX
    mul bx

    xor bx, bx
    mov bl, tableroX
    add ax, bx
    push ax

    push coordYTemp

    call DibujarFicha

    ; Leer marca individual.
    mov bx, indiceTemp
    add bx, si

    mov al, historialMarcas[bx]

    cmp al, 0
    je DFH_Siguiente

    ; Toro: verde.
    mov colorBorde, 21

    ; Vaca: amarillo.
    cmp al, 2
    jne DFH_DibujarBorde

    mov colorBorde, 49

DFH_DibujarBorde:

    push coordYTemp

    mov ax, si
    xor bx, bx
    mov bl, separacionX
    mul bx

    xor bx, bx
    mov bl, tableroX
    add ax, bx
    push ax

    call DibujarBorde

DFH_Siguiente:

    inc si
    jmp DFH_Gemas


DFH_Resultados:

    ; Mostrar toros.
    DIBUJA_TEXTO txtToros, 112, coordYTemp, 51

    xor bx, bx
    mov bl, byte ptr [bp+4]

    xor ax, ax
    mov al, historialT[bx]

    push ax

    mov ax, 128
    push ax

    push coordYTemp

    call MostrarDigito

    ; Mostrar vacas.
    DIBUJA_TEXTO txtVacas, 145, coordYTemp, 51

    xor bx, bx
    mov bl, byte ptr [bp+4]

    xor ax, ax
    mov al, historialV[bx]

    push ax

    mov ax, 161
    push ax

    push coordYTemp

    call MostrarDigito

    pop si
    pop bx
    pop ax
    pop bp

    ret 2

DibujarFilaHistorial ENDP


; ============================================================
; 8. DIBUJAR HISTORIAL COMPLETO
; ============================================================

DibujarHistorial PROC

    push ax
    push bx

    xor bx, bx
    mov al, intentosUsados
    cmp al, 10
    jb DH_Siguiente
    mov bx, 10

DH_Siguiente:

    xor ax, ax
    mov al, intentosUsados
    cmp bx, ax
    jae DH_Fin

    push bx
    call DibujarFilaHistorial
    inc bx
    jmp DH_Siguiente

DH_Fin:

    pop bx
    pop ax
    ret

DibujarHistorial ENDP


; ============================================================
; 9. DIBUJAR FILA ACTUAL
; ============================================================

DibujarFilaActual PROC

    push ax
    push bx
    push si

    ; Calcular posicion vertical.
    xor ax, ax
    mov al, intentosUsados
    cmp al, 10
    jb DFA_FilaVisible
    sub al, 10

DFA_FilaVisible:
    xor bx, bx
    mov bl, separacionY
    mul bx

    xor bx, bx
    mov bl, tableroY
    add ax, bx
    mov coordYTemp, ax

    xor si, si

DFA_Gemas:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae DFA_Borde

    ; Dibujar solamente posiciones llenas.
    cmp posicionesLlenas[si], 1
    jne DFA_Siguiente

    xor ax, ax
    mov al, combinacionJugador[si]

    push ax

    mov ax, si
    xor bx, bx
    mov bl, separacionX
    mul bx

    xor bx, bx
    mov bl, tableroX
    add ax, bx
    push ax

    push coordYTemp

    call DibujarFicha

DFA_Siguiente:

    inc si
    jmp DFA_Gemas


DFA_Borde:

    ; Dibujar borde de seleccion.
    mov colorBorde, 51

    push coordYTemp

    xor ax, ax
    xor ax, ax
    mov al, posicionSeleccionada
    xor bx, bx
    mov bl, separacionX
    mul bx

    xor bx, bx
    mov bl, tableroX
    add ax, bx
    push ax

    call DibujarBorde

    mov al, posicionSeleccionada
    mov posicionBordeAnterior, al

    mov al, intentosUsados
    mov filaBordeAnterior, al

    mov bordeExiste, 1

    pop si
    pop bx
    pop ax

    ret

DibujarFilaActual ENDP


; ============================================================
; 10. PANEL DE INFORMACION
; ============================================================

DibujarPanelInformacion PROC

    push ax
    push bx

    DIBUJA_TEXTO txtNivel, 210, 25, 51

    cmp dificultad, 0
    jne DPI_Normal

    DIBUJA_TEXTO txtNivelFacil, 210, 40, 51
    jmp DPI_Restantes

DPI_Normal:

    cmp dificultad, 1
    jne DPI_Dificil

    DIBUJA_TEXTO txtNivelNormal, 210, 40, 51
    jmp DPI_Restantes

DPI_Dificil:

    DIBUJA_TEXTO txtNivelDificil, 210, 40, 51


DPI_Restantes:

    DIBUJA_TEXTO txtRestantes, 210, 65, 51

    xor ax, ax
    mov al, cantidadIntentos

    xor bx, bx
    mov bl, intentosUsados

    sub ax, bx

    push ax

    mov ax, 300
    push ax

    mov ax, 65
    push ax

    call MostrarDigito

    DIBUJA_TEXTO txtAyuda1, 210, 95, 51
    DIBUJA_TEXTO txtAyuda2, 210, 112, 51
    DIBUJA_TEXTO txtAyuda3, 210, 129, 51
    DIBUJA_TEXTO txtAyuda4, 210, 146, 51

    pop bx
    pop ax

    ret

DibujarPanelInformacion ENDP


; ============================================================
; 11. DIBUJAR TABLERO
; ============================================================

DibujarTablero PROC

    push ax

    ; La limpieza completa se reserva para cambios de pantalla/turno.
    call LimpiarPantalla

    call DibujarEncabezado
    call DibujarHistorial
    call DibujarFilaActual
    call DibujarPanelInformacion

    DIBUJA_TEXTO txtInstruccion4, 4, 190, 51

    pop ax

    ret

DibujarTablero ENDP


; ============================================================
; 12. EVALUAR INTENTO
; ============================================================

EvaluarIntento PROC

    push ax
    push si
    push di

    mov toros, 0
    mov vacas, 0

    ; Limpiar marcas anteriores.
    xor si, si

EI_Limpiar:

    cmp si, 5
    jae EI_TorosInicio

    mov marcaSecreto[si], 0
    mov marcaJugador[si], 0
    mov marcaResultado[si], 0

    inc si
    jmp EI_Limpiar


; Buscar toros.
EI_TorosInicio:

    xor si, si

EI_Toros:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae EI_VacasInicio

    mov al, combinacionSecreta[si]

    cmp al, combinacionJugador[si]
    jne EI_SiguienteToro

    inc toros

    mov marcaSecreto[si], 1
    mov marcaJugador[si], 1
    mov marcaResultado[si], 1

EI_SiguienteToro:

    inc si
    jmp EI_Toros


; Buscar vacas.
EI_VacasInicio:

    xor si, si

EI_BuscarVaca:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae EI_Fin

    cmp marcaJugador[si], 1
    je EI_SiguienteJugador

    xor di, di

EI_BuscarSecreto:

    xor ax, ax
    mov al, cantidadGemas

    cmp di, ax
    jae EI_SiguienteJugador

    cmp marcaSecreto[di], 1
    je EI_SiguienteSecreto

    mov al, combinacionJugador[si]

    cmp al, combinacionSecreta[di]
    jne EI_SiguienteSecreto

    inc vacas

    mov marcaJugador[si], 1
    mov marcaSecreto[di], 1
    mov marcaResultado[si], 2

    jmp EI_SiguienteJugador

EI_SiguienteSecreto:

    inc di
    jmp EI_BuscarSecreto

EI_SiguienteJugador:

    inc si
    jmp EI_BuscarVaca

EI_Fin:

    pop di
    pop si
    pop ax

    ret

EvaluarIntento ENDP


; ============================================================
; 13. GUARDAR INTENTO
; ============================================================

GuardarIntento PROC

    push ax
    push bx
    push si
    push di

    ; Calcular posicion del historial.
    xor ax, ax
    mov al, intentosUsados

    mov bx, 5
    mul bx

    mov di, ax

    xor si, si

GI_Copiar:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae GI_Resultados

    mov al, combinacionJugador[si]
    mov historialGemas[di], al

    mov al, marcaResultado[si]
    mov historialMarcas[di], al

    inc si
    inc di

    jmp GI_Copiar


GI_Resultados:

    xor bx, bx
    mov bl, intentosUsados

    mov al, toros
    mov historialT[bx], al

    mov al, vacas
    mov historialV[bx], al

    inc intentosUsados

    pop di
    pop si
    pop bx
    pop ax

    ret

GuardarIntento ENDP


; ============================================================
; ACTUALIZAR SOLO LA GEMA Y EL BORDE DE SELECCION
; Evita redibujar textos, historial y panel al moverse.
; ============================================================

ActualizarSeleccion PROC

    push ax
    push bx

    ; Quitar solamente el borde de la posicion anterior.
    cmp bordeExiste, 0
    je AS_DibujarActual

    xor ax, ax
    mov al, filaBordeAnterior
    cmp al, 10
    jb AS_FilaAnterior
    sub al, 10

AS_FilaAnterior:
    xor bx, bx
    mov bl, separacionY
    mul bx
    xor bx, bx
    mov bl, tableroY
    add ax, bx
    mov coordYTemp, ax

    push coordYTemp

    xor ax, ax
    mov al, posicionBordeAnterior
    xor bx, bx
    mov bl, separacionX
    mul bx
    xor bx, bx
    mov bl, tableroX
    add ax, bx
    push ax

    call BorrarBorde

AS_DibujarActual:

    ; Calcular la fila visible actual.
    xor ax, ax
    mov al, intentosUsados
    cmp al, 10
    jb AS_FilaActual
    sub al, 10

AS_FilaActual:
    xor bx, bx
    mov bl, separacionY
    mul bx
    xor bx, bx
    mov bl, tableroY
    add ax, bx
    mov coordYTemp, ax

    ; Calcular X de la posicion seleccionada.
    xor ax, ax
    mov al, posicionSeleccionada
    xor bx, bx
    mov bl, separacionX
    mul bx
    xor bx, bx
    mov bl, tableroX
    add ax, bx
    mov coordXTemp, ax

    ; Limpiar la casilla actual antes de dibujar el sprite.
    ; Esto evita que queden pixeles del color anterior.
    push coordYTemp
    push coordXTemp
    call BorrarGema

    xor bx, bx
    mov bl, posicionSeleccionada
    cmp posicionesLlenas[bx], 1
    jne AS_BordeNuevo

    xor ax, ax
    mov al, combinacionJugador[bx]
    push ax
    push coordXTemp
    push coordYTemp
    call DibujarFicha

AS_BordeNuevo:
    mov colorBorde, 51

    push coordYTemp
    push coordXTemp
    call DibujarBorde

    mov al, posicionSeleccionada
    mov posicionBordeAnterior, al
    mov al, intentosUsados
    mov filaBordeAnterior, al
    mov bordeExiste, 1

    pop bx
    pop ax
    ret

ActualizarSeleccion ENDP


; ============================================================
; 14. CONTROL PRINCIPAL
; ============================================================

ControlarGemas PROC

    call DibujarTablero

CG_Bucle:

    mov ah, 01h
    int 16h
    jz CG_Bucle

    mov ah, 00h
    int 16h
    call ProcesarTecla

    ; AL=0 continua; AL=1 reinicia; AL=2 menu.
    cmp al, 0
    jne CG_Fin

    call ActualizarSeleccion
    jmp CG_Bucle

CG_Fin:
    ret

ControlarGemas ENDP


; ============================================================
; 15. PROCESAR TECLAS
; ============================================================

ProcesarTecla PROC

    push bx
    push dx

    ; Flecha izquierda.
    cmp ah, 4Bh
    jne PT_Derecha

    cmp posicionSeleccionada, 0
    jne PT_IzquierdaMover

    jmp PT_SinCambio

PT_IzquierdaMover:

    dec posicionSeleccionada
    jmp PT_CargarColor


; Flecha derecha.
PT_Derecha:

    cmp ah, 4Dh
    jne PT_Arriba

    mov al, cantidadGemas
    dec al

    cmp posicionSeleccionada, al
    jb PT_DerechaMover

    jmp PT_SinCambio

PT_DerechaMover:

    inc posicionSeleccionada
    jmp PT_CargarColor


; Flecha arriba.
PT_Arriba:

    cmp ah, 48h
    jne PT_Abajo

    cmp colorActual, 0
    jne PT_Decrementar

    mov al, cantidadColores
    dec al

    mov colorActual, al
    jmp PT_GuardarColor

PT_Decrementar:

    dec colorActual
    jmp PT_GuardarColor


; Flecha abajo.
PT_Abajo:

    cmp ah, 50h
    jne PT_F2

    inc colorActual

    mov al, cantidadColores

    cmp colorActual, al
    jb PT_GuardarColor

    mov colorActual, 0
    jmp PT_GuardarColor


; F2: modo de prueba.
PT_F2:

    cmp ah, 3Ch
    jne PT_Borrar

    call IntroducirSecretoManual
    jmp PT_SinCambio


; Backspace: borrar posicion.
PT_Borrar:

    cmp al, 08h
    jne PT_Enter

    xor bx, bx
    mov bl, posicionSeleccionada

    mov posicionesLlenas[bx], 0
    mov combinacionJugador[bx], 0
    mov colorActual, 0

    jmp PT_SinCambio


; Enter: confirmar.
PT_Enter:

    cmp al, 0Dh
    jne PT_Escape

    call ConfirmarIntento

    ; 0 continuar; 1 reiniciar; 2 regresar al menu.
    cmp al, 0
    jne PT_Fin

    jmp PT_SinCambio


; Escape: regresar al menu.
PT_Escape:

    cmp al, 1Bh
    jne PT_Numero

    mov al, 2
    jmp PT_Fin


; Numeros del 1 al 8.
PT_Numero:

    cmp al, '1'
    jb PT_SinCambio

    cmp al, '8'
    ja PT_SinCambio

    sub al, '1'

    cmp al, cantidadColores
    jae PT_SinCambio

    mov colorActual, al


; Guardar color seleccionado.
PT_GuardarColor:

    xor bx, bx
    mov bl, posicionSeleccionada

    mov al, colorActual

    mov combinacionJugador[bx], al
    mov posicionesLlenas[bx], 1

    jmp PT_SinCambio


; Cargar color de la posicion seleccionada.
PT_CargarColor:

    xor bx, bx
    mov bl, posicionSeleccionada

    cmp posicionesLlenas[bx], 1
    jne PT_ColorVacio

    mov al, combinacionJugador[bx]
    mov colorActual, al

    jmp PT_SinCambio

PT_ColorVacio:

    mov colorActual, 0


PT_SinCambio:

    mov al, 0
    jmp PT_Fin

PT_Salir:

    mov al, 2


PT_Fin:

    pop dx
    pop bx

    ret

ProcesarTecla ENDP


; ============================================================
; 16. CONFIRMAR INTENTO
; ============================================================

ConfirmarIntento PROC

    push bx
    push si

    xor si, si

CI_Validar:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae CI_Evaluar

    cmp posicionesLlenas[si], 1
    jne CI_Incompleto

    inc si
    jmp CI_Validar


CI_Incompleto:

    call MostrarAvisoIncompleto

    xor al, al
    jmp CI_Fin


CI_Evaluar:

    call EvaluarIntento
    call GuardarIntento

    ; Comprobar victoria.
    mov al, toros

    cmp al, cantidadGemas
    jne CI_NoVictoria
    jmp CI_Victoria

CI_NoVictoria:

    ; Comprobar derrota.
    mov al, intentosUsados

    cmp al, cantidadIntentos
    jb CI_NoDerrota
    jmp CI_Derrota

CI_NoDerrota:

    ; Continuar con otro intento.
    ; Quitar el borde de seleccion de la fila que acaba de confirmarse.
    ; Se evita limpiar y redibujar toda la pantalla.
    cmp intentosUsados, 10
    jae CI_CambioPagina

    ; Calcular Y de la fila que se acaba de guardar.
    xor ax, ax
    mov al, intentosUsados
    dec al
    xor bx, bx
    mov bl, separacionY
    mul bx
    xor bx, bx
    mov bl, tableroY
    add ax, bx
    mov coordYTemp, ax

    ; Calcular X de la ultima gema seleccionada.
    xor ax, ax
    mov al, posicionSeleccionada
    xor bx, bx
    mov bl, separacionX
    mul bx
    xor bx, bx
    mov bl, tableroX
    add ax, bx

    push coordYTemp
    push ax
    call BorrarBorde

    ; Dibujar solo la fila recien guardada, con sus resultados.
    xor ax, ax
    mov al, intentosUsados
    dec ax
    push ax
    call DibujarFilaHistorial

    ; Preparar y dibujar la nueva fila actual.
    call LimpiarFilaActual
    mov posicionSeleccionada, 0
    mov colorActual, 0
    mov bordeExiste, 0
    call DibujarFilaActual

    xor al, al
    jmp CI_Fin

CI_CambioPagina:
    ; Al comenzar el bloque de intentos 11-12, se reorganizan
    ; las filas visibles para mostrar el nuevo bloque desde arriba.
    call LimpiarFilaActual
    mov posicionSeleccionada, 0
    mov colorActual, 0
    mov bordeExiste, 0
    call DibujarTablero

    xor al, al
    jmp CI_Fin


CI_Victoria:

    mov al, 1
    call PantallaFin

    call EsperarFinPartida
    cmp al, 1
    je CI_Reiniciar
    mov al, 2
    jmp CI_Fin


CI_Derrota:

    mov al, 0
    call PantallaFin

    call EsperarFinPartida
    cmp al, 1
    je CI_Reiniciar
    mov al, 2
    jmp CI_Fin

CI_Reiniciar:
    mov al, 1

CI_Fin:

    pop si
    pop bx

    ret

ConfirmarIntento ENDP


; ============================================================
; 17. LIMPIAR FILA ACTUAL
; ============================================================

LimpiarFilaActual PROC

    push si

    xor si, si

LFA_Loop:

    cmp si, 5
    jae LFA_Fin

    mov combinacionJugador[si], 0
    mov posicionesLlenas[si], 0

    inc si
    jmp LFA_Loop

LFA_Fin:

    pop si

    ret

LimpiarFilaActual ENDP


; ============================================================
; 18. AVISO DE FILA INCOMPLETA
; ============================================================

MostrarAvisoIncompleto PROC

    push ax

    call BorrarBarra

    DIBUJA_TEXTO txtCompletar, 4, 184, 51

    mov ah, 00h
    int 16h

    pop ax

    ret

MostrarAvisoIncompleto ENDP


; ============================================================
; 19. ESPERAR DESPUES DE VICTORIA O DERROTA
;
; AL = 1: reiniciar
; AL = 0: regresar al menu
; ============================================================

EsperarFinPartida PROC

EFP_Tecla:

    mov ah, 00h
    int 16h

    cmp al, 0Dh
    je EFP_Reiniciar

    cmp al, 1Bh
    je EFP_Salir

    jmp EFP_Tecla


EFP_Reiniciar:

    mov al, 1
    ret


EFP_Salir:

    mov al, 0

    ret

EsperarFinPartida ENDP


; ============================================================
; 20. PANTALLA DE VICTORIA O DERROTA
; ============================================================

PantallaFin PROC

    push ax
    push bx
    push si

    ; Guardar resultado.
    mov byte ptr mensajeActual, al

    call LimpiarPantalla
    call ConfigurarPaleta

    cmp byte ptr mensajeActual, 1
    jne PF_Derrota

    DIBUJA_TEXTO txtVictoria, 28, 35, 51
    jmp PF_Gemas

PF_Derrota:

    DIBUJA_TEXTO txtDerrota, 28, 30, 51

PF_Gemas:

    xor si, si

PF_Dibujar:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae PF_Texto

    xor ax, ax
    mov al, combinacionSecreta[si]

    push ax

    mov ax, si
    mov bx, 20
    mul bx

    add ax, 100
    push ax

    mov ax, 90
    push ax

    call DibujarFicha

    inc si
    jmp PF_Dibujar


PF_Texto:

    DIBUJA_TEXTO txtReiniciar, 20, 140, 51

    pop si
    pop bx
    pop ax

    ret

PantallaFin ENDP


; ============================================================
; 21. INTRODUCIR COMBINACION SECRETA MANUAL
; ============================================================

IntroducirSecretoManual PROC

    push ax
    push bx
    push si

    call LimpiarPantalla

    ; Instruccion para introducir la combinacion manual.
    DIBUJA_TEXTO txtManual, 30, 20, 51

    xor si, si

ISM_Bucle:

    xor ax, ax
    mov al, cantidadGemas

    cmp si, ax
    jae ISM_Fin

    ; Leer tecla.
    mov ah, 00h
    int 16h

    ; Escape cancela la entrada.
    cmp al, 1Bh
    je ISM_Cancelar

    ; Solo aceptar numeros del 1 al 8.
    cmp al, '1'
    jb ISM_Bucle

    cmp al, '8'
    ja ISM_Bucle

    sub al, '1'

    cmp al, cantidadColores
    jae ISM_Bucle

    mov combinacionSecreta[si], al

    ; Mostrar gema ingresada.
    push ax

    mov ax, si
    mov bx, 20
    mul bx

    add ax, 30
    push ax

    mov ax, 60
    push ax

    call DibujarFicha

    inc si
    jmp ISM_Bucle


ISM_Fin:

    mov modoPrueba, 1

    ; Indicar que la combinacion ya fue ingresada.
    DIBUJA_TEXTO txtPulsaEnter, 30, 100, 51

ISM_EsperarEnter:
    mov ah, 00h
    int 16h
    cmp al, 0Dh
    jne ISM_EsperarEnter

    jmp ISM_Salir


ISM_Cancelar:

    mov modoPrueba, 0


ISM_Salir:

    call LimpiarPantalla
    call ConfigurarPaleta
    call DibujarTablero

    pop si
    pop bx
    pop ax

    ret

IntroducirSecretoManual ENDP

; ============================================================
; 22. ESPERAR UN TIC DEL RELOJ
; ============================================================

EsperarTick PROC

    push ax
    push dx

ET_Esperar:

    mov ah, 00h
    int 1Ah

    cmp dx, tickAnterior
    je ET_Esperar

    mov tickAnterior, dx

    pop dx
    pop ax

    ret

EsperarTick ENDP