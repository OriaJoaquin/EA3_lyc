include macros2.asm
include number.asm

.model large
.386
.stack 200h

.DATA

_cad0                         	db	"Ingrese un valor pivot mayor o igual a 1:",'$', 43 dup (?)
pivot                         	dd	?
_1                            	dd	1
_cad1                         	db	"El valor debe ser >=1",'$', 23 dup (?)
_cad2                         	db	"La lista tiene menos elementos que el indicado",'$', 48 dup (?)
@aux13                        	dd	?
_200                          	dd	200
@aux17                        	dd	?
@aux20                        	dd	?
_170                          	dd	170
@aux24                        	dd	?
@aux27                        	dd	?
_150                          	dd	150
@aux31                        	dd	?
@aux34                        	dd	?
_50                           	dd	50
@aux38                        	dd	?
@aux41                        	dd	?
_25                           	dd	25
@aux45                        	dd	?
@aux48                        	dd	?
@cont                         	dd	0
@acum                         	dd	0
@cantSaltos                   	dd	0
@cantElementos                	dd	5
resul                         	dd	?
_cad3                         	db	"El resultado es:",'$', 18 dup (?)

.CODE
START:
	mov AX,@DATA
	mov DS,AX
	mov ES,AX

; ESCRIBIR CAD
displayString _cad0
newLine 1
; LEER
GetInteger pivot
newLine 1
; COMPARACION
FLD pivot
FCOMP _1
FSTSW ax
SAHF
FFREE
JAE _etiq7
; ESCRIBIR CAD
displayString _cad1
newLine 1
JMP FINAL
_etiq7:
; COMPARACION
FLD pivot
FCOMP @cantElementos
FSTSW ax
SAHF
FFREE
JNA _etiq12
; ESCRIBIR CAD
displayString _cad2
newLine 1
JMP FINAL
_etiq12:
; RESTA
FLD @cantElementos
FLD pivot
FSUB 
FSTP @aux13
FFREE
; ASIGNACION
FLD @aux13
FSTP @cantSaltos
FFREE
; COMPARACION
FLD @cont
FCOMP @cantSaltos
FSTSW ax
SAHF
FFREE
JB _etiq19
; SUMA
FLD @acum
FLD _200
FADD 
FSTP @aux17
FFREE
; ASIGNACION
FLD @aux17
FSTP @acum
FFREE
_etiq19:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux20
FFREE
; ASIGNACION
FLD @aux20
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
FLD _170
FADD 
FSTP @aux24
FFREE
; ASIGNACION
FLD @aux24
FSTP @acum
FFREE
_etiq26:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux27
FFREE
; ASIGNACION
FLD @aux27
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
FLD _150
FADD 
FSTP @aux31
FFREE
; ASIGNACION
FLD @aux31
FSTP @acum
FFREE
_etiq33:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux34
FFREE
; ASIGNACION
FLD @aux34
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
FLD _50
FADD 
FSTP @aux38
FFREE
; ASIGNACION
FLD @aux38
FSTP @acum
FFREE
_etiq40:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux41
FFREE
; ASIGNACION
FLD @aux41
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
FLD _25
FADD 
FSTP @aux45
FFREE
; ASIGNACION
FLD @aux45
FSTP @acum
FFREE
_etiq47:
; SUMA
FLD @cont
FLD _1
FADD 
FSTP @aux48
FFREE
; ASIGNACION
FLD @aux48
FSTP @cont
FFREE
; ASIGNACION
FLD @acum
FSTP resul
FFREE
; ESCRIBIR CAD
displayString _cad3
newLine 1
; ESCRIBIR ID
displayInteger resul
newLine 1


FINAL:
	mov ah, 1 ; pausa, espera que oprima una tecla:
	int 21h ; AH=1 es el servicio de lectura
	MOV AX, 4C00h ; Sale del Dos
	INT 21h ; Enviamos la interripcion 21h
END START; final del archivo.
