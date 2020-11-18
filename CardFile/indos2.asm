        title   indos2.asm

;************************************************************************/
;*                                                              	*/
;*  Windows Cardfile - Written by Mark Cliggett                 	*/
;*  (c) Copyright Microsoft Corp. 1985, 1991 - All Rights Reserved      */
;*                                                              	*/
;************************************************************************/
memM = 1
?WIN = 1 
?PLM = 1


.xlist
include cmacros.inc
.list

createSeg   _INPUT,REPMOV,byte,public,CODE

sBegin  DATA
sEnd    DATA

sBegin  REPMOV

assumes CS,REPMOV
assumes DS,DATA

cProc   RepMov,<PUBLIC,FAR>,<di,si>
        parmD   lpDest
        parmD   lpSrc
        parmW   cnt
cBegin
        push    ds
        cld
        les     di,lpDest
        lds     si,lpSrc
        mov     cx,cnt
        rep     movsb
        pop     ds
cEnd

cProc   RepMovDown,<PUBLIC,FAR>,<di,si>
        parmD   lpDest
        parmD   lpSrc
        parmW   cnt
cBegin
        push    ds
        std
        les     di,lpDest
        lds     si,lpSrc
        mov     cx,cnt
        add     si,cx
        add     di,cx
        dec     si
        dec     di
        rep     movsb
        cld
        pop     ds
cEnd

sEnd    REPMOV

end
