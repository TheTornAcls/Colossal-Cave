# Nmake macros for building Win32 apps
!include <win32.mak>

PROJ = LCD

all: $(OUTDIR) $(OUTDIR)\$(PROJ).exe 

# Define project specific macros
PROJ_OBJS  = $(OUTDIR)\Main.obj

#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

# Inference rule for updating the object files
.cpp{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvars) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

# Build rule for EXE
$(OUTDIR)\$(PROJ).EXE: $(BASE_OBJS) $(PROJ_OBJS)
    $(link) $(linkdebug) $(conlflags) \
    $(BASE_OBJS) $(PROJ_OBJS) $(conlibs) \
    -out:$(OUTDIR)\$(PROJ).exe 


# Rules for cleaning out those old files
clean:
        $(CLEANUP)
