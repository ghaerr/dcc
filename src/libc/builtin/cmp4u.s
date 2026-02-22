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

;       _cmp4u   set flags for   cmp     dx ax,bx cx

        public  _CMP4U
_CMP4U: cmp     dx,bx           ;compare high words
        jnz     cmp4ru          ;done if different
        cmp     ax,cx           ;break tie with low words
cmp4ru: _ret
