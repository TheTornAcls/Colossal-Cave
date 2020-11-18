        title   indos.asm

;************************************************************************/
;*                                                              	*/
;*  Windows Cardfile - Written by Mark Cliggett                 	*/
;*  (c) Copyright Microsoft Corp. 1985, 1991 - All Rights Reserved	*/
;*                                                              	*/
;************************************************************************/

; http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vccore/html/_core_porting_ms.2d.dos_system_calls.asp
memM = 1
?WIN = 1 
?PLM = 1

.xlist
include cmacros.inc
.list

EXTRN	DOS3Call:FAR		    ; Replacement function for INT 21H

createSeg   _FILE,nrfile,byte,public,CODE

sBegin  DATA
labelW  <PUBLIC,dosAX>
        globalB dosAL,?     ; Ax register.
        globalB dosAH,?
labelW  <PUBLIC,dosBX>
        globalB dosBL,?     ; Bx register.
        globalB dosBH,?
labelW  <PUBLIC,dosCX>
        globalB dosCL,?     ; Cx register.
        globalB dosCH,?
labelW  <PUBLIC,dosDX>
        globalB dosDL,?     ; Dx register.
        globalB dosDH,?
sEnd    DATA

sBegin  NRFILE

assumes CS,NRFILE
assumes DS,DATA

cProc   Frename,<PUBLIC,FAR>,<di>
    parmW src
    parmW dst
cBegin
        mov     dx,src
        push    ds
        pop     es
        mov     di,dst
        mov     ah,56h
;	int	21h		    ; Original INT 21H
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
	jc	renexit
        xor     ax,ax
renexit:
cEnd

;
; Fdelete (pch) - delete a file
;   returns 0 or -1
;

cProc   Fdelete,<PUBLIC,FAR>
    parmW src
cBegin
        mov     dx,src
        mov     ah,41h
;	int	21h		    ; Original INT 21H
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
        jc      rmexit
        xor     ax,ax
rmexit:
cEnd

cProc mylread,<PUBLIC,FAR>
    parmW fh
    parmD buf
    parmW count
cBegin
    push    ds
    mov     bx,fh
    lds     dx,buf
    mov     ah,3fh
    mov     cx,count
;	int	21h		    ; Original INT 21H
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
    jnc     lreaddone
    xor     ax,ax
lreaddone:
    pop     ds
cEnd

cProc myread,<PUBLIC,FAR>
    parmW fh
    parmW buf
    parmW count
cBegin
    mov     bx,fh
    mov     dx,buf
    mov     ah,3fh
    mov     cx,count
;	int	21h		    ; Original INT 21H
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
    jnc     readdone
    xor     ax,ax
readdone:
cEnd

cProc mylwrite,<PUBLIC,FAR>
    parmW   fh
    parmD   buf
    parmW   count
cBegin
    push    ds
    mov     bx,fh
    lds     dx,buf
    mov     ah,40h
    mov     cx,count
;	int	21h		    ; Original INT 21H
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
    jnc     lwritedone
    xor     ax,ax
lwritedone:
    pop     ds
cEnd

cProc mywrite,<PUBLIC,FAR>
    parmW   fh
    parmW   buf
    parmW   count
cBegin
    mov     bx,fh
    mov     dx,buf
    mov     ah,40h
    mov     cx,count
;	int	21h		    ; Original INT 21H
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
    jnc     writedone
    xor     ax,ax
writedone:
cEnd

cProc mylmul,<PUBLIC,FAR>
    parmW   int1
    parmW   int2
cBegin
    mov     ax,int1
    mov     dx,int2
    mul     dx
cEnd

cProc   DosCall <PUBLIC,FAR>,<di,si>
cBegin
        mov     ax,dosax
        mov     bx,dosbx
        mov     cx,doscx
        mov     dx,dosdx
        clc
	call	DOS3Call	    ; Supports Windows 3.0 and WLO
;        int     21h
Assumes ds,DATA
        mov     dosax,ax
        mov     dosbx,bx
        mov     doscx,cx
        mov     dosdx,dx
        mov     ax,0
        jnc     bye
        not     ax
bye:
cEnd

sEnd    NRFILE

end
