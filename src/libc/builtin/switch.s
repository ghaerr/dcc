;
;  Released under the GNU LGPL.  See http://www.gnu.org/licenses/lgpl.txt
;
;  This program is part of the DeSmet C Compiler
;
;  This library is free software; you can redistribute it and/or modify
;  it under the terms of the GNU Lesser General Public License as published
;  by the Free Software Foundatation; either version 2.1 of the License, or
;  any later version.
;
;  This library is distributed in the hope that it will be useful, but
;  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
;  License for more details.
;
;       c88 builtins

        include "config.h"
        cseg

;       _SWITCH         dispatch for a c switch statement
;                       bx has the switch value.
;                       table follows call. default is 0x8000 and is
;                       always somewhere in the list.
;                               word            number of cases.
;                               word,word       value,label.
; for big model,recall that switch only works in current segment

        public  _SWITCH
_SWITCH:if      LARGE_CASE == 0
        mov     si,cs
        mov     es,si           ;set es to segment of call
        endif
        pop     si              ;return is address of table
        if      LARGE_CASE
        pop     es              ;segment of caller
        push    es
        endif
        mov     cx,es:[si]      ;number of cases
        add     si,2            ;point to first entry
sw_lp:  mov     ax,es:[si]      ;next case value
        cmp     ax,08000h       ;default case ?
        jz      set_def         ;yes
        cmp     bx,ax           ;is the case a match?
        jz      sw_mat          ;yes
        add     si,4
        loop    sw_lp           ;try the next in list
        jmp     dx              ;go to the default label
set_def:mov     dx,es:[si+2]    ;dx is default destination
        add     si,4
        loop    sw_lp           ;try the next in list
        if      LARGE_CASE
        push    dx
        lret
        endif
        jmp     dx              ;go to the default label
sw_mat: if      LARGE_CASE
        push    word es:[si+2]
        lret
        endif
        jmp     word es:[si+2]  ;found a matching label
