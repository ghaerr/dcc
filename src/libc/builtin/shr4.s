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

;       _SHR4   shift right     shr     dx ax,cl

        public  _SHR4
_SHR4:  mov     ch,0            ;need count in cx
        jcxz    shr4r
shr_lp: shr     dx,1            ;low bit in carry
        rcr     ax,1            ;now high in ax
        loop    shr_lp
shr4r:  _ret
