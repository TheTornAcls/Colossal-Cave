# Microsoft Developer Studio Generated NMAKE File, Based on ADVENTURE.dsp
!IF "$(CFG)" == ""
CFG=ADVENTURE - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ADVENTURE - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ADVENTURE - Win32 Release" && "$(CFG)" != "ADVENTURE - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ADVENTURE.mak" CFG="ADVENTURE - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ADVENTURE - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ADVENTURE - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ADVENTURE - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\ADVENTURE.exe"


CLEAN :
	-@erase "$(INTDIR)\ADVENT.OBJ"
	-@erase "$(INTDIR)\DATABASE.OBJ"
	-@erase "$(INTDIR)\ENGLISH.OBJ"
	-@erase "$(INTDIR)\ITVERB.OBJ"
	-@erase "$(INTDIR)\TURN.OBJ"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\VERB.OBJ"
	-@erase "$(OUTDIR)\ADVENTURE.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ADVENTURE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ADVENTURE.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\ADVENTURE.pdb" /machine:I386 /out:"$(OUTDIR)\ADVENTURE.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ADVENT.OBJ" \
	"$(INTDIR)\DATABASE.OBJ" \
	"$(INTDIR)\ENGLISH.OBJ" \
	"$(INTDIR)\ITVERB.OBJ" \
	"$(INTDIR)\TURN.OBJ" \
	"$(INTDIR)\VERB.OBJ"

"$(OUTDIR)\ADVENTURE.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ADVENTURE - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\ADVENTURE.exe"


CLEAN :
	-@erase "$(INTDIR)\ADVENT.OBJ"
	-@erase "$(INTDIR)\DATABASE.OBJ"
	-@erase "$(INTDIR)\ENGLISH.OBJ"
	-@erase "$(INTDIR)\ITVERB.OBJ"
	-@erase "$(INTDIR)\TURN.OBJ"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(INTDIR)\VERB.OBJ"
	-@erase "$(OUTDIR)\ADVENTURE.exe"
	-@erase "$(OUTDIR)\ADVENTURE.ilk"
	-@erase "$(OUTDIR)\ADVENTURE.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\ADVENTURE.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ  /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ADVENTURE.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\ADVENTURE.pdb" /debug /machine:I386 /out:"$(OUTDIR)\ADVENTURE.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ADVENT.OBJ" \
	"$(INTDIR)\DATABASE.OBJ" \
	"$(INTDIR)\ENGLISH.OBJ" \
	"$(INTDIR)\ITVERB.OBJ" \
	"$(INTDIR)\TURN.OBJ" \
	"$(INTDIR)\VERB.OBJ"

"$(OUTDIR)\ADVENTURE.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ADVENTURE.dep")
!INCLUDE "ADVENTURE.dep"
!ELSE 
!MESSAGE Warning: cannot find "ADVENTURE.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ADVENTURE - Win32 Release" || "$(CFG)" == "ADVENTURE - Win32 Debug"
SOURCE=.\ADVENT.C

"$(INTDIR)\ADVENT.OBJ" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DATABASE.C

"$(INTDIR)\DATABASE.OBJ" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ENGLISH.C

"$(INTDIR)\ENGLISH.OBJ" : $(SOURCE) "$(INTDIR)"


SOURCE=.\ITVERB.C

"$(INTDIR)\ITVERB.OBJ" : $(SOURCE) "$(INTDIR)"


SOURCE=.\TURN.C

"$(INTDIR)\TURN.OBJ" : $(SOURCE) "$(INTDIR)"


SOURCE=.\VERB.C

"$(INTDIR)\VERB.OBJ" : $(SOURCE) "$(INTDIR)"



!ENDIF 

