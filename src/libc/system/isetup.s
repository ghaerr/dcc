; startup code for DCC C88 (DeSmet C)
; ELKS system calls part 1
;
; 23 Nov 24 Greg Haerr for C86
; 26 Dec 24 Added C startup code for argc/argv and exit return
; 14 Feb 26 Ported C86 syscall.s to C88 isetup.s
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
include "syselks.h"
;
        DSEG                    ; first data seg
        dw      0,0             ; prevent data having address 0
        PUBLIC  errno_
        PUBLIC  environ_
        PUBLIC  __stacklow_
errno_:     dw      0
environ_:   dw      0
__stacklow_:dw      0
        ;.comm   __argc_,2
        ;.comm   __argv_,2
        ;.comm   __program_filename_,2

        CSEG                    ; first text seg
        ;nop                    ; prevent text having address 0 for SIG_DFL,SIG_IGN
        ;nop
        ;nop
        ;nop

        PUBLIC  _CSETUP,main_
_CSETUP:                        ; C program entry point
        mov     ax,sp
        sub     ax,dx           ; DX is stack size
        mov     __stacklow_,ax
        pop     ax              ; get argc
        ;mov     __argc_,ax
        mov     bx,sp
        ;mov     __argv_,bx
        mov     dx,bx           ; save argc in DX
L1:     cmp     WORD [bx],1
        inc     bx
        inc     bx
        jnc     L1
        mov     environ_,bx
        ;mov     bx,sp
        ;mov     bx,[bx]
        ;mov     __program_filename_, bx
        ;push    __argv_
        push    dx              ; push argv
        push    ax              ; restore argc
        ;call   initrtns
        call    main_
        push    ax              ; pass return value to exit
        call    _exit_          ; no return


        PUBLIC  callsys
callsys:                        ; common routine for ELKS system call
        push    bp
        mov     bp, sp
        push    si
        push    di
        mov     bx, [bp+4]
        mov     cx, [bp+6]
        mov     dx, [bp+8]
        mov     di, [bp+10]
        mov     si, [bp+12]
        int     128
        cmp     ax,0
        jae     L2              ; success
        neg     ax
        mov     errno_, ax
        mov     ax,-1
L2:
        pop     di
        pop     si
        pop     bp
        ret

        PUBLIC  exit_
        PUBLIC  _exit_
exit_:                          ; C exit temp comes here
_exit_:                         ; _exit
        mov     ax,SYS_exit
        jmp     callsys

        PUBLIC  read_
read_:
        mov     ax,SYS_read
        jmp     callsys

        PUBLIC  write_
write_:
        mov     ax,SYS_write
        jmp     callsys

        PUBLIC  open_
open_:
        mov     ax,SYS_open
        jmp     callsys

        PUBLIC  close_
close_:
        mov     ax,SYS_close
        jmp     callsys

        PUBLIC  _lseek_
_lseek_:
        mov     ax,SYS_lseek
        jmp     callsys

        PUBLIC  ioctl_
ioctl_:
        mov     ax,SYS_ioctl
        jmp     callsys

        PUBLIC  _sbrk_
_sbrk_:
        mov     ax,SYS_sbrk
        jmp     callsys

        PUBLIC  _fmemalloc_
_fmemalloc_:
        mov     ax,SYS_fmemalloc
        jmp     callsys

        PUBLIC  unlink_
unlink_:
        mov     ax,SYS_unlink
        jmp     callsys
