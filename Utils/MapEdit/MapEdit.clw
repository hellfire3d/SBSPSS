; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CGUILayerShade
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mapedit.h"
LastPage=0

ClassCount=24
Class1=CChildFrame
Class2=CGLEnabledView
Class3=CGUIAddLayer
Class4=CGUIElemList
Class5=CGUILayerActor
Class6=GUILayerCollision
Class7=CGUILayerList
Class8=CGUILayerPlatform
Class9=CGUILayerShade
Class10=CGUILayerThing
Class11=CGuiLayerThingPos
Class12=CGUIMultiBar
Class13=CGUINewMap
Class14=CGUIResize
Class15=CGUITileBank
Class16=GUIToolBar
Class17=CLayerTileCollision
Class18=CMainFrame
Class19=CMapEditApp
Class20=CAboutDlg
Class21=CMapEditDoc
Class22=CMapEditView

ResourceCount=20
Resource1=IDD_RESIZE
Resource2=IDD_NEWMAP
Resource3=IDD_TOOLBAR
Resource4=IDR_MAPEDITYPE (English (U.S.))
Resource5=IDD_LAYER_THING_POS
Resource6=IDD_LAYER_SHADE
Resource7=IDD_LAYER_ACTOR
Resource8=IDD_LAYER_LIST
Resource9=IDD_MULTIBAR (English (U.S.))
Resource10=IDD_LAYER_THING
Resource11=IDD_LAYER_TRIGGER
Resource12=IDD_ADDLAYER
Resource13=IDD_TILEBANK
Resource14=IDR_MAINFRAME (English (U.S.))
Resource15=IDD_ABOUTBOX (English (U.S.))
Resource16=IDD_LAYER_COLLISION
Resource17=IDD_ELEMLIST
Resource18=IDR_TOOLBAR (English (U.S.))
Class23=CGUILayerTrigger
Resource19=IDD_LAYER_PLATFORM
Class24=CGUILayerFX
Resource20=IDD_LAYER_FX

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

[CLS:CGUIAddLayer]
Type=0
BaseClass=CDialog
HeaderFile=GUIAddLayer.h
ImplementationFile=GUIAddLayer.cpp

[CLS:CGUIElemList]
Type=0
BaseClass=CDialog
HeaderFile=GUIElemList.h
ImplementationFile=GUIElemList.cpp

[CLS:CGUILayerActor]
Type=0
BaseClass=CDialog
HeaderFile=GUILayerActor.h
ImplementationFile=GUILayerActor.cpp

[CLS:GUILayerCollision]
Type=0
BaseClass=CDialog
HeaderFile=GUILayerCollision.h
ImplementationFile=GUILayerCollision.cpp

[CLS:CGUILayerList]
Type=0
BaseClass=CDialog
HeaderFile=GUILayerList.h
ImplementationFile=GUILayerList.cpp

[CLS:CGUILayerPlatform]
Type=0
BaseClass=CDialog
HeaderFile=GUILayerPlatform.h
ImplementationFile=GUILayerPlatform.cpp
Filter=D
VirtualFilter=dWC
LastObject=CGUILayerPlatform

[CLS:CGUILayerShade]
Type=0
BaseClass=CDialog
HeaderFile=GUILayerShade.h
ImplementationFile=GUILayerShade.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_LAYERSHADE_TRANS1

[CLS:CGUILayerThing]
Type=0
BaseClass=CDialog
HeaderFile=GUILayerThing.h
ImplementationFile=GUILayerThing.cpp

[CLS:CGuiLayerThingPos]
Type=0
BaseClass=CDialog
HeaderFile=GuiLayerThingPos.h
ImplementationFile=GuiLayerThingPos.cpp

[CLS:CGUIMultiBar]
Type=0
BaseClass=CDialogBar
HeaderFile=GUIMultibar.h
ImplementationFile=GUIMultibar.cpp

[CLS:CGUINewMap]
Type=0
BaseClass=CDialog
HeaderFile=GUINewMap.h
ImplementationFile=GUINewMap.cpp

[CLS:CGUIResize]
Type=0
BaseClass=CDialog
HeaderFile=GUIResize.h
ImplementationFile=GUIResize.cpp

[CLS:CGUITileBank]
Type=0
BaseClass=CDialog
HeaderFile=GUITileBank.h
ImplementationFile=GUITileBank.cpp

[CLS:GUIToolBar]
Type=0
HeaderFile=GUIToolBar.h
ImplementationFile=GUIToolBar.cpp

[CLS:CLayerTileCollision]
Type=0
BaseClass=CDialog
HeaderFile=LayerTileCollision.h
ImplementationFile=LayerTileCollision.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMapEditApp]
Type=0
BaseClass=CWinApp
HeaderFile=MapEdit.h
ImplementationFile=MapEdit.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=MapEdit.cpp
ImplementationFile=MapEdit.cpp

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

[DLG:IDD_ADDLAYER]
Type=1
Class=CGUIAddLayer
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_ADDLAYER_LIST,listbox,1352728833

[DLG:IDD_ELEMLIST]
Type=1
Class=CGUIElemList
ControlCount=1
Control1=IDD_ELEM_LIST,combobox,1344339971

[DLG:IDD_LAYER_ACTOR]
Type=1
Class=CGUILayerActor
ControlCount=14
Control1=IDC_ACTOR_SPEED_TEXT,static,1342308354
Control2=IDC_ACTOR_SPEED,edit,1350633600
Control3=IDC_ACTOR_SPEED_SPIN,msctls_updown32,1342177334
Control4=IDC_ACTOR_TURNRATE_TEXT,static,1342308354
Control5=IDC_ACTOR_TURNRATE,edit,1350633600
Control6=IDC_ACTOR_TURNRATE_SPIN,msctls_updown32,1342177334
Control7=IDC_ACTOR_HEALTH_TEXT,static,1342308354
Control8=IDC_ACTOR_HEALTH,edit,1350633600
Control9=IDC_ACTOR_HEALTH_SPIN,msctls_updown32,1342177334
Control10=IDC_ACTOR_ATTACK_TEXT,static,1342308354
Control11=IDC_ACTOR_ATTACK,edit,1350633600
Control12=IDC_ACTOR_ATTACK_SPIN,msctls_updown32,1342177334
Control13=IDC_ACTOR_PLAYER,button,1476461091
Control14=IDC_ACTOR_COLLISION,button,1342243363

[DLG:IDD_LAYER_COLLISION]
Type=1
Class=GUILayerCollision
ControlCount=8
Control1=IDC_LAYERCOLLISION_NORMAL,button,1342242816
Control2=IDC_LAYERCOLLISION_DAMAGE,button,1342242816
Control3=IDC_LAYERCOLLISION_SLIPPERY,button,1342242816
Control4=IDC_LAYERCOLLISION_ELECTRIC,button,1342242816
Control5=IDC_LAYERCOLLISION_STICKY,button,1342242816
Control6=IDC_LAYERCOLLISION_WATER,button,1342242816
Control7=IDC_LAYERCOLLISION_SOLID,button,1342242816
Control8=IDC_LAYERCOLLISION_DEATH,button,1342242816

[DLG:IDD_LAYER_LIST]
Type=1
Class=CGUILayerList
ControlCount=3
Control1=IDC_LAYERLIST_LIST,listbox,1352728913
Control2=IDC_LAYERLIST_ADD,button,1342242816
Control3=IDC_LAYERLIST_DELETE,button,1342242816

[DLG:IDD_LAYER_PLATFORM]
Type=1
Class=CGUILayerPlatform
ControlCount=9
Control1=IDC_PLATFORM_SPEED_TEXT,static,1342308354
Control2=IDC_PLATFORM_SPEED,edit,1350633600
Control3=IDC_PLATFORM_SPEED_SPIN,msctls_updown32,1342177334
Control4=IDC_PLATFORM_TURNRATE_TEXT,static,1342308354
Control5=IDC_PLATFORM_TURNRATE,edit,1350633600
Control6=IDC_PLATFORM_TURNRATE_SPIN,msctls_updown32,1342177334
Control7=IDC_PLATFORM_COLLISION,button,1342243363
Control8=IDC_PLATFORM_MOVE_TYPE,combobox,1344339971
Control9=IDC_PLATFORM_TYPE,combobox,1344339971

[DLG:IDD_LAYER_SHADE]
Type=1
Class=CGUILayerShade
ControlCount=28
Control1=IDC_LAYERSHADE_EDITR0,edit,1350770688
Control2=IDC_LAYERSHADE_EDITG0,edit,1350770816
Control3=IDC_LAYERSHADE_EDITB0,edit,1350770816
Control4=IDC_LAYERSHADE_COUNTTEXT,static,1342312449
Control5=IDC_LAYERSHADE_EDITR1,edit,1350770688
Control6=IDC_LAYERSHADE_EDITG1,edit,1350770816
Control7=IDC_LAYERSHADE_EDITB1,edit,1350770816
Control8=IDC_LAYERSHADE_EDITR2,edit,1350770688
Control9=IDC_LAYERSHADE_EDITG2,edit,1350770816
Control10=IDC_LAYERSHADE_EDITB2,edit,1350770816
Control11=IDC_LAYERSHADE_TEXT7,static,1342312449
Control12=IDC_LAYERSHADE_EDITR3,edit,1350770688
Control13=IDC_LAYERSHADE_EDITG3,edit,1350770816
Control14=IDC_LAYERSHADE_EDITB3,edit,1350770816
Control15=IDC_LAYERSHADE_COUNTEDIT,edit,1350576256
Control16=IDC_SPIN2,msctls_updown32,1342177302
Control17=IDC_LAYERSHADE_BACKGFX0,combobox,1344339971
Control18=IDC_LAYERSHADE_BACKGFX1,combobox,1344339971
Control19=IDC_LAYERSHADE_SPIN0,button,1342242851
Control20=IDC_LAYERSHADE_MOVE0,button,1342242851
Control21=IDC_LAYERSHADE_SCALE0,button,1342242851
Control22=IDC_LAYERSHADE_COLOR0,button,1342242851
Control23=IDC_LAYERSHADE_TRANS0,combobox,1344339971
Control24=IDC_LAYERSHADE_SPIN1,button,1342242851
Control25=IDC_LAYERSHADE_MOVE1,button,1342242851
Control26=IDC_LAYERSHADE_SCALE1,button,1342242851
Control27=IDC_LAYERSHADE_COLOR1,button,1342242851
Control28=IDC_LAYERSHADE_TRANS1,combobox,1344339971

[DLG:IDD_LAYER_THING]
Type=1
Class=CGUILayerThing
ControlCount=4
Control1=IDC_THING_LIST,combobox,1344339971
Control2=IDC_DEF_THING_LIST,combobox,1344339971
Control3=IDC_THING_GOTO,button,1342242816
Control4=IDC_THING_DELETE,button,1342242816

[DLG:IDD_LAYER_THING_POS]
Type=1
Class=CGuiLayerThingPos
ControlCount=4
Control1=IDC_THING_POS_LIST,listbox,1352728833
Control2=IDC_THING_POS_UP,button,1342242816
Control3=IDC_THING_POS_DOWN,button,1342242816
Control4=IDC_THING_POS_DELETE,button,1342242816

[DLG:IDD_MULTIBAR]
Type=1
Class=CGUIMultiBar

[DLG:IDD_NEWMAP]
Type=1
Class=CGUINewMap
ControlCount=6
Control1=IDOK,button,1342177281
Control2=IDCANCEL,button,1342177280
Control3=IDC_MAPSIZE_WIDTH_TEXT,static,1342308866
Control4=IDC_MAPSIZE_WIDTH,edit,1350639744
Control5=IDC_MAPSIZE_HEIGHT_TEXT,static,1342308866
Control6=IDC_MAPSIZE_HEIGHT,edit,1350639744

[DLG:IDD_RESIZE]
Type=1
Class=CGUIResize
ControlCount=7
Control1=IDOK,button,1342177281
Control2=IDCANCEL,button,1342177280
Control3=IDC_MAPSIZE_WIDTH_TEXT,static,1342308866
Control4=IDC_MAPSIZE_WIDTH,edit,1350639744
Control5=IDC_MAPSIZE_HEIGHT_TEXT,static,1342308866
Control6=IDC_MAPSIZE_HEIGHT,edit,1350639744
Control7=IDC_MAPSIZE_WARNING,static,1342308352

[DLG:IDD_TILEBANK]
Type=1
Class=CGUITileBank
ControlCount=3
Control1=IDD_TILEBANK_UPDATE,button,1342242816
Control2=IDD_TILEBANK_LOAD,button,1342242816
Control3=IDD_TILEBANK_DELETE,button,1342242816

[DLG:IDD_TOOLBAR]
Type=1
Class=GUIToolBar
ControlCount=2
Control1=IDD_LAYERTILE_BTN_PAINT,button,1342177344
Control2=IDD_LAYERTILE_BTN_SELECT,button,1342177344

[DLG:IDD_LAYERCOLLISION_GUI]
Type=1
Class=CLayerTileCollision

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
CommandCount=6

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_MAPEDITYPE (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_EXPORT
Command7=ID_RENDER_TO_TGA
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_RESET_VIEW
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_MAP_SETSIZE
Command18=ID_TOGGLE_GRID
Command19=ID_ZOOM_IN
Command20=ID_ZOOM_OUT
Command21=ID_MIRRORX
Command22=ID_MIRRORY
Command23=ID_ACTIVEBRUSH_LEFT
Command24=ID_ACTIVEBRUSH_RIGHT
Command25=ID_TOGGLE_SUBVIEW
Command26=ID_2D_3D_TOGGLE
Command27=ID_WINDOW_NEW
Command28=ID_WINDOW_CASCADE
Command29=ID_WINDOW_TILE_HORZ
Command30=ID_WINDOW_ARRANGE
Command31=ID_APP_ABOUT
CommandCount=31

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_RESET_VIEW
Command2=ID_ZOOM_IN
Command3=ID_ZOOM_OUT
Command4=ID_ACTIVEBRUSH_LEFT
Command5=ID_ACTIVEBRUSH_RIGHT
Command6=ID_2D_3D_TOGGLE
Command7=ID_EDIT_COPY
Command8=ID_TOGGLE_GRID
Command9=ID_FILE_NEW
Command10=ID_FILE_OPEN
Command11=ID_FILE_SAVE
Command12=ID_EDIT_PASTE
Command13=ID_ZOOM_IN
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_CUT
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_TOGGLE_SUBVIEW
Command19=ID_ZOOM_OUT
Command20=ID_NEXT_PANE
Command21=ID_PREV_PANE
Command22=ID_MIRRORX
Command23=ID_EDIT_CUT
Command24=ID_MIRRORY
Command25=ID_EDIT_UNDO
CommandCount=25

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889

[DLG:IDD_MULTIBAR (English (U.S.))]
Type=1
Class=?
ControlCount=0

[DLG:IDD_LAYER_TRIGGER]
Type=1
Class=CGUILayerTrigger
ControlCount=5
Control1=IDC_TRIGGER_SIZE,static,1342308354
Control2=IDC_TRIGGER_WIDTH,edit,1350633600
Control3=IDC_TRIGGER_WIDTH_SPIN,msctls_updown32,1342177334
Control4=IDC_TRIGGER_HEIGHT,edit,1350633600
Control5=IDC_PLATFORM_TURNRATE_SPIN,msctls_updown32,1342177334

[CLS:CGUILayerTrigger]
Type=0
HeaderFile=GUILayerTrigger.h
ImplementationFile=GUILayerTrigger.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CGUILayerTrigger

[DLG:IDD_LAYER_FX]
Type=1
Class=CGUILayerFX
ControlCount=8
Control1=IDC_FX_SPEED_TEXT,static,1342308354
Control2=IDC_FX_SIZE_TEXT,static,1342308354
Control3=IDC_FX_WIDTH,edit,1350633600
Control4=IDC_FX_WIDTH_SPIN,msctls_updown32,1342177334
Control5=IDC_FX_HEIGHT,edit,1350633600
Control6=IDC_FX_TURNRATE_SPIN,msctls_updown32,1342177334
Control7=IDC_FX_SPEED,edit,1350633600
Control8=IDC_FX_SPEED_SPIN,msctls_updown32,1342177462

[CLS:CGUILayerFX]
Type=0
HeaderFile=GUILayerFX.h
ImplementationFile=GUILayerFX.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_FX_WIDTH

