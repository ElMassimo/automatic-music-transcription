# Microsoft Developer Studio Project File - Name="library" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=library - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "library.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "library.mak" CFG="library - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "library - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "library - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe

!IF  "$(CFG)" == "library - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "library - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GR /GX /Z7 /Od /I "..\..\\" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /TP /c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "library - Win32 Release"
# Name "library - Win32 Debug"
# Begin Source File

SOURCE=..\..\ga\GA1DBinStrGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GA2DBinStrGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GA3DBinStrGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GABaseGA.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GABin2DecGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\gabincvt.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GABinStr.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GADemeGA.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\gaerror.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAIncGA.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAListBASE.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAParameter.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAPopulation.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\garandom.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GARealGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAScaling.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GASelector.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GASimpleGA.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GASStateGA.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAStatistics.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GAStringGenome.C
# End Source File
# Begin Source File

SOURCE=..\..\ga\GATreeBASE.C
# End Source File
# End Target
# End Project
