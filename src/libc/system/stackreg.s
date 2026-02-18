; stack and segment register manipulation functions for DCC
; 
        CSEG

        PUBLIC  _showds_
_showds_:
        mov    ax,ds           ;ds (and ss) value
        ret

        PUBLIC  _setsp_
_setsp_:
        pop     dx              ;chop the stack - dangerous operation
        pop     bx              ;new sp value
        cmp     bp,sp           ;any locals
        jnz     L1
        mov     bp,bx           ;must move the bp too
L1:     mov     sp,bx
        push    bx
        jmp     dx
