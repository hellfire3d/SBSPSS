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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /Gi- /GX /O2 /I "include" /I "include\pc" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "include" /I "include\pc" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /D "__GL_DEBUG__" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "GLib - Win32 Release"
# Name "GLib - Win32 Debug"
# Begin Group "Core Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Source\Gal.c
# End Source File
# Begin Source File

SOURCE=Source\Gmain.c
# End Source File
# Begin Source File

SOURCE=Source\Gtimer.c
# End Source File
# Begin Source File

SOURCE=Source\Gutils.c
# End Source File
# Begin Source File

SOURCE=Source\Ll.c
# End Source File
# Begin Source File

SOURCE=Source\Objs.c
# End Source File
# Begin Source File

SOURCE=Source\Tasker.c
# End Source File
# Begin Source File

SOURCE=Source\Tick.c
# End Source File
# End Group
# Begin Group "Core Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Glib\Include\Gal.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gdebug.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gmain.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gsys.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gtimer.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gtimsys.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Gutils.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Ll.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Objs.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Tasker.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Tick.h
# End Source File
# End Group
# Begin Group "PC Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=Source\Pc\Dpanim.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Frame.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Ganim.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Gdebug.c
# End Source File
# Begin Source File

SOURCE=Source\Pc\Gfname.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Gobject.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Gstring.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Gsys.c
# End Source File
# Begin Source File

SOURCE=Source\Pc\Gtimsys.c
# End Source File
# Begin Source File

SOURCE=Source\Pc\Iff.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Ilbm.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Misc.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Niff.cpp
# End Source File
# Begin Source File

SOURCE=Source\Pc\Pal.cpp
# End Source File
# Begin Source File

SOURCE=Source\PC\REGEX.C
# End Source File
# Begin Source File

SOURCE=Source\Pc\tquant.cpp
# End Source File
# End Group
# Begin Group "PC Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Dpanim.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Frame.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Ganim.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Gfname.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Gobject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Gstring.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Iff.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Ilbm.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Misc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Mtypes.h
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Niff.hpp
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\Pal.hpp
# End Source File
# Begin Source File

SOURCE=..\..\GLIB\Include\PC\REGEX.H
# End Source File
# Begin Source File

SOURCE=..\..\Glib\Include\Pc\tquant.h
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

SOURCE=Source\PC\pcre\chartables.c
# End Source File
# Begin Source File

SOURCE=Source\PC\pcre\internal.h
# End Source File
# Begin Source File

SOURCE=Source\PC\pcre\maketables.c
# End Source File
# Begin Source File

SOURCE=Source\PC\pcre\pcre.c
# End Source File
# Begin Source File

SOURCE=..\..\GLIB\Include\PC\pcre.h
# End Source File
# Begin Source File

SOURCE=Source\PC\pcre\study.c
# End Source File
# End Group
# End Target
# End Project
