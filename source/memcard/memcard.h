/*=========================================================================

	memcard.h

	Author:  Paul Grenfell @ Climax
	Created: Febuary 1999
	Project: Theme Park 2
	Purpose: Memory card access

	Copyright (c) 1999 Climax Development Ltd

===========================================================================*/

#ifndef __MEMCARD_MEMCARD_H__
#define __MEMCARD_MEMCARD_H__



/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __MEMCARD_MD5_H__
#include "memcard\md5.h"
#endif


/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */




/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Class
	----- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */

// Definition of a memcard file callback
typedef void (* FileCallbackDef)( int _status );


class MemCard
{
public:
		/*
		   Filenames created will be of something like "BEXXXX-00000SAVE000A". The last four characters are generated
		   automatically by the file create function. All files on a card need unique names so if we can't save the file
		   "BEXXXX-00000SAVE000A" we try and save "BEXXXX-00000SAVE000B" and so on until we succeed. If we create the filename
		   this way then we won't be able to copy between cards ( the first save on each card will always be called
		   "BEXXXX-00000SAVE000A" ). To solve this we put a random number ( or a frame count... ) into the last three
		   digits of the name when we save. This should at least limit the chances of files with identical names being saved
		   onto different cards.
		*/
		// PKG - Changed this now.. savegame names are fixed to "B?XXXX-00000xxxxxxxx" where
		// xxxxxxxx is the users save name

		// Describe the filename for this product
/*	What utter tosh
		#define FNAME_MAGIC			"B"					// Always B, 'cos it's magic
		#define FNAME_PCODE_US		"ASLUS-00000"
		#define FNAME_PCODE_EURO	"ESLES-00000"
*/
// Memcard filenames (not using else, to make sure its correct)
#if defined(__TERRITORY_USA__)
#define	MEMCARD_BASE_FILENAME		"BASLUS-01114"
#endif
#if defined(__TERRITORY_UK__)
#define	MEMCARD_BASE_FILENAME		"BESLES-03253"
#endif
#if defined(__TERRITORY_EUR__)
#define	MEMCARD_BASE_FILENAME		"BESLES-03286"
#endif




	
		// Describes the status of the card in a slot
		typedef enum
		{
			CS_NoCard,					   		// No card in slot
			CS_CardInserted,					// Card in slot ( busy scanning it )
			CS_ValidCard,						// Totally valid, formatted and happy card
			CS_UnformattedCard,					// Card is valid but not formatted
		} CARDSTATUS;

		// Describes what a card slot is doing
		typedef enum _pmcstatus
		{
			SS_Idle,							// Slot is idle
			SS_Scanning,						// Slot is scanning ( card existance checks )
			SS_Reading,							// Slot is reading card
			SS_Writing,							// Slot is writing to card
			SS_Formatting,						// Slot is formatting card
		} SLOTSTATUS;

		// Memcard callbacks return one of these:
		typedef enum _pmccallbackstatus
		{
			CCS_ReadFileOK,
			CCS_ReadFileFail,
			CCS_WriteFileOK,
			CCS_WriteFileFail,
			CCS_FormatCardOK,
			CCS_FormatCardFail,
			CCS_DeleteFileOK,
			CCS_DeleteFileFail,
			CCS_GeneralFailure,
		} CALLBACKSTATUS;




		MemCard()		{;}
		~MemCard()		{;}

		static bool Start( void );
		static bool Stop( void );
		static void InvalidateCard( int Channel );
		static void Handler( void );

		static CARDSTATUS GetCardStatus( int Channel );
		static SLOTSTATUS GetSlotStatus( int Channel );
		static int GetFileCountOnCard( int Channel );
		static int GetFreeBlocksOnCard( int Channel );
		static const char *GetFileName( int Channel, int File );
		static int GetFileSizeInBlocks( int Channel, int File );

		static void FillHeaderDetails( unsigned char *HeaderBase, int FileLength, char *Filename );

		static bool ReadFile( int Channel, int File, void *Dest, FileCallbackDef Callback );
		static bool WriteFile( int Channel, int File, u8 *FName, void *Source, int FileLength, FileCallbackDef Callback );
		static bool FormatCard( int Channel, FileCallbackDef Callback );
		static bool DeleteFile( int Channel, int File, FileCallbackDef Callback );

		static void SetActiveCardSlot( int Channel );

		static void GiveCheckSum(unsigned char *_result,unsigned char *_data,u32 _size);
		static int TestCheckSum(unsigned char *_result,unsigned char *_data,u32 _size);

		static bool	IsValidSavename(int _slot,char *_name);
		static int	FindFilenumberFromFilename(int _slot,char *_name);

		enum {
			BLOCKSIZE				=8192,			// 8k per file block

		};
private:
		enum
		{
			MAXBLOCKS				=15,			// Max of 15 blocks per card
			MAXFILES				=15,			// Max of 15 files per card
			COMPRESSEDNAMESIZE		=64,			// Length of Shift-JIS filename
			CARDSLOTSTOCHECK		=2,				// Number of card slots available
		};

		// Available command types
		typedef enum
		{
			CmdNone,								// No command
			CmdExist,								// Check that card exists
			CmdAccept,								// Accept card after cheking that it exists
			CmdReadDir,								// Read directory of card
			CmdReadFileInfo,						// Reading info of each file
			CmdReadFile,							// Reading a file from card to memory
			CmdWriteFile,							// Writing data to a file
			CmdFormatCard,							// Formatting card
			CmdDeleteFile,							// Delete a file
		} CMDTYPE;

		// Definition of a card slot
		typedef struct _pmccarddef
		{
			CARDSTATUS			m_cardStatus;					// Status of the crad ( 'in'/'out'/'maybe' sort of thing )
			SLOTSTATUS			m_slotStatus;					// Status of the slot ( 'Idle'/'Scan'/'Read'/'Write'/'Format' )

			// General data on all files on card
			int					m_totalFileCount;				// Number of files on this card
			int					m_totalFreeBlocks;				// Number of free blocks on this card
			struct DIRENTRY 	m_totalDirEntry[ MAXFILES ];	// Basic file info ( internal use mainly )

			// Data on app specific files
			int					m_nativeFileCount;				// Number of files on this card that belong to this app
			struct _fileinfo									// File info on files that belong to this app
			{
				unsigned short	m_sjisName[ 64 ];				// Shift-JIS name of file
				int				m_blocks;						// Number of blocks this file uses
				int				m_dirEntry;						// Number of file in TotalDirEntry array
			} m_nativeFileInfo[ MAXFILES ];
		} CARDDEF;


		// Command handler functions
		static void HandleCmd_None( void );
		static void HandleCmd_Exist( void );
		static void HandleCmd_Accept( void );
		static void HandleCmd_ReadDir( void );
		static void HandleCmd_ReadFileInfo( void );
		static void HandleCmd_ReadFile( void );
		static void HandleCmd_WriteFile( void );
		static void HandleCmd_FormatCard( void );
		static void HandleCmd_DeleteFile( void );

		static void CreateFName(char *fname);


		static int				s_active;
								
		static int				s_currentChannel,m_nextChannel;
		static CMDTYPE			s_currentCommand,m_nextCommand;
		static int				s_activeSlot;
		static CARDDEF			s_cardData[CARDSLOTSTOCHECK];
								
		static int				s_file;
		static int				s_blockCount;
		static char				s_tempFileInfoBuffer[128];
								
		static long int			s_syncStatus,s_syncCmds,s_syncResults;
								
		static void				*s_bufPtr;
								
		static char				s_userFileName[8+1];
		static char				s_fname[20+1];
		static char				s_fnameBase[40];

		static FileCallbackDef	s_callbackFunction;

};


/*---------------------------------------------------------------------- */

#endif	/* __MEMCARD_H__ */

/*===========================================================================
 end */