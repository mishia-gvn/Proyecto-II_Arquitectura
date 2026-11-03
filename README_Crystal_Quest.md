# CRYSTAL QUEST

Juego de deducción lógica desarrollado en lenguaje ensamblador x86 de 16 bits para DOS, como proyecto del curso **EIF205 – Arquitectura de Computadores**.

## 1. Descripción

Crystal Quest adapta la mecánica de *Mastermind* o *Toros y Vacas* a una temática de gemas. El objetivo es descubrir una combinación secreta seleccionando colores y utilizando las pistas que proporciona el programa después de cada intento:

- **T:** una gema tiene el color correcto y está en la posición correcta.
- **V:** una gema tiene el color correcto, pero está en una posición diferente.

El juego presenta una interfaz gráfica VGA y utiliza sprites de gemas y una paleta de colores personalizada.

## 2. Tecnologías y herramientas

- Procesador objetivo: arquitectura x86 de 16 bits.
- Lenguaje: ensamblador.
- Ensamblador: Turbo Assembler (TASM).
- Enlazador: Turbo Linker (TLINK).
- Entorno de ejecución: DOSBox.
- Modo gráfico: VGA 13h, resolución de 320 × 200 píxeles y 256 colores.

Las herramientas TASM y TLINK se incluyen en la carpeta `tools` del proyecto.

## 3. Estructura del proyecto

``` text
Gemas/
├── src/
│   ├── gemas.asm
│   ├── graf.inc
│   ├── juego.inc
│   ├── letras.inc
│   ├── menu.inc
│   ├── sprite.inc
│   └── comp.bat
├── sprites/
│   ├── hoja_sprites.png
│   └── imágenes de las gemas
├── tools/
│   ├── TASM.EXE
│   ├── TLINK.EXE
│   └── otras utilidades incluidas
├── obj/
│   └── GEMAS.OBJ (generado durante la compilación)
└── bin/
    ├── GEMAS.EXE (ejecutable generado)
    └── GEMAS.MAP (mapa generado por el enlazador)
```

### Archivos fuente

| Archivo | Descripción |
|----|----|
| `gemas.asm` | Programa principal. Declara variables y textos, incluye los archivos de datos y módulos, inicializa el entorno gráfico y controla el flujo principal. |
| `graf.inc` | Procedimientos gráficos: configuración de paleta, impresión de texto, dibujo de sprites, bordes y borrado de regiones. |
| `menu.inc` | Pantalla de presentación, menú principal, selección de dificultad, instrucciones y créditos. |
| `juego.inc` | Configuración de la partida, generación de la combinación, tablero, selección de gemas, evaluación de intentos e historial. |
| `sprite.inc` | Datos de la paleta y patrones de los sprites de las gemas. |
| `letras.inc` | Patrones utilizados para representar caracteres. |
| `comp.bat` | Automatiza la limpieza de archivos anteriores, el ensamblado, el enlazado y la ejecución del juego. |

Los archivos `.INC` se incorporan al programa principal mediante directivas `INCLUDE`; no se ejecutan por separado.

## 4. Requisitos

Para compilar y ejecutar el proyecto se necesita:

1.  Un equipo compatible con DOSBox.
2.  DOSBox instalado y funcionando.
3.  La carpeta del proyecto completa, conservando la estructura de directorios.
4.  Los ejecutables `TASM.EXE` y `TLINK.EXE` ubicados en `tools`.

Se recomienda utilizar DOSBox con una configuración compatible con los programas DOS de 16 bits.

## 5. Preparación en DOSBox

El archivo `comp.bat` utiliza rutas desde la raíz de la unidad DOS:

- `\SRC`
- `\TOOLS`
- `\OBJ`
- `\BIN`

Por esta razón, se debe montar la carpeta raíz del proyecto como la unidad `C:`. La carpeta montada debe contener directamente los directorios `src`, `tools`, `obj` y `bin`.

Ejemplo: si el proyecto está guardado en Windows en `C:\Proyectos\Gemas`, dentro de DOSBox se puede ejecutar:

``` dos
mount c C:\Proyectos\Gemas
c:
```

**Importante:** sustituya `C:\Proyectos\Gemas` por la ubicación real de la carpeta del proyecto en su equipo. No monte únicamente la carpeta `src`, porque el archivo BAT necesita acceder también a `tools`, `obj` y `bin`.

## 6. Compilación y ejecución

Una vez montada la carpeta del proyecto en DOSBox:

``` dos
c:
cd \src
comp
```

También se puede ejecutar el archivo indicando su extensión:

``` dos
comp.bat
```

El archivo `comp.bat` realiza los siguientes pasos:

1.  Elimina los archivos anteriores `GEMAS.OBJ`, `GEMAS.EXE` y `GEMAS.MAP`, si existen.
2.  Se ubica en el directorio `\SRC`.
3.  Ejecuta TASM para ensamblar `GEMAS.ASM` y generar `..\OBJ\GEMAS.OBJ`.
4.  Comprueba si TASM reportó un error. Si ocurre, pasa a la sección de error y no continúa con el enlazado.
5.  Ejecuta TLINK para generar `..\BIN\GEMAS.EXE` y `..\BIN\GEMAS.MAP`.
6.  Si el enlazado termina correctamente, ejecuta `..\BIN\GEMAS.EXE`.
7.  Al finalizar, muestra una pausa en la consola.

### Compilación manual (opcional)

Desde el directorio `\SRC`, los pasos equivalentes son:

``` dos
\TOOLS\TASM GEMAS.ASM, ..\OBJ\GEMAS.OBJ
\TOOLS\TLINK ..\OBJ\GEMAS.OBJ, ..\BIN\GEMAS.EXE, ..\BIN\GEMAS.MAP
..\BIN\GEMAS.EXE
```

Se recomienda utilizar `comp.bat`, ya que incluye la comprobación de errores entre las etapas.

## 7. Instrucciones básicas del juego

1.  Inicie el programa y espere la pantalla de presentación.
2.  Acceda al menú principal y seleccione la opción deseada.
3.  Seleccione un nivel de dificultad.
4.  Durante la partida, complete las posiciones de la combinación con las gemas disponibles.
5.  Confirme el intento con `ENTER`.
6.  Interprete los resultados: T indica coincidencias exactas y la V indica coincidencias de color en otra posición.
7.  Continúe hasta descubrir la combinación secreta o agotar los intentos disponibles.

### Controles

| Tecla | Acción |
|----|----|
| Flecha izquierda / derecha | Mover la posición seleccionada. |
| Flecha arriba / abajo | Cambiar el color de la posición. |
| Teclas numéricas `1` a `8` | Seleccionar un color, según los colores habilitados en el nivel. |
| `ENTER` | Confirmar la opción o el intento, según la pantalla. |
| `BACKSPACE` | Borrar la posición seleccionada. |
| `F2` | Acceder a la introducción manual del secreto para pruebas. |
| `ESC` | Regresar o cancelar en las pantallas donde está habilitado. |

## 8. Niveles de dificultad

| Nivel   | Cantidad de gemas | Colores disponibles | Intentos |
|---------|------------------:|--------------------:|---------:|
| Fácil   |                 4 |                   6 |       12 |
| Normal  |                 4 |                   6 |       10 |
| Difícil |                 5 |                   8 |       10 |

En el nivel Fácil no se permiten colores repetidos en la combinación secreta. Los niveles Normal y Difícil permiten repeticiones.

## 9. Solución de problemas

### TASM o TLINK no se encuentran

- Compruebe que `tools` se encuentra directamente dentro de la carpeta montada como unidad `C:`.
- Verifique que los archivos `TASM.EXE` y `TLINK.EXE` estén presentes.
- Compruebe que está ejecutando el comando desde DOSBox y no desde la consola de Windows.

### No se encuentra `GEMAS.ASM`

- Compruebe que el directorio `src` está dentro de la carpeta montada.
- Ejecute `cd \src` antes de iniciar `comp.bat`.

### La compilación muestra errores

- Revise el mensaje de TASM para identificar el archivo y la línea reportados.
- Si TASM falla, el archivo BAT detiene el proceso antes de enlazar.
- Si TLINK falla, revise los mensajes del enlazador y confirme que se generó correctamente el archivo objeto.

### El programa no inicia

- Confirme que `GEMAS.EXE` se haya generado en `bin`.
- Ejecute el archivo desde DOSBox.
- Verifique que la estructura de directorios no haya sido modificada.

## 10. Autora

**Mishia Valverde Narváez**\
Proyecto individual\
Curso: EIF205 – Arquitectura de Computadores\
Universidad Nacional

## 11. Créditos y herramientas de apoyo

El proyecto fue desarrollado por la autora como parte del curso EIF205. Se utilizó ChatGPT como herramienta de apoyo para consultas conceptuales, revisión de errores y elaboración de documentación.
