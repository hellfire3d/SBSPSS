/*=========================================================================

	spu.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__SOUND_SPU_H__
#define	__SOUND_SPU_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

typedef struct ReverbDetails
{
	long			m_type;
	long			m_delay;
	short			m_depth;
	long			m_feedback;
};


class CSpuSound
{
public:
	enum
	{
		REVERB_MAX_DEPTH=0x7fff,
		REVERB_MIN_DEPTH=-0x8000,
		REVERB_MIN_MODE_DELAY=0,
		REVERB_MAX_MODE_DELAY=127,
		REVERB_MIN_MODE_FEEDBACK=0,
		REVERB_MAX_MODE_FEEDBACK=127,
	};

	void			initialise();
	void			shutdown();

	void			setReverbActive(int _active);
	void			setReverbDetails(ReverbDetails *_details);
	
private:
	enum
	{
		MAX_SPU_MANAGEMENT=200,			// Number of samples that can be loaded..
	};

	static char		s_spuManagementTable[];

	int				m_reverbActive;
	ReverbDetails	m_currentDetails;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __SOUND_SPU_H__ */

/*===========================================================================
 end */
