; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMultiBar
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mapedit.h"
LastPage=0

ClassCount=9
Class1=CChildFrame
Class2=CGLEnabledView
Class3=CMainFrame
Class4=CMapEditApp
Class5=CAboutDlg
Class6=CMapEditDoc
Class7=CMapEditView

ResourceCount=10
Resource1=IDD_TEST1
Resource2=IDR_MAPEDITYPE (English (U.S.))
Resource3=IDD_ABOUTBOX (English (U.S.))
Resource4=IDD_DIALOGBAR (English (U.S.))
Resource5=IDR_MAINFRAME (English (U.S.))
Class8=CMultiBar
Resource6=IDD_TRASHHEAP
Resource7=IDD_TEST2
Resource8=IDR_TOOLBAR (English (U.S.))
Resource9=IDD_MULTIBAR (English (U.S.))
Class9=CLayerList
Resource10=IDD_LAYER_LIST_DIALOG

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CGLEnabledView]
Type=0
BaseClass=CView
HeaderFile=GLEnabledView.h
ImplementationFile=GLEnabledView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CMainFrame

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

[CLS:CMapEditView]
Type=0
BaseClass=CGLEnabledView
HeaderFile=MapEditView.h
ImplementationFile=MapEditView.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

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

[DLG:IDD_TRASHHEAP]
Type=1
Class=?
ControlCount=1
Control1=IDC_TOOLBAR_COMBO,combobox,1344340226

[DLG:IDD_DIALOGBAR (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CHECK1,button,1342242819
Control3=IDC_RADIO1,button,1342177289

[CLS:CMultiBar]
Type=0
HeaderFile=multibar.h
ImplementationFile=multibar.cpp
BaseClass=CDialogBar
LastObject=CMultiBar
Filter=D
VirtualFilter=dWC

[DLG:IDD_MULTIBAR (English (U.S.))]
Type=1
Class=CMultiBar
ControlCount=0

[DLG:IDD_TEST1]
Type=1
Class=?
ControlCount=9
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT1,edit,1350631552
Control3=IDC_EDIT2,edit,1350631552
Control4=IDC_EDIT3,edit,1350631552
Control5=IDC_EDIT4,edit,1350631552
Control6=IDC_COMBO1,combobox,1344340226
Control7=IDC_COMBO2,combobox,1344340226
Control8=IDC_COMBO3,combobox,1344340226
Control9=IDC_COMBO4,combobox,1344340226

[DLG:IDD_TEST2]
Type=1
Class=?
ControlCount=16
Control1=IDC_CHECK1,button,1342242819
Control2=IDC_CHECK2,button,1342242819
Control3=IDC_CHECK3,button,1342242819
Control4=IDC_CHECK4,button,1342242819
Control5=IDC_CHECK5,button,1342242819
Control6=IDC_CHECK6,button,1342242819
Control7=IDC_CHECK7,button,1342242819
Control8=IDC_CHECK8,button,1342242819
Control9=IDC_CHECK9,button,1342242819
Control10=IDC_CHECK10,button,1342242819
Control11=IDC_CHECK11,button,1342242819
Control12=IDC_CHECK12,button,1342242819
Control13=IDC_CHECK13,button,1342242819
Control14=IDC_CHECK14,button,1342242819
Control15=IDC_CHECK15,button,1342242819
Control16=IDC_CHECK16,button,1342242819

[DLG:IDD_LAYER_LIST_DIALOG]
Type=1
Class=CLayerList
ControlCount=1
Control1=IDC_LAYER_LIST,listbox,1352728915

[CLS:CLayerList]
Type=0
HeaderFile=LayerList.h
ImplementationFile=LayerList.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CLayerList

