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

;       _cmp4   set flags for   cmp     dx ax,bx cx

        public  _CMP4
_CMP4:  cmp     dx,bx           ;compare high words
        jnz     cmp4r           ;done if different
        jns     norev
        cmp     ax,cx           ;flip usiigned result into signed
        jz      cmp4r
        mov     al,1
        jb      isg
isl:    cmp     al,2
        _ret
norev:  cmp     ax,cx           ;break tie with low words
        jz      cmp4r
        mov     al,1
        jb      isl
isg:    cmp     al,0
cmp4r:  _ret
