# Microsoft Developer Studio Project File - Name="spongebob project" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=spongebob project - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "spongebob project.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "spongebob project.mak" CFG="spongebob project - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "spongebob project - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "spongebob project - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/users/paul/spongebob project", TJCAAAAA"
# PROP Scc_LocalPath "."

!IF  "$(CFG)" == "spongebob project - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Cmd_Line "NMAKE /f "spongebob project.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "spongebob project.exe"
# PROP BASE Bsc_Name "spongebob project.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Cmd_Line "nmake /f "spongebob project.mak""
# PROP Rebuild_Opt "/a"
# PROP Target_File "spongebob project.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "spongebob project - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Cmd_Line "NMAKE /f "spongebob project.mak""
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "spongebob project.exe"
# PROP BASE Bsc_Name "spongebob project.bsc"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Cmd_Line "nmake /f "spongebob project.mak""
# PROP Rebuild_Opt "/a"
# PROP Target_File "spongebob project.exe"
# PROP Bsc_Name ""
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "spongebob project - Win32 Release"
# Name "spongebob project - Win32 Debug"

!IF  "$(CFG)" == "spongebob project - Win32 Release"

!ELSEIF  "$(CFG)" == "spongebob project - Win32 Debug"

!ENDIF 

# Begin Group "source"

# PROP Default_Filter ""
# Begin Group "fileio"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\fileio\cdfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\cdfile.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\fileio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\fileio.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\filetab.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\filetab.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\pcfile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\fileio\pcfile.h
# End Source File
# End Group
# Begin Group "frontend"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\frontend\credits.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\credits.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\demomode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\demomode.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\frontend.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\frontend.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\maintitl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\maintitl.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\options.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\options.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\scrollbg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\scrollbg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\start.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\frontend\start.h
# End Source File
# End Group
# Begin Group "game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\game\game.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\game\game.h
# End Source File
# End Group
# Begin Group "gfx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\gfx\animtex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\animtex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\bubicles.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\bubicles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\fader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\fader.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\fdata.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\fdata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\font.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\font.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\prim.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\prim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\sprbank.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\sprbank.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\tpage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gfx\tpage.h
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\gui\gbutton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gbutton.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gfactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gfactory.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gframe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gframe.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\greadout.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\greadout.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gsprite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gsprite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gtextbox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gtextbox.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gui.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\gui\gui.h
# End Source File
# End Group
# Begin Group "locale"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\locale\textdbase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\locale\textdbase.h
# End Source File
# End Group
# Begin Group "mem"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\mem\memory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\mem\memory.h
# End Source File
# End Group
# Begin Group "pad"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\pad\pads.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\pad\pads.h
# End Source File
# End Group
# Begin Group "paul"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\paul\paul.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\paul\paul.h
# End Source File
# End Group
# Begin Group "psxboot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\PsxBoot\psxboot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\PsxBoot\PSXBOOT.H
# End Source File
# End Group
# Begin Group "sound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\sound\cdxa.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\cdxa.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\sound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\sound.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\speech.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\spu.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\spu.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\xmplay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\sound\xmplay.h
# End Source File
# End Group
# Begin Group "system"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\system\dbg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\dbg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\except.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\except.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\except_a.mip
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\global.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\gp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\gp.mip
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\gstate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\gstate.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\gte.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\info.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\lnkopt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\lnkopt.mip
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\main.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\vid.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\vid.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\vsprintf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\system\vsprintf.h
# End Source File
# End Group
# Begin Group "utils"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\source\utils\cmxmacro.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\fixed.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\gpu.inc
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\gtemisc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\mathmip.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\mathmip.mip
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\mathtab.H
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\quat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\quatmip.mip
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\replace.h
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\replace.mip
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\sincos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\utils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\source\utils\utils.h
# End Source File
# End Group
# End Group
# Begin Group "makefiles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\makefile.gaz
# End Source File
# Begin Source File

SOURCE=..\..\..\makefile.gfx
# End Source File
# End Group
# Begin Group "data"

# PROP Default_Filter ""
# Begin Group "text database"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\data\translations\text.dat
# End Source File
# End Group
# Begin Group "scripts"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\data\scripts\defs.scr
# End Source File
# Begin Source File

SOURCE=..\..\..\data\scripts\test.scr
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\data\DataCache.scr
# End Source File
# End Group
# Begin Group "out"

# PROP Default_Filter ""
# Begin Group "USA"

# PROP Default_Filter ""
# Begin Group "include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\out\USA\include\BigLump.h
# End Source File
# Begin Source File

SOURCE=..\..\..\out\USA\include\frontend.h
# End Source File
# Begin Source File

SOURCE=..\..\..\out\USA\include\ingamefx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\out\USA\include\trans.h
# End Source File
# Begin Source File

SOURCE=..\..\..\out\USA\include\UIGfx.h
# End Source File
# End Group
# End Group
# End Group
# Begin Group "build"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\build\ccopt.mak
# End Source File
# Begin Source File

SOURCE=..\..\..\build\getuser.mak
# End Source File
# Begin Source File

SOURCE=..\..\..\build\globals.mak
# End Source File
# Begin Source File

SOURCE=..\..\..\build\lnk.mak
# End Source File
# Begin Source File

SOURCE=..\..\..\build\outdirs.mak
# End Source File
# Begin Source File

SOURCE=..\..\..\build\parse.mak
# End Source File
# End Group
# End Target
# End Project
