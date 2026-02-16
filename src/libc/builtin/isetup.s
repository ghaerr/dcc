; ELKS system call library and startup code for DCC C88 (DeSmet C)
; Must be kept synchronized with elks/arch/i86/kernel/syscall.dat
; PART I
;
; 23 Nov 24 Greg Haerr
; 26 Dec 24 Added C startup code for argc/argv and exit return
; 14 Feb 26 Ported C86 syscall.s to C88 isetup.s
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
        CSEG                    ; first text seg
        nop                     ; prevent text having address 0 for SIG_DFL,SIG_IGN
        nop
        nop
        nop

        DSEG                    ; first data seg
        dw      0,0             ; prevent data having address 0
errno_:     dw      0
environ_:   dw      0
__stacklow_:dw      0
        ;.comm   ___argc,2
        ;.comm   ___argv,2
        ;.comm   ___program_filename,2

        CSEG
        PUBLIC  _CSETUP,main_,exit_,_exit_
_CSETUP:                        ; C program entry point
        mov     ax,sp
        sub     ax,dx           ; DX is stack size
        mov     __stacklow_,ax
        pop     ax              ; get argc
        ;mov     [___argc],ax
        mov     bx,sp
        ;mov     [___argv],bx
        mov     dx,bx           ; save argc in DX
L1:     cmp     WORD [bx],1
        inc     bx
        inc     bx
        jnc     L1
        mov     environ_,bx
        ;mov     bx,sp
        ;mov     bx,[bx]
        ;mov     [___program_filename], bx
        ;push    [___argv]
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

        PUBLIC  _exit
        PUBLIC  __exit
exit_:                          ; C exit temp comes here
_exit_:                         ; _exit
        mov     ax,1
        jmp     callsys

        PUBLIC  read_
read_:
        mov     ax,3
        jmp     callsys

        PUBLIC  write_
write_:
        mov     ax,4
        jmp     callsys

        PUBLIC  open_
open_:
        mov     ax,5
        jmp     callsys

        PUBLIC  close_
close_:
        mov     ax,6
        jmp     callsys

        PUBLIC  _lseek_
_lseek_:
        mov     ax,19
        jmp     callsys

        PUBLIC  ioctl_
ioctl_:
        mov     ax,54
        jmp     callsys
