/********************/
/*** Cmd Messages ***/
/********************/

#ifndef	__COMMAND_MESSAGES__
#define	__COMMAND_MESSAGES__

enum	CmdMsg
{
		CmdMsg_None=0,
// Core
		CmdMsg_ToggleSubView,		//  1
		CmdMsg_ToggleGrid,			//  2
		CmdMsg_Toggle2d,			//  3
		CmdMsg_ZoomIn,				//  4
		CmdMsg_ZoomOut,				//  5
		CmdMsg_ResetView,			//  6
		CmdMsg_SetLayer,			//  7
		CmdMsg_AddLayer,			//  8
		CmdMsg_DeleteLayer,			//  9

// Generic
		CmdMsg_SetMode,				// 10
		CmdMsg_Copy,				// 11
		CmdMsg_Paste,				// 12
		CmdMsg_SubViewLoad,			// 13
		CmdMsg_SubViewDelete,		// 14
		CmdMsg_SubViewUpdate,		// 15
		CmdMsg_SubViewSet,			// 16

// TileLayer/TileBank
		CmdMsg_MirrorX,				// 17
		CmdMsg_MirrorY,				// 18
		CmdMsg_SetColFlag,			// 19
		CmdMsg_ActiveBrushLeft,		// 20
		CmdMsg_ActiveBrushRight,	// 21

// Thing
		CmdMsg_ThingListDelete,		// 23
		CmdMsg_ThingListSelect,		// 24
		CmdMsg_ThingLevelSelect,	// 25
		CmdMsg_ThingPosSelect,		// 26
		CmdMsg_ThingPosUp,			// 27
		CmdMsg_ThingPosDown,		// 28
		CmdMsg_ThingPosDelete,		// 29
};
#endif
/*****************************************************************************/
