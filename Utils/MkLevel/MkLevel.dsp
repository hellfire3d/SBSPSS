# Microsoft Developer Studio Project File - Name="MkLevel" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=MkLevel - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MkLevel.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MkLevel.mak" CFG="MkLevel - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MkLevel - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MkLevel - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Utils/MkLevel", JTGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MkLevel - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "..\libs\glib" /I "..\libs\maths" /I "..\libs\davelib" /I "..\libs\ginlib" /I "..\libs\texgrab" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glib.lib ginlib.lib texgrab.lib maths.lib /nologo /subsystem:console /machine:I386 /out:"..\..\tools\data\bin\MkLevel.exe" /libpath:"..\libs\psxlib\release" /libpath:"..\libs\ginlib\release" /libpath:"..\libs\glib\release" /libpath:"..\libs\davelib\release" /libpath:"..\libs\texgrab\release" /libpath:"..\libs\maths\release"

!ELSEIF  "$(CFG)" == "MkLevel - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /Gi /GX /ZI /Od /I "..\libs\glib" /I "..\libs\maths" /I "..\libs\davelib" /I "..\libs\ginlib" /I "..\libs\psxlib" /I "..\libs\texgrab" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr /YX /FD /GZ /c
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib glib.lib ginlib.lib texgrab.lib maths.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\libs\ginlib\debug" /libpath:"..\libs\glib\debug" /libpath:"..\libs\davelib\debug" /libpath:"..\libs\texgrab\debug" /libpath:"..\libs\maths\debug"

!ENDIF 

# Begin Target

# Name "MkLevel - Win32 Release"
# Name "MkLevel - Win32 Debug"
# Begin Group "Layers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Layers\MkLevelLayer.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayer3d.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayer3d.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerActor.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerActor.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerCollision.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerCollision.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerFX.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerFX.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerHazard.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerHazard.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerItem.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerItem.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerPlatform.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerPlatform.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerRGB.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerRGB.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerShade.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerShade.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerThing.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerThing.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerTile.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerTile.h
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerTrigger.cpp
# End Source File
# Begin Source File

SOURCE=.\Layers\MkLevelLayerTrigger.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\MkLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\MkLevel.h
# End Source File
# Begin Source File

SOURCE=..\..\tools\Data\bin\MkLevel.ini
# End Source File
# Begin Source File

SOURCE=.\pak.cpp
# End Source File
# Begin Source File

SOURCE=.\pak.h
# End Source File
# End Target
# End Project
