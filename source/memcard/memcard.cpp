/*=========================================================================

	memcard.cpp

	Author:  Paul Grenfell @ Climax
	Created: Febuary 1999
	Project: Theme Park 2
	Purpose: Memory card access

	Copyright (c) 1999 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "memcard/memcard.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif


/*	Std Lib
	------- */

#include <sys\types.h>
#include <libmcrd.h>

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

#define	SAVENAMEHEADER		"SBSP: "

// for prlsr - use slot 1 only
#define USE_SLOT_ONE_ONLY




/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */


/*----------------------------------------------------------------------
	Vars
	---- */


int							MemCard::s_active=false;
						
int							MemCard::s_currentChannel;
int							MemCard::m_nextChannel;
MemCard::CMDTYPE			MemCard::s_currentCommand;
MemCard::CMDTYPE			MemCard::m_nextCommand;
						
int							MemCard::s_activeSlot=-1;
						
MemCard::CARDDEF			MemCard::s_cardData[MemCard::CARDSLOTSTOCHECK];
						
int							MemCard::s_file;
int							MemCard::s_blockCount;
char						MemCard::s_tempFileInfoBuffer[128];
						
long int					MemCard::s_syncStatus,MemCard::s_syncCmds,MemCard::s_syncResults;
						
void						*MemCard::s_bufPtr;
						
char						MemCard::s_userFileName[8+1];
char						MemCard::s_fname[20+1];
char						MemCard::s_fnameBase[40];

FileCallbackDef				MemCard::s_callbackFunction;





/*****************************************************************************/
/*** SJIS Conversion Stuff ***************************************************/
/*****************************************************************************/
struct SjisS
{
	char 			Ascii;
	unsigned char 	Num;
	unsigned short 	Sjis;
};

static struct SjisS	SjisTable[]=
{
	{'0',10, 0x824f },{'A',26, 0x8260 },{'a',26, 0x8281 },

	{' ', 1,0x8140},{'!', 1,0x8149},{'"', 1,0x8168},{'#', 1,0x8194},{'$', 1,0x8190},
	{'%', 1,0x8193},{'&', 1,0x8195},{'\'',1,0x8166},{'(', 1,0x8169},{')', 1,0x816a},
	{'*', 1,0x8196},{'+', 1,0x817b},{',', 1,0x8143},{'-', 1,0x817c},{'.', 1,0x8144},
	{'/', 1,0x815e},{':', 1,0x8146},{';', 1,0x8147},{'<', 1,0x8171},{'=', 1,0x8181},
	{'>', 1,0x8172},{'?', 1,0x8148},{'@', 1,0x8197},{'[', 1,0x816d},{'\\',1,0x818f},
	{']', 1,0x816e},{'^', 1,0x814f},{'_', 1,0x8151},{'`', 1,0x8165},{'{', 1,0x816f},
	{'|', 1,0x8162},{'}', 1,0x8170},{'~', 1,0x8150},{0,0,0}
};

/*****************************************************************************/
char 	MCToAscii(unsigned short Sjis)
{
	struct SjisS 	*Ptr = SjisTable;

	while( Ptr->Ascii )
	{
		if( ( Sjis >= Ptr->Sjis ) && ( Sjis < ( Ptr->Sjis + Ptr->Num ) ) )
		{
			/* Found SJIS code, return ASCII equivalent */
			return( Ptr->Ascii + ( Sjis - Ptr->Sjis ) );
		}
		Ptr++;
	}

	return '?';		// Unknown SJIS code
}
/*****************************************************************************/
unsigned short	MCToSjis(char Ascii)
{
	struct	SjisS 	*Ptr=SjisTable;

	while (Ptr->Ascii)
	{
		if ((Ascii>=Ptr->Ascii) && (Ascii<(Ptr->Ascii+Ptr->Num)))
		{
			/* Found ASCII char, return SJIS equivalent */
			return (Ptr->Sjis+(Ascii-Ptr->Ascii));
		}
		Ptr++;
	}

	return( 0x8148 ); 		// Unknow char, return '?'
}

/*****************************************************************************/
//	Returns:	Pointer to a non-persistant containing ascii test
static char *JIStoASCII( unsigned short *Sjis )
{
	unsigned short SwappedSjis[ 32+1 ];
	unsigned short *Src, *Dst;
	char *CDst;
	static char Ascii[ 64+1 ];


	Ascii[ 0 ] = '\0';

	if( Sjis[ 0 ] & 0x0080 )
	{
		Src = Sjis;
		Dst = SwappedSjis;
		while( *Src )
		{
			*Dst  = ( ( *Src & 0xff ) << 8 ) + ( *Src >> 8 );
			Src++;
			Dst++;
		}
		*Dst++ = 0;

		// Convert to ASCII
		Src = SwappedSjis;
		CDst = Ascii;
		while( *Src )
		{
			*CDst++ = MCToAscii( *Src++ );
		}
		*CDst++ = 0;
	}
	else
	{
		// Old style ASCII string - Just copy it across
//		strcpy( (char *)sjis, ascii );
		return( "ASCII" );
	}

	return( Ascii );
}
/*****************************************************************************/
static unsigned short *ASCIItoJIS( char *Str )
{
	static unsigned short SjisText[ 32+1 ];
	unsigned short *Sjis = SjisText;

	while( *Str )
	{
		if( ( Str[ 0 ] ) & 0x80 )
		{
			*Sjis++ = ( Str[ 0 ] << 8 ) | Str[ 1 ];
			Str += 2;
		}
		else
		{
			*Sjis++ = MCToSjis( *Str++ );
		}
	}

	// Byte swap
	unsigned short temp;
	for( int i = 0; i < 32; i++ )
	{
		temp = SjisText[ i ];
		SjisText[ i ] = ( ( temp & 0x00ff ) << 8 ) + ( ( temp & 0xff00 ) >> 8 );
	}


	*Sjis++=0; *Sjis++=0;
	return( SjisText );
}
/*****************************************************************************/
/*** SJIS Conversion Stuff ***************************************************/
/*****************************************************************************/






/*----------------------------------------------------------------------
	Function:	Start
	Purpose:	Starts up memory card services. MemCardInit() should have already been called.
	Params:		None
	Returns:	Success
  ---------------------------------------------------------------------- */
bool MemCard::Start( void )
{
	long int Dummy;


	// Don't allow the sytem to be activated twice!
	if( s_active == true )
	{
		return( false );
	}

	// Startup system memcard services
	MemCardStart();

	// Make sure that the system memcard interface is idle
	MemCardSync( 0, &Dummy, &Dummy );

	// Initialisation
	s_currentCommand=CmdNone;
	m_nextCommand=CmdNone;
	m_nextChannel=-1;

	InvalidateCard( 0 );
	InvalidateCard( 1 );
	s_currentChannel = 1;
	s_activeSlot = -1;
	CreateFName("\0");

	// It's alive!
	s_active = true;
//	MEMCARD_DBGMSG( "[PMC] PMC started" );
	return( true );
}



/*----------------------------------------------------------------------
	Function:	Stop
	Purpose:	Closes down memcard services. Call MemCardEnd() afterwards if you _really_ want to
				shut the sytem memcard services down.
	Params:		None
	Returns:	Success
  ---------------------------------------------------------------------- */
bool MemCard::Stop( void )
{
	long int Dummy;


	// Check that the system is active
	if( s_active != true )
	{
		return( false );
	}
	s_active = false;

	// Make sure that the system memcard interface is idle
	MemCardSync( 0, &Dummy, &Dummy );

	// Close down PSX memory card services
	MemCardStop();

	// Invalidate all data
	InvalidateCard( 0 );
	InvalidateCard( 1 );

	// Finished!
//	MEMCARD_DBGMSG( "[PMC] PMC ended" );
	return( true );
}



/*----------------------------------------------------------------------
	Function:	InvalidateCard
	Purpose:	Invalidates a card by clearing internal flags, also frees its memory
	Params:		Channel of card to invalidate
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::InvalidateCard( int Channel )
{
	// Clear data on the card
	s_cardData[ Channel ].m_cardStatus = CS_NoCard;
	s_cardData[ Channel ].m_slotStatus = SS_Idle;
	s_cardData[ Channel ].m_totalFileCount = 0;
	s_cardData[ Channel ].m_totalFreeBlocks = 0;
	s_cardData[ Channel ].m_nativeFileCount = 0;

MemCardClose();

	// Stop whatever command was in operation and set the system to idle, the existance
	// checks will then re-find this card if it is ok
	if( Channel == m_nextChannel/*PKGm_nextCommand*/ )
	{
		m_nextCommand = CmdNone;
		m_nextChannel = -1;
	}
	s_currentCommand = CmdNone;
	s_currentChannel = Channel;

	MEMCARD_DBGMSG( "[PMC] Invalidate card %d", Channel );
}



/*----------------------------------------------------------------------
	Function:	GetErrorString
	Purpose:	Returns a pointer to a string which describes a memcard error code
	Params:		Error code
	Returns:	Pointer to error string
  ---------------------------------------------------------------------- */
static const char *GetErrorString( int ErrorCode )
{
	switch( ErrorCode )
	{
		case McErrNone:
			return( "No error(!?)" );
			break;

		case McErrCardNotExist:
			return( "Not connected" );

		case McErrCardInvalid:
			return( "Bad card" );

		case McErrNewCard:
			return( "New card ( card was replaced )" );

		case McErrNotFormat:
			return( "Not formatted" );

		case McErrFileNotExist:
			return( "File not found" );

		case McErrAlreadyExist:
			return( "File already exists" );

		case McErrBlockFull:
			return( "Not enough memory blocks" );

		default:
			return( "UNKNOWN MEMCARD ERROR CODE!" );
	}
}



/*----------------------------------------------------------------------
	Function:	GetCardStatus
	Purpose:	Access function to a cards status
	Params:		Channel of card
	Returns:	Status
  ---------------------------------------------------------------------- */
MemCard::CARDSTATUS MemCard::GetCardStatus( int Channel )
{
	return( s_cardData[ Channel ].m_cardStatus );
}



/*----------------------------------------------------------------------
	Function:	GetSlotStatus
	Purpose:	Access function to a slots status
	Params:		Channel of slot
	Returns:	Status
  ---------------------------------------------------------------------- */
MemCard::SLOTSTATUS MemCard::GetSlotStatus( int Channel )
{
	return( s_cardData[ Channel ].m_slotStatus );
}



/*----------------------------------------------------------------------
	Function:	GetFileCountOnCard
	Purpose:	Access function that gives the number of files on a cardthat belong to this app
	Params:		Channel of card
	Returns:	Number of files on card
  ---------------------------------------------------------------------- */
int MemCard::GetFileCountOnCard( int Channel )
{
	if( s_cardData[ Channel ].m_cardStatus == CS_ValidCard )
	{
		return( s_cardData[ Channel ].m_nativeFileCount );
	}
	else
	{
		// If the card is not valid then return a file count of 0
		return( 0 );
	}
}



/*----------------------------------------------------------------------
	Function:	GetFreeBlocksOnCard
	Purpose:	Access function that gives the number of free blocks on a card
	Params:		Channel of card
	Returns:	Free block count
  ---------------------------------------------------------------------- */
int MemCard::GetFreeBlocksOnCard( int Channel )
{
	if( s_cardData[ Channel ].m_cardStatus == CS_ValidCard )
	{
		return( s_cardData[ Channel ].m_totalFreeBlocks );
	}
	else
	{
		// If the card is not valid then return a free block count of 0
		return( 0 );
	}
}



/*----------------------------------------------------------------------
	Function:	GetFileName
	Purpose:	Access function to return a files name
	Params:		Channel of card and native file number
	Returns:	A non-persistant pointer to ASCII filename or NULL on error
  ---------------------------------------------------------------------- */
const char *MemCard::GetFileName( int Channel, int File )
{
	if( File >= s_cardData[ Channel ].m_nativeFileCount )
	{
		// Woah, we don't have this many files
		return( NULL );
	}
	else
	{
		// Convert name from SJIS and return
		return( &JIStoASCII( s_cardData[ Channel ].m_nativeFileInfo[ File ].m_sjisName )[strlen(SAVENAMEHEADER)] );
	}
}



/*----------------------------------------------------------------------
	Function:	GetFileSizeInBlocks
	Purpose:	Access function to return the number of 8k blocks in a file
	Params:		Channel of card and file number
	Returns:    Number of blocks in file
  ---------------------------------------------------------------------- */
int MemCard::GetFileSizeInBlocks( int Channel, int File )
{
	if( File >= s_cardData[ Channel ].m_nativeFileCount )
	{
		// Woah, we don't have this many files
		return( 0 );
	}
	else
	{
		return( s_cardData[ Channel ].m_nativeFileInfo[ File ].m_blocks );
	}
}



/*----------------------------------------------------------------------
	Function:	FillHeaderDetails
	Purpose:	Takes a pointer to a file header and fills in filename and block count. This assumes
				that you have used Sonys PDATool.exe to generate the header
	Params:		Number of blocks to save
				ASCII filename ( max 32 chars )
	Returns:    None
  ---------------------------------------------------------------------- */
void MemCard::FillHeaderDetails( unsigned char *HeaderBase, int FileLength, char *Filename )
{
	int		BlockCount=((FileLength-1)/BLOCKSIZE)+1;
	char	realFilename[32+1];

	if( HeaderBase == NULL ||
		BlockCount < 1 || BlockCount > 15 ||
		strlen( Filename ) > 32-8 ||
		Filename[ 0 ] == '\0' )
	{
		// Damn fool!
		return;
	}
	sprintf(realFilename,"%s%s",SAVENAMEHEADER,Filename);

	// Fill in the details
	HeaderBase[ 3 ] = (char)BlockCount;
	memcpy( &HeaderBase[ 4 ], ASCIItoJIS( realFilename ), 64 );

	return;
}


/*----------------------------------------------------------------------
	Function:	ReadFile
	Purpose:	Starts file reading.
	Params:		Channel of card and file number
				Callback gets called when the file reading has finsihed
	Returns:	Success
  ---------------------------------------------------------------------- */
bool MemCard::ReadFile( int Channel, int File, void *Dest, FileCallbackDef Callback )
{
	// Check for valid input and that the card is idle etc...
	if( s_cardData[ Channel ].m_cardStatus != CS_ValidCard ||
		File < 0 || File >= s_cardData[ Channel ].m_nativeFileCount ||
		Callback == NULL )
	{
		return( false );
	}

	// Setup the load
	m_nextChannel = Channel;
	m_nextCommand = CmdReadFile;
	s_file = File;//s_cardData[ Channel ].NativeFileInfo[ File ].DirEntry;
	s_callbackFunction = Callback;
	s_bufPtr = Dest;

	VSync( 0 );

//	MEMCARD_DBGMSG( "[PMC] File read registered for card %d file %d", Channel, File );
	return( true );
}


/*----------------------------------------------------------------------
	Function:	WriteFile
	Purpose:	Starts file writing. If file supplied does not exist then a new one will be created first
	Params:		Channel of card and file number ( pass -1 as file number for new file )
				Source points to the source data
				Blocks says how many blocks to wirte ( 1 block = 8k )
				Callback gets called when the file writing has finsihed
	Returns:	Success
  ---------------------------------------------------------------------- */
bool MemCard::WriteFile( int Channel, int File, u8 *FName, void *Source, int FileLength, FileCallbackDef Callback )
{
	int Blocks=((FileLength-1)/BLOCKSIZE)+1;

	// Check for valid input and that the card is idle etc...
MEMCARD_DBGMSG("s:	%d",s_cardData[ Channel ].m_cardStatus);
	if( s_cardData[ Channel ].m_cardStatus != CS_ValidCard ||
		File < -1 || File >= s_cardData[ Channel ].m_nativeFileCount ||
		Blocks < 1 ||// Blocks > s_cardData[ Channel ].TotalFreeBlocks ||
		Callback == NULL )
	{
		return( false );
	}

	// Setup the write as the next command
	if( FName )
		strncpy( s_userFileName, (char*)FName, 8 );
	else
		s_userFileName[0]='\0';
	m_nextChannel = Channel;
	m_nextCommand = CmdWriteFile;
	s_file = File;
	s_callbackFunction = Callback;
	s_blockCount = Blocks;
	s_bufPtr = Source;

	VSync( 0 );

//	MEMCARD_DBGMSG( "[PMC] File write registered" );
	return( true );
}



/*----------------------------------------------------------------------
	Function:	FormatCard
	Purpose:	Starts file formatting. This will fail is the requested card is not unformatted
	Params:		Channel of card to format
				Callback gets called when the format has finsihed
	Returns:	Success
  ---------------------------------------------------------------------- */
bool MemCard::FormatCard( int Channel, FileCallbackDef Callback )
{
	// Check for valid input and that the card is idle etc...
	if(	s_cardData[ Channel ].m_cardStatus != CS_UnformattedCard ||
		Callback == NULL )
	{
//		MEMCARD_DBGMSG( "[PMC] Card format not registered" );
		return( false );
	}


	// Setup the format as the next command
	m_nextChannel = Channel;
	m_nextCommand = CmdFormatCard;
	s_callbackFunction = Callback;
	s_cardData[ Channel ].m_slotStatus = SS_Formatting;

//	MEMCARD_DBGMSG( "[PMC] Card format registered" );
	return( true );
}



/*----------------------------------------------------------------------
	Function:	DeleteFile
	Purpose:	Starts file delete.
	Params:		Channel of card and file number
				Callback gets called when the file writing has finsihed
	Returns:	Success
  ---------------------------------------------------------------------- */
bool MemCard::DeleteFile( int Channel, int File, FileCallbackDef Callback )
{
	// Check for valid input etc...
	if( s_cardData[ Channel ].m_cardStatus != CS_ValidCard ||
		File < 0 || File >= s_cardData[ Channel ].m_nativeFileCount ||
		Callback == NULL )
	{
		return( false );
	}

	// Setup the load as the next command
	m_nextChannel = Channel;
	m_nextCommand = CmdDeleteFile;
	strcpy( s_fname, s_cardData[ Channel ].m_totalDirEntry[ s_cardData[ Channel ].m_nativeFileInfo[ File ].m_dirEntry ].name );
	s_callbackFunction = Callback;
	s_cardData[ s_currentChannel ].m_slotStatus = SS_Writing;

//	MEMCARD_DBGMSG( "[PMC] File delete registered for card %d file %d", Channel, File );
	return( true );
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_None
	Purpose:	Card command handler for CmdNone
				In idle moments ( like these ) the memcard system checks both cards to
				see if they have been inserted/removed.
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_None( void )
{
	// System is idle?
	if( s_syncStatus != -1 )
	{
//		MEMCARD_DBGMSG( "[PMC] Not quite idle...!" );
		return;
	}

//	MEMCARD_DBGMSG( "[PMC] ..HandleCmd_None()" );

	if( m_nextCommand != CmdNone &&
		s_currentChannel == m_nextChannel )
	{
		// Execute buffered command
//		MEMCARD_DBGMSG( "[PMC] Set next command..  ss:%d ch:%d", (int)s_syncStatus, s_currentChannel );

		s_currentCommand = m_nextCommand;
		s_currentChannel = m_nextChannel;
		m_nextCommand = CmdNone;
		m_nextChannel = -1;
	}
	else
#ifdef USE_SLOT_ONE_ONLY
	{
		s_currentChannel=0;
 		s_currentCommand = CmdExist;
	}
#else
	{
		// Do checks that the cards are still there
//		MEMCARD_DBGMSG( "[PMC] check existance of other card.." );
		switch( s_activeSlot )
		{
			case 1:
				s_currentChannel=0;
				break;
			case 2:
				s_currentChannel=1;
				break;
			default:
				s_currentChannel^=1;
				break;
		}
 		s_currentCommand = CmdExist;
	}
#endif
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_Exist
	Purpose:	Card command handler for CmdExist
				If a valid card is found then it is left ( it has already been accepted ).
				If a new card is found then acceptance is started next frame.
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_Exist( void )
{
	if( s_syncStatus == -1 )
	{
		// No process active
		MemCardExist( s_currentChannel << 4 );
		s_cardData[ s_currentChannel ].m_slotStatus = SS_Scanning;
//		MEMCARD_DBGMSG( "[PMC] called MemCardExist()" );
	}
	else
	{
//		MEMCARD_DBGMSG( "[PMC] returned from MemCardExist()" );
		// Process just ended
		switch( s_syncResults )
		{
			// Card exists
			case McErrNone:
			case McErrNewCard:
				if( s_cardData[ s_currentChannel ].m_cardStatus == CS_ValidCard )
				{
					// Lovely, the old valid card is still there
 					s_currentCommand = CmdNone;
					s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
				}
				else if( s_cardData[ s_currentChannel ].m_cardStatus == CS_UnformattedCard )
				{
					// Found a card that has previously been marked as un-formatted
					s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
					s_currentCommand = CmdNone;
				}
				else
				{
					// A card that hasn't yet been validated is there
					//MEMCARD_DBGMSG( "[PMC] PMCmdExist says card exists but not yet valid" );
					s_cardData[ s_currentChannel ].m_cardStatus = CS_CardInserted;
					s_currentCommand = CmdAccept;
				}
				break;

			// Error - Invalidate the card and go back to scanning
			default:
//				MEMCARD_DBGMSG( "[PMC] PMCmdExist returned error '%s'", GetErrorString( s_syncResults ) );
				InvalidateCard( s_currentChannel );
				break;
		}
	}
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_Accept
	Purpose:	Card command handler for CmdAccept
				If a card is accepted then start reading the directories next frame.
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_Accept( void )
{
 	if( s_syncStatus == -1 )
 	{
		// No process active
 		MemCardAccept( s_currentChannel << 4 );
 		s_cardData[ s_currentChannel ].m_slotStatus = SS_Scanning;
//		MEMCARD_DBGMSG( "[PMC] called MemCardAccept()" );
 	}
 	else
 	{
//		MEMCARD_DBGMSG( "[PMC] returned from MemCardAccept()" );
		// Process just ended
 		switch( s_syncResults )
 		{
 			// Accepted ok
 			case McErrNone:
 			case McErrNewCard:
 				{
 					//MEMCARD_DBGMSG( "[PMC] PMCmdAccept says card ok but not yet valid" );
 					s_currentCommand = CmdReadDir;
 				}
 				break;

			// Wa-hey, an unformatted card!
			case  McErrNotFormat:
				s_cardData[ s_currentChannel ].m_cardStatus = CS_UnformattedCard;
				s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
 				s_currentCommand = CmdNone;
				break;

 			// Other error - Invalidate the card and continue searching
 			default:
 				//MEMCARD_DBGMSG( "[PMC] PMCmdAccept returned error '%s'", GetErrorString( s_syncResults ) );
				InvalidateCard( s_currentChannel );
 				break;
 		}
 	}
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_ReadDir
	Purpose:	Card command handler for CmdReadDir
				NB: This function contains blocking calls to the system memcard library and _will_ slow down game code.
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_ReadDir( void )
{
	int i,
		BlockCount = 0;


	// memCardGetDirectory() is blocking and returns after it has finsihed
	switch( MemCardGetDirentry( s_currentChannel << 4, "*",
								s_cardData[ s_currentChannel ].m_totalDirEntry,
								(long int *)&s_cardData[ s_currentChannel ].m_totalFileCount,
								0, MAXFILES ) )
	{
		// OK, now we have the cards file details
		case McErrNone:
			//MEMCARD_DBGMSG( "[PMC] PMCmdReadDir is valid" );

			if( s_cardData[ s_currentChannel ].m_totalFileCount == 0 )
			{
				// Card has no files - Accept the card now I guess...
				s_cardData[ s_currentChannel ].m_totalFileCount = 0;
				s_cardData[ s_currentChannel ].m_totalFreeBlocks = MAXBLOCKS;
				s_cardData[ s_currentChannel ].m_nativeFileCount = 0;
				s_file = 0;
 				s_currentCommand = CmdNone;
				s_cardData[ s_currentChannel ].m_cardStatus = CS_ValidCard;
				s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
			}
			else
			{
				// Calculate number of blocks free on this card
				for( i = 0; i < s_cardData[ s_currentChannel ].m_totalFileCount; i++ )
				{
					BlockCount += s_cardData[ s_currentChannel ].m_totalDirEntry[ i ].size / BLOCKSIZE;
				}
				s_cardData[ s_currentChannel ].m_totalFreeBlocks = MAXBLOCKS - BlockCount;

				// Start getting the file details from file 0
				s_file = 0;
				s_currentCommand = CmdReadFileInfo;
				s_cardData[ s_currentChannel ].m_nativeFileCount = 0;
			}
			break;

		// Error!
		default:
			//MEMCARD_DBGMSG( "[PMC] PMCmdReadDir is not valid!" );
			InvalidateCard( s_currentChannel );
			break;
	}
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_ReadFileInfo
	Purpose:	Card command handler for Cmd
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_ReadFileInfo( void )
{
 	if( s_syncStatus == -1 )
 	{
		// No process active - Start reading file info
 		if( MemCardReadFile( s_currentChannel << 4,
							 s_cardData[ s_currentChannel ].m_totalDirEntry[ s_file ].name,
							 (long unsigned int *)s_tempFileInfoBuffer, 0, 128 ) == 0 )
		{
			MEMCARD_DBGMSG( "[PMC] Reading file info for file %d", s_file );
		}
 	}
 	else
 	{
		// Must have just finished a MemCardReadFile()
		switch( s_syncResults )
		{
			// File read ok
			case McErrNone:
				// Does the file we have just read look like a valid native file?
				if( strncmp( s_fnameBase, s_cardData[ s_currentChannel ].m_totalDirEntry[ s_file ].name, 12 ) == 0 &&
					(char)s_tempFileInfoBuffer[ 0 ] == 'S' & (char)s_tempFileInfoBuffer[ 1 ] == 'C' /*&&		// Magic 'SC'
					(char)s_tempFileInfoBuffer[ 68 ] == 0 &&												// Pad area all 0s
					memcmp( (char *)&s_tempFileInfoBuffer[ 68 ], (char *)&s_tempFileInfoBuffer[ 68 + 1 ], 27 ) == 0 */ )
			// Pad area check removed since some save files ( RidgeRacerR4 f'rinstance ) have crap in there :(
			// Actually... This turns out to be PDA data :)
				{
					// Yup
					int FileNumber = s_cardData[ s_currentChannel ].m_nativeFileCount++;
					memcpy( s_cardData[ s_currentChannel ].m_nativeFileInfo[ FileNumber ].m_sjisName, &s_tempFileInfoBuffer[ 4 ], COMPRESSEDNAMESIZE );
					s_cardData[ s_currentChannel ].m_nativeFileInfo[ FileNumber ].m_blocks = (char)s_tempFileInfoBuffer[ 3 ];
					s_cardData[ s_currentChannel ].m_nativeFileInfo[ FileNumber ].m_dirEntry = s_file;
				}
				else
				{
					// Nope, ignore this file!
				}

				// Have we read all of the files yet?
				if( s_file < s_cardData[ s_currentChannel ].m_totalFileCount - 1 )
				{
					// Nope - Check the next one
					s_file++;
				}
				else
				{
					// Yes - Finished reading file info of all files on this card so it is now, at last, valid ( Huzzah! )
					s_file = 0;
 					s_currentCommand = CmdNone;
					s_cardData[ s_currentChannel ].m_cardStatus = CS_ValidCard;
					s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
				}
				break;

			// Error - Invalidate the card and go back to scanning
			default:
				//MEMCARD_DBGMSG( "[PMC] PMCmdReadFileInfo returned error '%s'", GetErrorString( s_syncResults ) );
				InvalidateCard( s_currentChannel );
				break;
		}
	}
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_ReadFile
	Purpose:	Card command handler for CmdReadFile
				Starts file loading, which is done in one big go.
				When file reading is finished, a callback function is executed
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_ReadFile( void )
{
 	if( s_syncStatus == -1 )
 	{
		// No process active - Start the load
		int FileNumber = s_cardData[ s_currentChannel ].m_nativeFileInfo[ s_file ].m_dirEntry;
		if( MemCardReadFile( s_currentChannel << 4, s_cardData[ s_currentChannel ].m_totalDirEntry[ FileNumber ].name,
							 (unsigned long *)s_bufPtr,
							 0, GetFileSizeInBlocks( s_currentChannel, s_file ) * BLOCKSIZE ) == 0 )
		{
//			MEMCARD_DBGMSG( "[PMC] Couldn't register MemCardReadFile!" );
			s_currentCommand = CmdNone;
			s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
		}
		else
		{
//			MEMCARD_DBGMSG( "[PMC] Called MemCardReadFile ( %d %d )", s_currentChannel, s_file );
			s_cardData[ s_currentChannel ].m_slotStatus = SS_Reading;
		}
	}
	else
	{
		// Must have just finished reading a file in
		switch( s_syncResults )
		{
			case McErrNone:
//				MEMCARD_DBGMSG( "[PMC] File read ok for card %d file %d", s_currentChannel, s_file );
				s_file = 0;
				s_currentCommand = CmdNone;
				s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;

				// Call the callback
				s_callbackFunction( CCS_ReadFileOK );
				s_callbackFunction = NULL;
				break;

			default:
//				MEMCARD_DBGMSG( "[PMC] PMCmdReadFile returned error '%s'", GetErrorString( s_syncResults ) );
//				InvalidateCard( s_currentChannel );

				// Call the callback
				s_currentCommand = CmdExist;
				s_callbackFunction( CCS_ReadFileFail );
				s_callbackFunction = NULL;
				break;
		}
	}
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_WriteFile
	Purpose:	Card command handler for CmdWriteFile
				Starts file writing, which is done in one big go.
				When file writing is finished, a callback function is executed
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_WriteFile( void )
{
//	MEMCARD_DBGMSG( "[PMC] HandleCmd_WriteFile  sync=%d", s_syncStatus );
 	if( s_syncStatus == -1 )
 	{
		// No process active
		int CRes;
//		bool NameValid = false;

		// Does the specified file exist?
		if( s_file == -1 )
		{
			// Nope, need to create it
//			do
			{
				// Create filename
				char NewFName[ 20 + 1 ] = "\0";
				CreateFName(s_fname);
				strcat( NewFName, s_fnameBase );		// Base filename
				strcpy( s_fname, NewFName );

				// The creation ( blocking function )
				CRes = MemCardCreateFile( s_currentChannel << 4, s_fname, s_blockCount );
				switch( CRes )
				{
					case McErrNone:
						MEMCARD_DBGMSG( "[PMC] Created new file '%s'", s_fname );
						s_currentCommand = CmdWriteFile;
						s_cardData[ s_currentChannel ].m_slotStatus = SS_Writing;
//						NameValid = true;
						break;

//					case McErrAlreadyExist:
//						// File already exists, try next
//						CreateFName();
//						break;

					default:
						MEMCARD_DBGMSG( "[PMC] Couldn't MemCardCreateFile due to '%s'", GetErrorString( CRes ) );
						s_currentCommand = CmdNone;
						s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
						s_callbackFunction( CCS_WriteFileFail );
						s_callbackFunction = NULL;
						return;
				}
			}
//			while( NameValid == false );
		}
		else
		{
			strcpy( s_fname, s_cardData[ s_currentChannel ].m_totalDirEntry[ s_cardData[ s_currentChannel ].m_nativeFileInfo[ s_file ].m_dirEntry ].name );
		}

		// Start the write
		if( MemCardWriteFile( s_currentChannel << 4, s_fname, (unsigned long *)s_bufPtr,
							 0, s_blockCount * BLOCKSIZE ) == 0 )
		{
			MEMCARD_DBGMSG( "[PMC] Couldn't register MemCardWriteFile due to '%s'", GetErrorString( s_syncResults ) );
			s_currentCommand = CmdNone;
			s_cardData[ s_currentChannel ].m_slotStatus = SS_Idle;
		}
		else
		{
			MEMCARD_DBGMSG( "[PMC] Called MemCardWriteFile" );
			s_cardData[ s_currentChannel ].m_slotStatus = SS_Writing;
		}
	}
	else
	{
		// Must have just finished writing a file
		switch( s_syncResults )
		{
			case McErrNone:
				MEMCARD_DBGMSG( "[PMC] File write completed!" );
				// Call the callback
				s_currentCommand = CmdReadDir;
				s_callbackFunction( CCS_WriteFileOK );
				s_callbackFunction = NULL;
				break;

			default:
				MEMCARD_DBGMSG( "[PMC] PMCmdWriteFile returned error '%s'", GetErrorString( s_syncResults ) );
				// Call the callback
				s_currentCommand = CmdReadDir;
				s_callbackFunction( CCS_WriteFileFail );
				s_callbackFunction = NULL;
				break;
		}
	}
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_FormatCard
	Purpose:	Card command handler for CmdFormatCard
				Starts card formatting, which is done in one big go and _is_ a blocking function!
				When card formatting is finished, a callback function is executed
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_FormatCard( void )
{
	int FRes;


	long int Dummy;
	MemCardSync( 0, &Dummy, &Dummy );

	// Format is a blocking function
	MemCardClose();
	MemCardUnformat( s_currentChannel << 4 );
	MemCardClose();
	FRes = MemCardFormat( s_currentChannel << 4 );
//	MEMCARD_DBGMSG( "[PMC] FRes = %d", FRes );
	switch( FRes )
	{
		case McErrNone:
			//MEMCARD_DBGMSG( "[PMC] Format OK!" );
			s_callbackFunction( CCS_FormatCardOK );
			break;

		default:
			//MEMCARD_DBGMSG( "[PMC] Format not ok - Returned error '%s'", GetErrorString( FRes ) );
			s_callbackFunction( CCS_FormatCardFail );
			break;
	}

	MemCardClose();
	s_callbackFunction = NULL;
	s_currentCommand = CmdExist;
}



/*----------------------------------------------------------------------
	Function:	HandleCmd_DeleteFile
	Purpose:	Card command handler for CmdDeleteFile
				Starts file deletion, which is done in one big go and _is_ a blocking function!
				When file deletion is finished, a callback function is executed
	Params:		None ( Uses global PMC variables :)
	Returns:	None
  ---------------------------------------------------------------------- */
void MemCard::HandleCmd_DeleteFile( void )
{
	int DRes;


	// Delete is a blocking function
	DRes = MemCardDeleteFile( s_currentChannel << 4, s_fname );
	switch( DRes )
	{
		case McErrNone:
			//MEMCARD_DBGMSG( "[PMC] Delete OK!" );
			s_callbackFunction( CCS_DeleteFileOK );
			s_callbackFunction = NULL;

			// Force a re-scan of the directories
			s_currentCommand = CmdReadDir;
			break;

		default:
			//MEMCARD_DBGMSG( "[PMC] Delete not ok - Returned error '%s'", GetErrorString( DRes ) );
			s_callbackFunction( CCS_DeleteFileFail );
			s_callbackFunction = NULL;

			// Force a re-scan of the directories
			s_currentCommand = CmdReadDir;
			break;
	}
}



/*----------------------------------------------------------------------
	Function:   	Handler
	Purpose:		Main memory card handler. Call from main loop somewhere once every vsync ( or less if you dare :)
	Params:			None
	Returns:		None
  ---------------------------------------------------------------------- */
//#define DEBUG_MEMCARD_STATUS

#ifdef	DEBUG_MEMCARD_STATUS
static int dinfo=true;
static char *cardtext[]= { "nocard", "cardinserted", "validcard", "unformattedcard" };
//static char *slottext[]= { "idle", "scanning", "reading", "writing", "formatting" };
static MemCard::CARDSTATUS lastCardStatus[2]={MemCard::CS_UnformattedCard,MemCard::CS_UnformattedCard};
static MemCard::SLOTSTATUS lastSlotStatus[2]={MemCard::SS_Formatting,MemCard::SS_Formatting};
#endif

void MemCard::Handler( void )
{
	// Ensure that memory cards are active
	if( s_active )
	{

// debug info
#ifdef	DEBUG_MEMCARD_STATUS
		if(dinfo)
{
	for(int i=0;i<2;i++)
	{
		if( lastCardStatus[i] != s_cardData[i].m_cardStatus )
		{
			MEMCARD_DBGMSG("card %d status changed to %s", i, cardtext[s_cardData[i].m_cardStatus] );
			lastCardStatus[i] = s_cardData[i].m_cardStatus;
		}
/*
		if( lastSlotStatus[i] != s_cardData[i].SlotStatus )
		{
			MEMCARD_DBGMSG("slot %d status changed to %s", i, slottext[s_cardData[i].SlotStatus] );
			lastSlotStatus[i] = s_cardData[i].SlotStatus;
		}
*/
	}
}
#endif

		// See what the system is doing
		s_syncStatus = MemCardSync( 1, &s_syncCmds, &s_syncResults );

		// Don't bother calling the command function if status is 0 ( still processing )
		if( s_syncStatus != 0 )
		{
			//MEMCARD_DBGMSG( "[PMC] run command  (syn:%c%d com:%d)", s_syncStatus<0?'-':'+', s_syncStatus, s_currentCommand );
			switch(s_currentCommand)
			{
				case CmdNone:
					HandleCmd_None();
					break;
				case CmdExist:
					HandleCmd_Exist();
					break;
				case CmdAccept:
					HandleCmd_Accept();
					break;
				case CmdReadDir:
					HandleCmd_ReadDir();
					break;
				case CmdReadFileInfo:
					HandleCmd_ReadFileInfo();
					break;
				case CmdReadFile:
					HandleCmd_ReadFile();
					break;
				case CmdWriteFile:
					HandleCmd_WriteFile();
					break;
				case CmdFormatCard:
					HandleCmd_FormatCard();
					break;
				case CmdDeleteFile:
					HandleCmd_DeleteFile();
					break;
			}
		}
	}
}



void MemCard::SetActiveCardSlot( int Channel )
{
	s_activeSlot = Channel;
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:		*result should point to a 16byte buffer
	Returns:	
  ---------------------------------------------------------------------- */
void MemCard::GiveCheckSum(unsigned char *_result,unsigned char *_data,u32 _size)
{
	MD5_CTX		ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,_data,_size);
	MD5Final(_result,&ctx);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:		*result is the checksum generated when the data was saved
	Returns:	
  ---------------------------------------------------------------------- */
int MemCard::TestCheckSum(unsigned char *_result,unsigned char *_data,u32 _size)
{
	MD5_CTX			ctx;
	unsigned char	calcedResult[MD5_CHECKSUM_SIZE];
	int				i;

	MD5Init(&ctx);
	MD5Update(&ctx,_data,_size);
	MD5Final(calcedResult,&ctx);

	for(i=0;i<MD5_CHECKSUM_SIZE;i++)
	{
		if(calcedResult[i]!=_result[i])
		{
			return false;
		}
	}

	return true;
}





/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:		
	Returns:	
  ---------------------------------------------------------------------- */
void MemCard::CreateFName(char *fname)
{
	sprintf( s_fnameBase, "%s", MEMCARD_BASE_FILENAME);

	// Finally, add the users savename
	strcat( s_fnameBase, "--------\0" );
	for(int i=0;i<strlen(s_userFileName);i++)
	{
		s_fnameBase[12+i]=s_userFileName[i];
	}

	
	MEMCARD_DBGMSG("MemCard filename set to '%s'", s_fnameBase );
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:		
	Returns:	
  ---------------------------------------------------------------------- */
bool MemCard::IsValidSavename(int _slot,char *_name)
{
	int		i;
	CARDDEF	*cdata=&s_cardData[_slot];

	if(_name==NULL||_name[0]=='\0')
		return false;

	for(i=0;i<cdata->m_nativeFileCount;i++)
	{
		if(strncmp(GetFileName(_slot,i),_name,8)==0)
			return false;
	}
	return true;
}

// returns -1 on error
int MemCard::FindFilenumberFromFilename(int _slot,char *_name)
{
	int		i;
	CARDDEF	*cdata=&s_cardData[_slot];

	if(_name==NULL||_name[0]=='\0')
		return -1;

	for(i=0;i<cdata->m_nativeFileCount;i++)
	{
		if(strncmp(GetFileName(_slot,i),_name,8)==0)
			return i;
	}
	return -1;
}



/*===========================================================================
 end */