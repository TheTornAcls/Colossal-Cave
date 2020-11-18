# Nmake macros for building Windows 32-Bit apps

!include <Win32.Mak>

all: $(OUTDIR) $(OUTDIR)\heap.exe

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

$(OUTDIR)\heap.obj: heap.cpp
    $(cc) $(cflags) $(cvars) /WX /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" heap.cpp

$(OUTDIR)\heap.exe: $(OUTDIR)\heap.obj
    $(link) $(conflags) -out:$(OUTDIR)\heap.exe $(OUTDIR)\heap.obj $(conlibs)

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)
