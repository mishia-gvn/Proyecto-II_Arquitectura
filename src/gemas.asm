.model small
.stack 100h
.data  
    modoOriginal db ?
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

    ; Segmento de memoria de video
    mov ax, 0A000h
    mov es, ax

    ; Calcular la posicion del pixel y darle color
    mov di, 7DA0h
    mov BYTE PTR es:[di], 12

    ; Esperar una tecka antes de continuar
    mov ah, 00h
    int 16h

    ; Restaurar el modo de video original
    mov ah, 00h
    mov al, modoOriginal
    int 10h 

    mov ax, 4C00h
    int 21h 
main ENDP
end main
