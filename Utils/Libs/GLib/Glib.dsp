# Microsoft Developer Studio Project File - Name="GLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=GLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Glib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Glib.mak" CFG="GLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GLib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "GLib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/tp2psx/glibdev/build/GLib", GXNAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lib\Release"
# PROP Intermediate_Dir "lib\Release"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR"Release/" /Fp"Release/Glib.pch" /YX /Fo"Release/" /Fd"Release/" /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\Glib.lib"

!ELSEIF  "$(CFG)" == "GLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "lib\debug"
# PROP Intermediate_Dir "lib\debug"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__GL_DEBUG__" /FR"debug/" /Fp"debug/Glib.pch" /YX /Fo"debug/" /Fd"debug/" /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"debug\Glib.lib"

!ENDIF 

# Begin Target

# Name "GLib - Win32 Release"
# Name "GLib - Win32 Debug"
# Begin Group "Core Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Gal.c
# End Source File
# Begin Source File

SOURCE=Gmain.c
# End Source File
# Begin Source File

SOURCE=Gtimer.c
# End Source File
# Begin Source File

SOURCE=Gutils.c
# End Source File
# Begin Source File

SOURCE=Ll.c
# End Source File
# Begin Source File

SOURCE=Objs.c
# End Source File
# Begin Source File

SOURCE=Tasker.c
# End Source File
# Begin Source File

SOURCE=Tick.c
# End Source File
# End Group
# Begin Group "Core Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Gal.h
# End Source File
# Begin Source File

SOURCE=Gdebug.h
# End Source File
# Begin Source File

SOURCE=Gmain.h
# End Source File
# Begin Source File

SOURCE=Gsys.h
# End Source File
# Begin Source File

SOURCE=Gtimer.h
# End Source File
# Begin Source File

SOURCE=Gtimsys.h
# End Source File
# Begin Source File

SOURCE=Gtypes.h
# End Source File
# Begin Source File

SOURCE=Gutils.h
# End Source File
# Begin Source File

SOURCE=Ll.h
# End Source File
# Begin Source File

SOURCE=Objs.h
# End Source File
# Begin Source File

SOURCE=Tasker.h
# End Source File
# Begin Source File

SOURCE=Tick.h
# End Source File
# End Group
# Begin Group "PC Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Dpanim.cpp
# End Source File
# Begin Source File

SOURCE=Frame.cpp
# End Source File
# Begin Source File

SOURCE=Ganim.cpp
# End Source File
# Begin Source File

SOURCE=Gdebug.c
# End Source File
# Begin Source File

SOURCE=Gfname.cpp
# End Source File
# Begin Source File

SOURCE=Gobject.cpp
# End Source File
# Begin Source File

SOURCE=Gstring.cpp
# End Source File
# Begin Source File

SOURCE=Gsys.c
# End Source File
# Begin Source File

SOURCE=Gtimsys.c
# End Source File
# Begin Source File

SOURCE=Iff.cpp
# End Source File
# Begin Source File

SOURCE=Ilbm.cpp
# End Source File
# Begin Source File

SOURCE=Misc.cpp
# End Source File
# Begin Source File

SOURCE=Niff.cpp
# End Source File
# Begin Source File

SOURCE=Pal.cpp
# End Source File
# Begin Source File

SOURCE=REGEX.C
# End Source File
# Begin Source File

SOURCE=tquant.cpp
# End Source File
# End Group
# Begin Group "PC Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Dpanim.hpp
# End Source File
# Begin Source File

SOURCE=Frame.hpp
# End Source File
# Begin Source File

SOURCE=Ganim.hpp
# End Source File
# Begin Source File

SOURCE=Gfname.hpp
# End Source File
# Begin Source File

SOURCE=Gobject.hpp
# End Source File
# Begin Source File

SOURCE=Gstring.hpp
# End Source File
# Begin Source File

SOURCE=Iff.hpp
# End Source File
# Begin Source File

SOURCE=Ilbm.hpp
# End Source File
# Begin Source File

SOURCE=Misc.hpp
# End Source File
# Begin Source File

SOURCE=Mtypes.h
# End Source File
# Begin Source File

SOURCE=Niff.hpp
# End Source File
# Begin Source File

SOURCE=Pal.hpp
# End Source File
# Begin Source File

SOURCE=REGEX.H
# End Source File
# Begin Source File

SOURCE=tquant.h
# End Source File
# End Group
# Begin Group "Template"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Template\Template.cpp
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\..\Template\Template.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# End Group
# Begin Group "PCRE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=chartables.c
# End Source File
# Begin Source File

SOURCE=internal.h
# End Source File
# Begin Source File

SOURCE=maketables.c
# End Source File
# Begin Source File

SOURCE=pcre.c
# End Source File
# Begin Source File

SOURCE=pcre.h
# End Source File
# Begin Source File

SOURCE=study.c
# End Source File
# End Group
# End Target
# End Project
