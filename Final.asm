include macros2.asm
include number.asm

.model large
.386
.stack 200h

.DATA

_cad0                         	db	"Ingrese numero",'$', 16 dup (?)
id5                           	dd	?
_1                            	dd	1
_cad1                         	db	"El valor debe ser >=1",'$', 23 dup (?)
_cad2                         	db	"La lista tiene menos elementos que el indicado",'$', 48 dup (?)
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
resul                         	dd	?

.CODE
START:
	mov AX,@DATA
	mov DS,AX
	mov ES,AX

; ESCRIBIR
displayString _cad0
newLine 1
; LEER
GetInteger id5
newLine 1
; COMPARACION
FLD id5
FCOMP _1
FSTSW ax
SAHF
FFREE
JAE _etiq7
; ESCRIBIR
displayString _cad1
newLine 1
JMP FINAL
_etiq7:
; RESTA
FLD @cantElementos
FLD id5
FSUB 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cantSaltos
FFREE
; COMPARACION
FLD id5
FCOMP @cantElementos
FSTSW ax
SAHF
FFREE
JNA _etiq14
; ESCRIBIR
displayString _cad2
newLine 1
JMP FINAL
_etiq14:
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq19
; SUMA
FLD @acum
FLD _10
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @acum
FFREE
_etiq19:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cont
FFREE
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq26
; SUMA
FLD @acum
FLD _20
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @acum
FFREE
_etiq26:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cont
FFREE
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq33
; SUMA
FLD @acum
FLD _30
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @acum
FFREE
_etiq33:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cont
FFREE
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq40
; SUMA
FLD @acum
FLD _40
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @acum
FFREE
_etiq40:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cont
FFREE
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq47
; SUMA
FLD @acum
FLD _5
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @acum
FFREE
_etiq47:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cont
FFREE
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq54
; SUMA
FLD @acum
FLD _4
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @acum
FFREE
_etiq54:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux
FFREE
; ASIGNACION
FLD @aux
FSTP @cont
FFREE
; ASIGNACION
FLD @acum
FSTP resul
FFREE
; ESCRIBIR
displayString resul
newLine 1


FINAL:
	mov ah, 1 ; pausa, espera que oprima una tecla:
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h ; Enviamos la interripcion 21h
END START; final del archivo.
