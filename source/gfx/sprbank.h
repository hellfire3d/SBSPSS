/*=========================================================================

	sprbank.h

	Author:		PKG
	Created:
	Project:	PRLSR
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __GFX_SPRBANK_H__
#define __GFX_SPRBANK_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __VRAM_H__
#include "gfx\tpage.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
// Single instance of a sprite bank
class SpriteBankInstance
{
public:
				SpriteBankInstance( SpriteBankInstance *_next );
				~SpriteBankInstance();
	
	sFrameHdr	*getFrameHeaders()	{return m_frameHdr;}

	void		load( FileEquate _file );
	int			dump();

	SpriteBankInstance	*getNext()								{ return m_next; }
	void			setNext( SpriteBankInstance *_next)		{ m_next = _next; }
	FileEquate		getFile()								{ return m_file; }

//private:
	sFrameHdr	*m_frameHdr;
	TPAGE_DESC	m_tpageDesc;

	FileEquate	m_file;
	int			m_refCount;

	SpriteBankInstance	*m_next;
};

// Class for a bank of sprites ( actually a reference to a SpriteBankInstance )
class SpriteBank
{
public:
				SpriteBank();
				~SpriteBank();

	void		load( FileEquate _file );
	void		dump();

	sFrameHdr *	getHeaders()						{return (m_SpriteBankInstance->getFrameHeaders());}
	sFrameHdr *	getFrameHeader( int _frame )		{return (&m_SpriteBankInstance->getFrameHeaders()[ _frame ]);}
	s32			getFrameWidth( int _frame )			{return (m_SpriteBankInstance->getFrameHeaders()[ _frame ].W);}
	s32			getFrameHeight( int _frame )		{return (m_SpriteBankInstance->getFrameHeaders()[ _frame ].H);}

inline	POLY_FT4	*printFT4(int _frame,int _x,int _y,int _xFlip,int _yFlip,int _ot)	{return printFT4(&m_SpriteBankInstance->getFrameHeaders()[ _frame ],_x,_y,_xFlip,_yFlip,_ot);}
inline	POLY_FT4	*printFT4(sFrameHdr *_fh,int _x,int _y,int _xFlip,int _yFlip,int _ot)
{
	POLY_FT4	*ft4=GetPrimFT4();
	prepareFT4(ft4,_fh,_x,_y,_xFlip,_yFlip);
	AddPrimToList(ft4,_ot);
	return(ft4);
}
inline	POLY_FT4	*printFT4Scaled(int _frame,int _x,int _y,int _xFlip,int _yFlip,int _ot,long _z)	{return printFT4Scaled(&m_SpriteBankInstance->getFrameHeaders()[ _frame ],_x,_y,_xFlip,_yFlip,_ot,_z);}
inline	POLY_FT4	*printFT4Scaled(sFrameHdr *_fh,int _x,int _y,int _xFlip,int _yFlip,int _ot, long _z)
{
	POLY_FT4	*ft4=GetPrimFT4();
	prepareFT4Scaled(ft4,_fh,_x,_y,_xFlip,_yFlip,_z );
	AddPrimToList(ft4,_ot);
	return(ft4);
}

inline	POLY_FT4	*printRotatedScaledSprite(int _frame,int _xCentre,int _yCentre,int _xScale,int _yScale,int _rot,int _ot)	{return printRotatedScaledSprite(&m_SpriteBankInstance->getFrameHeaders()[ _frame ],_xCentre,_yCentre,_xScale,_yScale,_rot,_ot);}
inline	POLY_FT4	*printRotatedScaledSprite(sFrameHdr *_fh,int _xCentre,int _yCentre,int _xScale,int _yScale,int _rot,int _ot)
{
	ASSERT(_rot>=0);
	ASSERT(_rot<=4095);
	POLY_FT4	*ft4=GetPrimFT4();
	prepareFT4RotatedScaled(ft4,_fh,_xCentre,_yCentre,_xScale,_yScale,_rot);
	AddPrimToList(ft4,_ot);
	return(ft4);
}

	// Hmm.. why did these used to be private?
	void		prepareFT4(POLY_FT4 *_ft4,sFrameHdr *_fh,int _x,int _y,bool _xFlip,bool _yFlip);
	void		prepareGT4(POLY_GT4 *_gt4,sFrameHdr *_fh,int _x,int _y,bool _xFlip,bool _yFlip);
	void		prepareFT4Scaled(POLY_FT4 *_ft4,sFrameHdr *_fh,int _x,int _y,bool _xFlip,bool _yFlip,long _z);
	void		prepareFT4RotatedScaled(POLY_FT4 *_ft4,sFrameHdr *_fh,int _xCentre,int _yCentre,int _xScale,int _yScale,int _rot);
	void 		setUVTp(sFrameHdr *_fh,POLY_FT4 *_ft4,int _xFlip,int _yFlip);
	void		setUVTp(sFrameHdr *_fh,POLY_GT4 *_gt4,int _xFlip,int _yFlip);

//	void		printRotatedScaledSprite(sFrameHdr *_fh,int _xCentre,int _yCentre,int _xScale,int _yScale,int _rot,int _ot);


private:
	class SpriteBankInstance	*m_SpriteBankInstance;

};

#endif	/* __GFX_SPRBANK_H__ */

/*===========================================================================
 end */
