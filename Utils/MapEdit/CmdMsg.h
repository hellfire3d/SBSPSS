/********************/
/*** Cmd Messages ***/
/********************/

#ifndef	__COMMAND_MESSAGES__
#define	__COMMAND_MESSAGES__

enum	CmdMsg
{
		CmdMsg_None=0,
// Core
		CmdMsg_ToggleSubView,
		CmdMsg_ToggleGrid,
		CmdMsg_Toggle2d,
		CmdMsg_ZoomIn,
		CmdMsg_ZoomOut,
		CmdMsg_ResetView,
		CmdMsg_SetLayer,
		CmdMsg_AddLayer,
		CmdMsg_DeleteLayer,

// Generic
		CmdMsg_SetMode,
		CmdMsg_Copy,
		CmdMsg_Paste,
		CmdMsg_SubViewLoad,
		CmdMsg_SubViewDelete,
		CmdMsg_SubViewUpdate,
		CmdMsg_SubViewSet,
// TileLayer/TileBank
		CmdMsg_MirrorX,
		CmdMsg_MirrorY,
		CmdMsg_SetColFlag,
		CmdMsg_ActiveBrushLeft,
		CmdMsg_ActiveBrushRight,
};
#endif
/*****************************************************************************/
