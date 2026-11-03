.model small
.stack 100h

.data

; ============================================================
; 1. VARIABLES DEL SISTEMA Y VIDEO

modoOriginal            db ?
semilla                 dw 0
contadorPartidas        dw 0

; ============================================================
; 2. VARIABLES TEMPORALES Y CALCULOS

coordXTemp              dw 0
coordYTemp              dw 0
indiceTemp              dw 0
numeroBuffer            db '00$'

; ============================================================
; 3. VARIABLES DE TIEMPO Y ANIMACION

tickAnterior            dw 0
animFrame               db 0

; ============================================================
; 4. VARIABLES DE PRUEBA

modoPrueba              db 0

; ============================================================
; 5. VARIABLES DEL BORDE DE SELECCION

colorBorde              db 51
colorBordeTemporal      db 51
posicionBordeAnterior   db 0
filaBordeAnterior       db 0
bordeExiste             db 0

; ============================================================
; 6. VARIABLES DE CONFIGURACION DEL JUEGO

dificultad              db 0
; 0 = Facil, 1 = Normal, 2 = Dificil

cantidadGemas           db 4
cantidadIntentos        db 12
cantidadColores         db 6
intentosUsados          db 0

; ============================================================
; 7. COMBINACIONES Y VALIDACION

combinacionSecreta      db 5 dup(0)
ultimaCombinacion       db 5 dup(0)
hayCombinacionAnterior  db 0

combinacionJugador      db 5 dup(0)
posicionesLlenas        db 5 dup(0)

marcaSecreto            db 5 dup(0)
marcaJugador            db 5 dup(0)
marcaResultado          db 5 dup(0)

toros                   db 0
vacas                   db 0

; ============================================================
; 8. HISTORIAL DE INTENTOS

historialGemas          db 60 dup(0)
historialT              db 12 dup(0)
historialV              db 12 dup(0)
historialMarcas         db 60 dup(0)

; ============================================================
; 9. VARIABLES DE CONTROL DEL JUGADOR

posicionSeleccionada    db 0
colorActual             db 0

; ============================================================
; 10. VARIABLES DE NAVEGACION DEL MENU

opcionSeleccionada      db 0
nivelMenu               db 0
posicionIndicador       dw 0
posicionIndicadorX      dw 0
cantidadOpciones        db 0
opcionTemporal          db 0

; ============================================================
; 11. VARIABLES DE POSICION DEL TABLERO

tableroX                db 4
tableroY                db 22
separacionX             db 20
separacionY             db 16

; ============================================================
; 12. VARIABLES PARA MOSTRAR MENSAJES Y TEXTO

mensajeActual           dw 0

textoX                  dw 0
textoY                  dw 0
textoColor              db 63
textoPtr                dw 0
textoFila               db 0

; ============================================================
; 13. TEXTOS DEL MENU PRINCIPAL

textoIndicador          db '>$'
textoTitulo             db 'CRYSTAL QUEST$'
textoJugar              db 'JUGAR$'
textoDificultad         db 'DIFICULTAD$'
textoInstrucciones      db 'INSTRUCCIONES$'
textoCreditos           db 'CREDITOS$'
textoSalir              db 'SALIR$'

; ============================================================
; 14. TEXTOS DE LOS NIVELES DE DIFICULTAD

txtFacil                db 'FACIL 4 GEMAS 6 COLORES 12 TURNOS$'
txtNormal               db 'NORMAL 4 GEMAS 6 COLORES 10 TURNOS$'
txtDificil               db 'DIFICIL 5 GEMAS 8 COLORES 10 TURNOS$'

txtNivelFacil           db 'FACIL$'
txtNivelNormal          db 'NORMAL$'
txtNivelDificil         db 'DIFICIL$'

; ============================================================
; 15. TEXTOS DE INSTRUCCIONES

txtInstruccion1         db '- ADIVINA LA COMBINACION SECRETA.$'
txtInstruccion2         db '- FLECHAS IZQ DER: MOVER POSICION.$'
txtInstruccion3         db '- FLECHAS ARR ABA: CAMBIAR COLOR.$'
txtInstruccion4         db '- TECLAS 1 A 8: ELEGIR COLOR.$'
txtInstruccion5         db '- ENTER: CONFIRMAR INTENTO.$'
txtInstruccion6         db '- BACKSPACE: BORRAR POSICION.$'
txtInstruccion7         db '- T: COLOR Y POSICION CORRECTOS.$'
txtInstruccion8         db '- V: COLOR CORRECTO, OTRO LUGAR.$'
txtInstruccion9         db '- F2: INTRODUCIR SECRETO MANUAL.$'

; ============================================================
; 16. TEXTOS DE CREDITOS Y PRESENTACION

txtCreditos1            db 'CRYSTAL QUEST - JUEGO DE GEMAS$'
txtCreditos2            db 'PROYECTO EIF205 ARQUITECTURA$'
txtCreditos3            db 'DESARROLLADO POR MISHIA$'

txtEquipo               db 'MISHIA VALVERDE NARVAEZ$'

txtPulsaEnter           db 'PRESIONE ENTER PARA CONTINUAR$'

; ============================================================
; 17. TEXTOS DURANTE LA PARTIDA

txtCompletar            db 'COMPLETE TODAS LAS POSICIONES$'
txtVictoria             db 'VICTORIA! COMBINACION DESCUBIERTA$'
txtDerrota              db 'FIN DEL JUEGO - CODIGO SECRETO$'
txtReiniciar            db 'ENTER: NUEVA PARTIDA  ESC: MENU$'

txtRestantes            db 'RESTANTES:$'
txtNivel                db 'NIVEL:$'
txtToros                db 'T:$'
txtVacas                db 'V:$'

txtManual               db 'PRUEBA: INGRESE COLORES DEL 1 AL 8$'
txtIntentos             db 'INTENTOS RESTANTES$'
txtActual               db 'INTENTO ACTUAL$'

; ============================================================
; 18. TEXTOS DE AYUDA

txtAyuda1               db 'FLECHAS MOVER$'
txtAyuda2               db 'ARR ABA COLOR$'
txtAyuda3               db 'ENTER ACEPTA$'
txtAyuda4               db 'BACKSPACE$'

; ============================================================
; 19. DATOS GRAFICOS

include sprite.inc
include letras.inc

; ============================================================

.code

main PROC

    ; Inicializar segmento de datos
    mov ax, @data
    mov ds, ax

    ; Guardar el modo de video original
    mov ah, 0Fh
    int 10h
    mov modoOriginal, al

    ; Activar modo grafico VGA 13h
    mov ax, 0013h
    int 10h

    ; Mostrar presentacion y menu
    call PantallaPresentacion
    call MostrarMenu

    ; Restaurar modo de video original
    mov ah, 00h
    mov al, modoOriginal
    int 10h

    ; Regresar a DOS
    mov ax, 4C00h
    int 21h

main ENDP

; ============================================================

include graf.inc
include menu.inc
include juego.inc

END main