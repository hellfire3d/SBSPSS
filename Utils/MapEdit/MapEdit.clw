; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMapEditDoc
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MapEdit.h"
LastPage=0

ClassCount=7
Class1=CMapEditApp
Class2=CMapEditDoc
Class3=CMapEditView
Class4=CMainFrame

ResourceCount=10
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_MAPEDITYPE
Resource8=IDD_PROPPAGE_SMALL (English (U.S.))
Class5=CChildFrame
Class6=CAboutDlg
Resource4=IDR_MAINFRAME (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDR_MAPEDITYPE (English (U.S.))
Resource7=IDD_PROPPAGE_LARGE (English (U.S.))
Resource9=IDD_DIALOGBAR (English (U.S.))
Class7=CLayerBar
Resource10=IDD_LAYERBAR (English (U.S.))

[CLS:CMapEditApp]
Type=0
HeaderFile=MapEdit.h
ImplementationFile=MapEdit.cpp
Filter=N
LastObject=CMapEditApp

[CLS:CMapEditDoc]
Type=0
HeaderFile=MapEditDoc.h
ImplementationFile=MapEditDoc.cpp
Filter=N
LastObject=CMapEditDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CMapEditView]
Type=0
HeaderFile=MapEditView.h
ImplementationFile=MapEditView.cpp
Filter=C
LastObject=CMapEditView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=D
LastObject=CMainFrame
BaseClass=CMDIFrameWnd
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
LastObject=IDC_LAYERBAR_LIST


[CLS:CAboutDlg]
Type=0
HeaderFile=MapEdit.cpp
ImplementationFile=MapEdit.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
CommandCount=8
Command8=ID_APP_ABOUT

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
CommandCount=8
Command8=ID_APP_ABOUT

[MNU:IDR_MAPEDITYPE]
Type=1
Class=CMapEditView
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
CommandCount=21
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_APP_ABOUT

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
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
CommandCount=14
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE


[TB:IDR_MAINFRAME (English (U.S.))]
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
Command9=ID_TOOLBAR_LAYERBAR
Command10=ID_TOOLBAR_TILEPALETTE
CommandCount=10

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
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
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LAYERBAR (English (U.S.))]
Type=1
Class=CLayerBar
ControlCount=5
Control1=IDC_LAYERBAR_NEW,button,1342242816
Control2=IDC_LAYERBAR_DELETE,button,1342242816
Control3=IDC_LAYERBAR_UP,button,1342242816
Control4=IDC_LAYERBAR_DOWN,button,1342242816
Control5=IDC_LAYERBAR_LIST,listbox,1352728833

[DLG:IDD_PROPPAGE_SMALL (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_PROPPAGE_LARGE (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOGBAR (English (U.S.))]
Type=1
Class=?
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CUSTOM1,casd,1342242816

[CLS:CLayerBar]
Type=0
HeaderFile=LayerBar.h
ImplementationFile=LayerBar.cpp
BaseClass=CDialogBar
Filter=D
VirtualFilter=dWC
LastObject=CLayerBar

