.MODEL LARGE
.386
.STACK 200h

.DATA

_cad0                         	dd	"El valor debe ser >=1"
_cad1                         	dd	"La lista tiene menos elementos que el indicado"
_10                           	dd	10
_20                           	dd	20
_30                           	dd	30
_40                           	dd	40
_5                            	dd	5
_4                            	dd	4
@cont                         	dd	0
@acum                         	dd	0
@cantSaltos                   	dd	0
@cantElementos                	dd	6

.CODE
MOV ax, @data
MOV ds, ax
FINIT; Inicializa el coprocesador



FINAL:
	mov ah, 1 ; pausa, espera que oprima una tecla:
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h ; Enviamos la interripcion 21h
END ; final del archivo.
