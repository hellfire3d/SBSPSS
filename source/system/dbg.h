/*=========================================================================

	dbg.h

	Author:		PKG
	Created:
	Project:	
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__SYSTEM_DBG_H__
#define	__SYSTEM_DBG_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Allow debug messaging
#if defined(__VERSION_DEBUG__)
#define DBG_ACTIVE
#endif


// Available channels
typedef enum
{
	DC_CHARLES		=1<<0,
	DC_DAVE			=1<<1,
	DC_PAUL			=1<<2,

	DC_SYSTEM		=1<<3,
	DC_SOUND		=1<<4,
	DC_GUI			=1<<5,
	DC_MEMCARD		=1<<6,


	DC_NONE			=0,
	DC_ALL			=-1,

} DEBUG_CHANNEL_FLAG;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

#ifdef DBG_ACTIVE
	void				__setActiveDbgChannels(DEBUG_CHANNEL_FLAG _channelFlags);
	DEBUG_CHANNEL_FLAG	__getActiveDbgChannels();

	void				__setDbgChannels(DEBUG_CHANNEL_FLAG _channelFlags);
	void				__setDbgFilenameAndLine(const char *_filename,int _line);
	void				__writeDbgMessage(const char *_format,...);

	int					__getNumberOfDbgLinesInLog();
	char				*__getDbgLineFromLog(int _line);

	#define __DBGMSG(channel)	__setDbgChannels(channel);__setDbgFilenameAndLine(__FILE__,__LINE__);__writeDbgMessage
#endif




#ifdef DBG_ACTIVE
	#define CHARLES_DBGMSG						__DBGMSG(DC_CHARLES)
	#define DAVE_DBGMSG							__DBGMSG(DC_DAVE)
	#define PAUL_DBGMSG							__DBGMSG(DC_PAUL)
	#define SYSTEM_DBGMSG						__DBGMSG(DC_SYSTEM)
	#define SOUND_DBGMSG						__DBGMSG(DC_SOUND)
	#define GUI_DBGMSG							__DBGMSG(DC_GUI)
	#define MEMCARD_DBGMSG						__DBGMSG(DC_MEMCARD)

	#define setActiveDbgChannels(_channelFlags)	__setActiveDbgChannels(_channelFlags)
	#define getActiveDbgChannels()				__getActiveDbgChannels()

	#define getNumberOfDbgLinesInLog()			__getNumberOfDbgLinesInLog()
	#define getDbgLineFromLog(_line)			__getDbgLineFromLog(_line)
#else
	#define CHARLES_DBGMSG						;
	#define DAVE_DBGMSG							;
	#define PAUL_DBGMSG							;
	#define SYSTEM_DBGMSG						;
	#define SOUND_DBGMSG						;
	#define GUI_DBGMSG							;
	#define MEMCARD_DBGMSG						;

	#define setActiveDbgChannels(_channelFlags)	;
	#define getActiveDbgChannels()				;

	#define getNumberOfDbgLinesInLog()			0
	#define getDbgLineFromLog(_line)			NULL
#endif


#if defined(__VERSION_DEBUG__)
	void	DbgPollHost();
	void	DoAssert(const char * Txt, const char * file, const int line);
	#define	ASSERT(x)							if (!(x)) DoAssert(#x,__FILE__,__LINE__);
#else
#define	DbgPollHost()							;
	#define	ASSERT(x)							;
#endif


/*---------------------------------------------------------------------- */

#endif	/* __SYSTEM_DBG_H__ */

/*===========================================================================
 end */