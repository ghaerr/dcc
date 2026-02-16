;	int a[1],b[1] ;
;	int i ;
;	for (i=c1;i<c2;i++) a[i]=b[i] ;

 MOV WORD [BP-6],c1
_L1:
 CMP WORD [BP-6],c2
 JGE _L2
 MOV SI,WORD [BP-6]
 SHL SI,1		; Omitted for char array
 MOV DI,WORD [BP-6]
 SHL DI,1		; Omitted for char array
 MOV SI,WORD [BP+SI-4]
 MOV WORD [BP+DI-2],SI
_L3:
 INC WORD [BP-6]
 JMP _L1
_L2:

	; replace by:
	; -----------

; PUBLIC _MOVE_

	PUSH	DST	; may need to compute
	PUSH	SRC	; may need to compute
	PUSH	N	; may need to compute
	CALL	_move_
	ADD	SP,6
