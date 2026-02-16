;	int a[1], i ;
;	for (i=c1;i<c2;i++) a[i]=src ;

 MOV WORD [BP-4],c1	; may be an instruction pair
_L1:
 CMP WORD [BP-4],c2	; may be an instruction pair
 JGE _L2		; JGE/JG/JAE/JA/JZ
 MOV SI,WORD [BP-4]
 SHL SI,1		; omitted for char array
 MOV AX,src		; omitted if src is a constant
 MOV WORD [BP+SI-2],src	; dst[SI]
_L3:
 INC WORD [BP-4]
 JMP _L1
_L2:

	; Replace the above with:
	; -----------------------

; PUBLIC _setmem_

 MOV AX,src		; value to store
 PUSH AX

 MOV AX,n		; JGE/JAE/JZ:c2-c1, JG/JA:c2-c1+1 (compute if nec.)
 PUSH AX

 MOV SI,c1		; starting index
 SHL SI,1		; omit for char array
 LEA SI,WORD dst[SI]	; get 1st destination
 PUSH SI

 CALL _setmem_
 ADD SP,6
