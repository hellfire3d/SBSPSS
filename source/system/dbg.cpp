/*=========================================================================

	dbg.cpp

	Author:		PKG
	Created: 
	Project:	
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "dbg.h"

#ifdef	DBG_ACTIVE

#ifndef __SYSTEM_VSPRINTF_H__
#include "system\vsprintf.h"
#endif


/*	Std Lib
	------- */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*
Available options are:
	DBG_OUTPUT_TO_DEBUGGER					Send output to the debug console
	DBG_OUTPUT_TO_LOG						Send output to an internal log
	DBG_MAX_MESSAGE_LENGTH	chars				Length of entries in this log
	DBG_LOG_LINES			lines				Number of lines in the log buffer
	DBG_SHOW_MESSAGE_ORIGIN					Prepend file and linenumber to messages
	DBG_FILENAME_LENGTH		chars				This many chars of the filename are shown
	DBG_DEFAULT_CHANNEL		channelflags	Channels to listen to by default
*/

#ifdef	__USER_charles__
	#define DBG_OUTPUT_TO_DEBUGGER
	#define DBG_SHOW_MESSAGE_ORIGIN
	#define	DBG_FILENAME_LENGTH		16
	#define DBG_DEFAULT_CHANNEL		DC_CHARLES|DC_SYSTEM|DC_SOUND
#elif	__USER_dave__
	#define DBG_OUTPUT_TO_DEBUGGER
	#define DBG_SHOW_MESSAGE_ORIGIN
	#define	DBG_FILENAME_LENGTH		16
	#define DBG_DEFAULT_CHANNEL		DC_DAVE|DC_SYSTEM|DC_SOUND
#elif	__USER_paul__
	#define DBG_OUTPUT_TO_DEBUGGER
	#define DBG_OUTPUT_TO_LOG
	#define	DBG_MAX_MESSAGE_LENGTH	256
	#define DBG_LOG_LINES			8
	#define DBG_SHOW_MESSAGE_ORIGIN
	#define	DBG_FILENAME_LENGTH		10
	#define DBG_DEFAULT_CHANNEL		DC_PAUL|DC_SYSTEM|DC_SOUND|DC_GUI
#else
	#define DBG_OUTPUT_TO_DEBUGGER
	#define DBG_DEFAULT_CHANNEL		DC_ALL
#endif



/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static DEBUG_CHANNEL_FLAG	s_activeChannelFlags=(DEBUG_CHANNEL_FLAG)(DBG_DEFAULT_CHANNEL);

static int					s_dbgTransientChannelFlags=DC_NONE;
static char					s_dbgTransientFilename[DBG_FILENAME_LENGTH+1]="\0";
static int					s_dbgTransientLine=0;

#ifdef DBG_OUTPUT_TO_LOG
static char					s_logLines[DBG_LOG_LINES][DBG_MAX_MESSAGE_LENGTH];
static int					s_logLineCount=0;
static int					s_logLinePtr=0;
#endif


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void __setActiveDbgChannels(DEBUG_CHANNEL_FLAG _channelFlags)
{
	s_activeChannelFlags=_channelFlags;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
DEBUG_CHANNEL_FLAG __getActiveDbgChannels()
{
	return s_activeChannelFlags;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Used by the__DBGMSG() macro
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void __setDbgChannels(DEBUG_CHANNEL_FLAG _channelFlags)
{
	s_dbgTransientChannelFlags=_channelFlags;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Used by the__DBGMSG() macro
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void __setDbgFilenameAndLine(const char *_filename,int _line)
{
	int	filenameLength;
	int	start,len;
	filenameLength=strlen(_filename);
	if(filenameLength<DBG_FILENAME_LENGTH)
	{
		for(int i=0;i<DBG_FILENAME_LENGTH-filenameLength;i++)
			s_dbgTransientFilename[i]=' ';
		start=0;
		len=filenameLength;
	}
	else
	{
		start=strlen(_filename)-DBG_FILENAME_LENGTH;
		len=DBG_FILENAME_LENGTH;
	}

	strncpy(s_dbgTransientFilename+DBG_FILENAME_LENGTH-len,_filename+start,len);
	s_dbgTransientLine=_line;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void __writeDbgMessage(const char *_format,...)
{
	if(s_dbgTransientChannelFlags&s_activeChannelFlags)
	{
		char		messageBuffer[DBG_MAX_MESSAGE_LENGTH+1];
		__va_list	va;
		int			start;//,len;
		
		__va_start(va,_format);
#ifdef DBG_SHOW_MESSAGE_ORIGIN
		sprintf(messageBuffer,"%s:%04d ",s_dbgTransientFilename,s_dbgTransientLine);
		start=strlen(messageBuffer);
//		len=DBG_MAX_MESSAGE_LENGTH-start;
#else
		start=0;
//		len=DBG_MAX_MESSAGE_LENGTH;
#endif
		__vsprintf(messageBuffer+start,_format,va);
		__va_end(va);


#ifdef DBG_OUTPUT_TO_DEBUGGER
		printf("%s\n",messageBuffer);
#endif
#ifdef DBG_OUTPUT_TO_LOG
		strncpy(s_logLines[s_logLinePtr],messageBuffer,DBG_MAX_MESSAGE_LENGTH);
		s_logLinePtr=(s_logLinePtr+1)%DBG_LOG_LINES;
		if(s_logLineCount<DBG_LOG_LINES)s_logLineCount++;
#endif
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int __getNumberOfDbgLinesInLog()
{
#ifdef DBG_OUTPUT_TO_LOG
	return s_logLineCount;
#else
	return 0;
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
char *__getDbgLineFromLog(int _line)
{
#ifdef DBG_OUTPUT_TO_LOG
	return s_logLines[(s_logLinePtr+_line)%DBG_LOG_LINES];
#else
	return "Damn fool..";
#endif
}

#endif	/* DBG_ACTIVE */





#ifdef __VERSION_DEBUG__
#define OnScreenDebug
#include "gfx\font.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"

/*****************************************************************************/
void DoAssert( const char * Txt, const char * file, const int line )
{
	printf( "%s", Txt );

#ifdef	OnScreenDebug

	FontBank	F;
	char		Text[2048];


	sprintf( Text, "%s\nLine %d in %s", Txt, line, file );

	F.initialise( &standardFont );
	F.setJustification( FontBank::JUST_CENTRE );
	F.setOt( 0 );

	ClearNTag(OtPtr, MAX_OT);
	F.print( (512 >> 1), (256 >> 1), Text );

	VSync(0);
	VidSwapDraw();
	PrimDisplay();
	while(DrawSync(1));

	ClearNTag(OtPtr, MAX_OT);
	F.print( (512 >> 1), (256 >> 1), Text );

	VSync(0);
	VidSwapDraw();
	PrimDisplay();
	while(DrawSync(1));

	F.dump();

#endif
	PSYQpause();
}

/*****************************************************************************/
void	DbgPollHost()
{
#if		!defined(__USER_ART__)
	#if			__FILE_SYSTEM__==PC
		pollhost();
	#endif
#endif
}

#endif	/* __VERSION_DEBUG__ */





/*===========================================================================
 end */

