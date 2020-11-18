# THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF 
# ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
# THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
# PARTICULAR PURPOSE.
#
# Copyright 2002 Microsoft Corporation.  All Rights Reserved.
#
#
# Processor independent makefile

# Nmake macros for building makefile
!include <win32.mak>

PROJ = CARDFILE

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"
    
OBJS= \
		$(OUTDIR)\asmport.obj \
                $(OUTDIR)\indb.obj \
                $(OUTDIR)\card.obj \
                $(OUTDIR)\dial.obj \
                $(OUTDIR)\dragdrop.obj \
                $(OUTDIR)\file.obj \
                $(OUTDIR)\find.obj \
                $(OUTDIR)\fixmenu.obj \
                $(OUTDIR)\index.obj \
                $(OUTDIR)\input.obj \
                $(OUTDIR)\init.obj \
                $(OUTDIR)\insert.obj \
                $(OUTDIR)\main.obj \
                $(OUTDIR)\object.obj \
                $(OUTDIR)\page.obj \
                $(OUTDIR)\picture.obj \
                $(OUTDIR)\print.obj \
                $(OUTDIR)\read.obj \
                $(OUTDIR)\register.obj \
                $(OUTDIR)\special.obj \
                $(OUTDIR)\write.obj 

.cpp{$(OUTDIR)}.obj:
        $(cc) $(cdebug) $(cflags) $(cvarsmt) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\"  $**

$(OUTDIR)\$(PROJ).res: messages\usa\$(PROJ).rc 
    $(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\$(PROJ).res /Imessages\usa messages\usa\$(PROJ).rc
    
$(OUTDIR)\$(PROJ).exe: $(OBJS) $(OUTDIR)\$(PROJ).res
        $(link) $(linkdebug) $(guiflags) -out:$(OUTDIR)\$(PROJ).exe $(OBJS) $(OUTDIR)\$(PROJ).res 

clean:
        $(CLEANUP)

test:
!message $(LINK32_OBJS)
