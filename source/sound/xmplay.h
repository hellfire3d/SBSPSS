/*=========================================================================

	xmplay.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__SOUND_XMPLAY_H__
#define	__SOUND_XMPLAY_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// This is a bit messy, but makes sure that you can't easily pass the wrong IDs to the xm functions
// ( ..something scares me about this way of doing it tho :)
typedef enum	{NO_SAMPLE=-1}		xmSampleId;
typedef enum	{NO_SONG=-1}		xmSongId;
typedef enum	{NOT_PLAYING=-1}	xmPlayingSongId;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CXMPlaySound
{
public:
	void			initialise();
	void			shutdown();

	xmSampleId		loadSamples(FileEquate _vhFe,FileEquate _vbFe);
	xmSongId		loadSongData(FileEquate _songFe);
	void			dumpSamples(xmSampleId _sampleId);
	void			dumpSongData(xmSongId _songId);

	void			setStereo(int _stereo);
	
	void			setVolume(xmPlayingSongId _songId,unsigned char _volume);
	void			setPanning(xmPlayingSongId _songId,char _pan);

	xmPlayingSongId	playSong(xmSampleId _sampleId,xmSongId _songId,int _baseChannel);
	void			stopSong(xmPlayingSongId _songId);

	xmPlayingSongId	playSfx(xmSampleId _sampleId,xmSongId _songId,int _baseChannel,int _sfxPattern,int _playMask=-1);
	int				isSfxActive(xmPlayingSongId _id);


	
private:
	enum
	{
		MAX_XM_HEADERS=8,
		MAX_SONG_HEADERS=24,
	};

	void			defragSpuMemory();

	unsigned char	*s_fhPtr[MAX_XM_HEADERS];
	unsigned char	*s_songPtr[MAX_SONG_HEADERS];
	
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __SOUND_XMPLAY_H__ */

/*===========================================================================
 end */
