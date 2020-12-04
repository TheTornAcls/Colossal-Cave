# ADVENT : Colossal cave adventure
# MS-MAKE 4.07, MSC 5.1 Makefile
# 4 May 1991
#
# This file was modified slightly so it creates a FAMILY
# mode program that can run under both OS/2 and MS-DOS
# This was done by adding the -Fb option to the cl command.  
# You also need to have DOSCALLS.LIB and API.LIB in your current directory.  
# and the OS/2 MAKE, LINK, CL, and BIND programs in your path.
#
# Kevin Lowey - LOWEY@SASK.USask.CA - Kevin Lowey @ 1:140/43
# I did not write this program, I only converted it to a family application
# It appears to work, but if you have problems, take out the -Fb option,
# and re-compile it as a strictly OS/2 application
#
!include <win32.mak>

cflags=$(cdebug) $(cflags)

all: $(OUTDIR)\Advent0.exe $(OUTDIR)\advtext.h $(OUTDIR)\advent.exe

LINK32_OBJS= \
	$(OUTDIR)\advent.obj \
	$(OUTDIR)\database.obj \
	$(OUTDIR)\english.obj \
	$(OUTDIR)\itverb.obj \
	$(OUTDIR)\turn.obj \
	$(OUTDIR)\verb.obj 

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

$(OUTDIR)\Advent0.exe : $(OUTDIR) $(OUTDIR)\advent0.obj
    $(link) $(linkdebug) /PDB:$(OUTDIR)\advent0.pdb $(conflags) -out:$(OUTDIR)\advent0.exe "$(OUTDIR)\advent0.obj" $(conlibs)

$(OUTDIR)\advtext.h: $(OUTDIR)\advent0.exe advent1.txt advent2.txt advent3.txt advent4.txt
    copy *.txt $(OUTDIR)
    cd $(OUTDIR)
    advent0
    cd ..

SOURCE=.\advent0.c

$(OUTDIR)\advent0.obj : $(SOURCE) $(OUTDIR)
    $(cc) $(cdebug) $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd $(SOURCE)

.c{$(OUTDIR)}.obj:
    $(cc) $(cflags) $(cvars) /I$(OUTDIR) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

$(OUTDIR)\Advent.exe:  $(OUTDIR) $(OUTDIR)\advtext.h  $(LINK32_OBJS)
    $(link) $(linkdebug) $(conflags) /PDB:$(OUTDIR)\advent.pdb -out:$(OUTDIR)\advent.exe $(LINK32_OBJS) $(conlibs)

clean:
        $(CLEANUP)
