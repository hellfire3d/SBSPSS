/*=========================================================================

	font.h

	Author:  PKG
	Created:
	Project:
	Purpose:

	Copyright (c) 2000

===========================================================================*/

#ifndef __GFX_FONT_H__
#define __GFX_FONT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_FDATA_H__
#include "gfx\fdata.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class FontBank
{
public:
	enum Justification
	{
		JUST_LEFT,
		JUST_RIGHT,
		JUST_CENTRE
	};

			FontBank()	{ m_initialised=false; };
	virtual ~FontBank()	{ ASSERT( !m_initialised ); }

	virtual void	initialise( FontData *_fontData );
	void	dump();
	int		isInitialised()				{ return m_initialised; }
	virtual void	print( int _x, int _y, u8 *_text );
	void			print( int _x, int _y, s32 _textId );
	void	setColour( u8 _r, u8 _g, u8 _b );
	void	setJustification( Justification _justification );
	void	setOt( u16 _ot );
	void	setPrintArea( int _x, int _y, int _w, int _h );
	void	setTrans( int _trans );
	void	setSMode( int _sMode );

	int		getCharWidth( u8 _char );
	int		getCharHeight();
	int		getStringWidth( char * text );
	int		getStringHeight( u8 *_text );

	int		getStrWrapLen( u8 *_text,int _maxWidth );

private:
	virtual int		printChar( u8 _char,int _x,int _y );


	enum {
		DEFAULT_R=128,
		DEFAULT_G=128,
		DEFAULT_B=128,
		DEFAULT_JUSTIFICATION=JUST_LEFT,
		DEFAULT_OT=1,
	};

protected:
	FontData		*m_fontData;
	u8				m_r, m_g, m_b;						// Font colour
	Justification	m_justification;
	u16				m_ot;								// Depth
	RECT			m_printArea;
	SpriteBank		m_spriteBank;
	int				m_initialised;

	int				m_trans, m_sMode;

};


/*---------------------------------------------------------------------- */

#endif	/* __GFX_FONT_H__ */

/*===========================================================================
 end */
