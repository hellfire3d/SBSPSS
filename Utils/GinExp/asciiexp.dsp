# Microsoft Developer Studio Project File - Name="asciiexp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ASCIIEXP - WIN32 RELEASE
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "asciiexp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "asciiexp.mak" CFG="ASCIIEXP - WIN32 RELEASE"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "asciiexp - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "asciiexp - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/KatBox/Project/asciiexp", VHBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "asciiexp - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MD /W3 /GX /Od /I "..\..\..\include" /I "c:\3dsmax3_1\MaxSDK\INCLUDE" /I "c:\3dsmax3_1\cstudio\docs" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 KERNEL32.LIB USER32.LIB GDI32.LIB WINSPOOL.LIB COMDLG32.LIB ADVAPI32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB ODBC32.LIB ODBCCP32.LIB COMCTL32.LIB /nologo /base:"0X02C60000" /subsystem:windows /dll /incremental:yes /machine:I386 /out:"C:\3dsmax3_1\plugins\GinExp.dle"

!ELSEIF  "$(CFG)" == "asciiexp - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "c:\3dsmax3_1\MaxSDK\INCLUDE" /I "c:\3dsmax3_1\cstudio\docs" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 KERNEL32.LIB USER32.LIB GDI32.LIB WINSPOOL.LIB COMDLG32.LIB ADVAPI32.LIB SHELL32.LIB OLE32.LIB OLEAUT32.LIB UUID.LIB ODBC32.LIB ODBCCP32.LIB COMCTL32.LIB /nologo /base:"0X02C60000" /subsystem:windows /dll /incremental:no /debug /machine:I386 /out:"C:\3dsmax3_1\plugins\GinExp.dle"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "asciiexp - Win32 Release"
# Name "asciiexp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\asciiexp.def
# End Source File
# Begin Source File

SOURCE=.\asciiexp.rc
# End Source File
# Begin Source File

SOURCE=.\ExpBone.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpCamLight.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpFFD.cpp

!IF  "$(CFG)" == "asciiexp - Win32 Release"

!ELSEIF  "$(CFG)" == "asciiexp - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExpFileIo.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpFileIo.h
# End Source File
# Begin Source File

SOURCE=.\ExpMain.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpMaterial.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpModel.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpUProp.cpp
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\asciiexp.h
# End Source File
# Begin Source File

SOURCE=.\ffd.h
# End Source File
# Begin Source File

SOURCE=.\Triobjed.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Libraries"

# PROP Default_Filter "*.lib"
# Begin Source File

SOURCE=\3dsmax3_1\MaxSDK\LIB\CORE.LIB
# End Source File
# Begin Source File

SOURCE=\3dsmax3_1\MaxSDK\LIB\GEOM.LIB
# End Source File
# Begin Source File

SOURCE=\3dsmax3_1\MaxSDK\LIB\MESH.LIB
# End Source File
# Begin Source File

SOURCE=\3dsmax3_1\Maxsdk\Lib\Bmm.lib
# End Source File
# Begin Source File

SOURCE=\3dsmax3_1\Maxsdk\lib\maxutil.lib
# End Source File
# End Group
# End Target
# End Project
