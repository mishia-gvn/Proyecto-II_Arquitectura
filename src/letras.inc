; ============================================================

; Caracteres ASCII 32..126
; Cada caracter ocupa 8 bytes.
; El bit 7 representa el pixel izquierdo.
; Las cadenas terminan con '$'.

; ============================================================

tablaLetrasGeneral LABEL BYTE

    ; ASCII 32..47: espacio y signos
    db 00h,00h,00h,00h,00h,00h,00h,00h ; ESPACIO
    db 18h,18h,18h,18h,18h,00h,18h,00h ; !
    db 28h,28h,28h,00h,00h,00h,00h,00h ; "
    db 14h,14h,3Eh,34h,7Ch,28h,28h,00h ; #
    db 08h,1Eh,28h,3Ch,1Eh,0Ah,3Ch,08h ; $
    db 70h,50h,76h,18h,6Eh,0Ah,0Eh,00h ; %
    db 1Ch,18h,08h,1Eh,36h,36h,1Eh,00h ; &
    db 10h,10h,10h,00h,00h,00h,00h,00h ; '
    db 08h,18h,18h,18h,18h,18h,08h,04h ; (
    db 08h,0Ch,0Ch,0Ch,0Ch,0Ch,08h,10h ; )
    db 2Ah,1Ch,1Ch,2Ah,00h,00h,00h,00h ; *
    db 00h,10h,10h,7Ch,10h,10h,00h,00h ; +
    db 00h,00h,18h,18h,30h,00h,00h,00h ; ,
    db 00h,00h,38h,00h,00h,00h,00h,00h ; -
    db 00h,00h,00h,18h,18h,00h,00h,00h ; .
    db 02h,04h,04h,08h,08h,10h,10h,20h ; /

    ; ASCII 48..57: numeros
    db 1Ch,36h,36h,3Eh,36h,36h,1Ch,00h ; 0
    db 78h,18h,18h,18h,18h,18h,7Eh,00h ; 1
    db 78h,0Ch,0Ch,18h,30h,60h,7Ch,00h ; 2
    db 3Eh,06h,18h,06h,06h,06h,3Ch,00h ; 3
    db 0Ch,0Ch,1Ch,2Ch,3Eh,0Ch,0Ch,00h ; 4
    db 3Eh,30h,3Ch,06h,06h,06h,3Ch,00h ; 5
    db 1Eh,30h,3Ch,36h,36h,36h,1Ch,00h ; 6
    db 3Eh,02h,04h,04h,08h,08h,10h,00h ; 7
    db 1Ch,36h,36h,08h,36h,36h,1Ch,00h ; 8
    db 1Ch,36h,36h,36h,1Eh,06h,3Ch,00h ; 9

    ; ASCII 58..64: signos
    db 00h,18h,18h,00h,18h,18h,00h,00h ; :
    db 00h,18h,18h,00h,18h,18h,30h,00h ; ;
    db 00h,02h,1Ch,20h,1Ch,02h,00h,00h ; <
    db 00h,3Eh,00h,3Eh,00h,00h,00h,00h ; =
    db 00h,20h,1Ch,02h,1Ch,20h,00h,00h ; >
    db 1Ch,2Ch,0Ch,10h,18h,00h,18h,00h ; ?
    db 1Ch,22h,4Eh,52h,52h,4Eh,20h,1Eh ; @

    ; ASCII 65..90: letras mayusculas
    db 08h,1Ch,14h,14h,1Ch,36h,36h,00h ; A
    db 3Ch,36h,36h,38h,36h,36h,3Ch,00h ; B
    db 1Eh,30h,30h,30h,30h,30h,1Eh,00h ; C
    db 3Ch,36h,36h,36h,36h,36h,3Ch,00h ; D
    db 3Eh,30h,30h,3Eh,30h,30h,3Eh,00h ; E
    db 3Eh,30h,30h,3Eh,30h,30h,30h,00h ; F
    db 1Eh,30h,30h,36h,36h,36h,1Eh,00h ; G
    db 36h,36h,36h,3Eh,36h,36h,36h,00h ; H
    db 3Ch,18h,18h,18h,18h,18h,3Ch,00h ; I
    db 1Eh,06h,06h,06h,06h,26h,1Ch,00h ; J
    db 64h,6Ch,78h,78h,6Ch,64h,66h,00h ; K
    db 30h,30h,30h,30h,30h,30h,3Eh,00h ; L
    db 36h,3Eh,3Eh,3Eh,36h,36h,36h,00h ; M
    db 36h,36h,3Eh,3Eh,3Eh,36h,36h,00h ; N
    db 1Ch,36h,36h,36h,36h,36h,1Ch,00h ; O
    db 3Ch,36h,36h,36h,3Ch,30h,30h,00h ; P
    db 1Ch,36h,36h,36h,36h,36h,1Ch,04h ; Q
    db 78h,6Ch,6Ch,70h,68h,6Ch,66h,00h ; R
    db 38h,64h,60h,38h,0Ch,4Ch,38h,00h ; S
    db 7Eh,18h,18h,18h,18h,18h,18h,00h ; T
    db 36h,36h,36h,36h,36h,36h,1Ch,00h ; U
    db 36h,36h,14h,14h,14h,1Ch,1Ch,00h ; V
    db 42h,5Ah,5Ah,7Eh,3Ch,24h,24h,00h ; W
    db 36h,14h,1Ch,08h,1Ch,14h,36h,00h ; X
    db 66h,24h,3Ch,18h,18h,18h,18h,00h ; Y
    db 3Eh,06h,0Ch,08h,18h,30h,3Eh,00h ; Z

    ; ASCII 91..96: signos
    db 18h,18h,18h,18h,18h,18h,18h,1Ch ; [
    db 20h,10h,10h,08h,08h,04h,04h,02h ; \
    db 0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,0Ch,1Ch ; ]
    db 10h,28h,6Ch,00h,00h,00h,00h,00h ; ^
    db 00h,00h,00h,00h,7Eh,00h,00h,00h ; _
    db 30h,18h,00h,00h,00h,00h,00h,00h ; `

    ; ASCII 97..122: letras minusculas
    db 00h,1Ch,06h,3Eh,36h,3Eh,00h,00h ; a
    db 30h,30h,30h,3Ch,36h,36h,36h,3Ch ; b
    db 00h,1Eh,30h,30h,30h,1Eh,00h,00h ; c
    db 06h,06h,06h,1Eh,36h,36h,36h,1Eh ; d
    db 00h,1Ch,36h,3Eh,30h,1Eh,00h,00h ; e
    db 0Eh,18h,18h,3Eh,18h,18h,18h,18h ; f
    db 1Eh,36h,36h,36h,1Eh,06h,1Ch,00h ; g
    db 30h,30h,30h,3Eh,36h,36h,36h,36h ; h
    db 18h,18h,00h,38h,18h,18h,18h,7Eh ; i
    db 0Ch,00h,1Ch,0Ch,0Ch,0Ch,0Ch,0Ch ; j
    db 30h,30h,30h,34h,38h,3Ch,34h,36h ; k
    db 38h,18h,18h,18h,18h,18h,18h,0Eh ; l
    db 00h,3Eh,2Ah,2Ah,2Ah,2Ah,00h,00h ; m
    db 00h,3Eh,36h,36h,36h,36h,00h,00h ; n
    db 00h,1Ch,36h,36h,36h,1Ch,00h,00h ; o
    db 3Ch,36h,36h,36h,3Ch,30h,30h,00h ; p
    db 1Eh,36h,36h,36h,1Eh,06h,06h,00h ; q
    db 00h,3Ch,30h,30h,30h,30h,00h,00h ; r
    db 00h,1Eh,30h,1Ch,06h,3Eh,00h,00h ; s
    db 18h,18h,3Eh,18h,18h,18h,1Eh,00h ; t
    db 00h,36h,36h,36h,36h,3Eh,00h,00h ; u
    db 00h,36h,36h,14h,14h,1Ch,00h,00h ; v
    db 00h,22h,2Ah,3Eh,14h,14h,00h,00h ; w
    db 00h,36h,1Ch,08h,14h,36h,00h,00h ; x
    db 36h,14h,14h,1Ch,08h,18h,30h,00h ; y
    db 00h,3Eh,04h,08h,10h,3Eh,00h,00h ; z

    ; ASCII 123..126: signos
    db 18h,18h,18h,60h,18h,18h,18h,1Ch ; {
    db 10h,10h,10h,10h,10h,10h,10h,10h ; |
    db 18h,18h,18h,06h,18h,18h,18h,38h ; }
    db 00h,38h,06h,00h,00h,00h,00h,00h ; ~
