; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CListBox
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mapedit.h"
LastPage=0

ClassCount=9
Class1=CChildFrame
Class2=CGLEnabledView
Class3=CParamBar
Class4=CMainFrame
Class5=CMapEditApp
Class6=CAboutDlg
Class7=CMapEditDoc
Class8=CMapEditView

ResourceCount=8
Resource1=IDR_MAPEDITYPE (English (U.S.))
Resource2=IDD_TILEBAR (English (U.S.))
Resource3=IDD_LAYERBAR1 (English (U.S.))
Resource4=IDR_MAINFRAME (English (U.S.))
Resource5=IDD_TRASHHEAP
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDR_TOOLBAR (English (U.S.))
Class9=CCheckList
Resource8=IDD_PARAMBAR (English (U.S.))

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
LastObject=CChildFrame

[CLS:CGLEnabledView]
Type=0
BaseClass=CView
HeaderFile=GLEnabledView.h
ImplementationFile=GLEnabledView.cpp
LastObject=CGLEnabledView

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame
Filter=M
VirtualFilter=fWC

[CLS:CMapEditApp]
Type=0
BaseClass=CWinApp
HeaderFile=MapEdit.h
ImplementationFile=MapEdit.cpp
LastObject=CMapEditApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MapEdit.cpp
ImplementationFile=MapEdit.cpp
LastObject=CAboutDlg

[CLS:CMapEditDoc]
Type=0
BaseClass=CDocument
HeaderFile=MapEditDoc.h
ImplementationFile=MapEditDoc.cpp
LastObject=CMapEditDoc
Filter=N
VirtualFilter=DC

[CLS:CMapEditView]
Type=0
BaseClass=CGLEnabledView
HeaderFile=MapEditView.h
ImplementationFile=MapEditView.cpp
LastObject=ID_NEWMAP_OK
Filter=C
VirtualFilter=VWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAPEDITYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT
CommandCount=21

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TILEBAR (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_TILETOOLBAR_COMBO,combobox,1344340227

[DLG:IDD_LAYERBAR1 (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_LAYERBAR_LIST,listbox,1352728833

[TB:IDR_TOOLBAR (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
Command9=ID_TOOLBAR_PARAMBAR
Command10=ID_TOOLBAR_TILEPALETTE
Command11=ID_TOOLBAR_COMBO
CommandCount=11

[DLG:IDD_TRASHHEAP]
Type=1
Class=?
ControlCount=1
Control1=IDC_TOOLBAR_COMBO,combobox,1344340226

[DLG:IDD_PARAMBAR (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_PARAMBAR_LAYER_LIST,listbox,1352728833

[CLS:CParamBar]
Type=0
HeaderFile=ParamBar.h
ImplementationFile=ParamBar.cpp
BaseClass=CDialogBar
Filter=D
LastObject=IDC_PARAMBAR_LAYER_LIST
VirtualFilter=dWC

[CLS:CCheckList]
Type=0
HeaderFile=CheckList.h
ImplementationFile=CheckList.cpp
BaseClass=CListBox
Filter=W
LastObject=CCheckList

