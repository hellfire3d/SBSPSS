/*=========================================================================

	font.cpp

	Author:  PKG
	Created:
	Project:
	Purpose: 

	Copyright (c) 2000

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_FONT_H__
#include "font.h"
#endif

#ifndef __VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif


/*	Std Lib
	------- */

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */


/*----------------------------------------------------------------------
	Vars
	---- */

int FontBank::s_wobbleValue=0;


/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::initialise( FontData *_fontData )
{
	ASSERT( !m_initialised );

	m_fontData=_fontData;

	m_spriteBank.load( m_fontData->fontFileId );

	setOt( DEFAULT_OT );
	setColour( DEFAULT_R, DEFAULT_G, DEFAULT_B );
	setJustification( (Justification)DEFAULT_JUSTIFICATION );
	setPrintArea( 0,0, VidGetScrW(),VidGetScrH() );

	setTrans( 0 );
	setSMode( 0 );

	m_initialised=true;
	m_wobble=false;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::dump()
{
	m_spriteBank.dump();
	
	m_initialised=false;
}



/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::print( int _x, int _y, s32 _textId )
{
	print(_x,_y,(char*)TranslationDatabase::getString(_textId));
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int wstep=50;
int wspeed=100;
int wscale=1000;
void FontBank::print( int _x, int _y, const char *_text )
{
	ASSERT( m_initialised );

	int 	Size;
	int 	StartX;
	int 	Length=0;
	int		RectWidth;

	switch(m_justification)
	{
		case JUST_CENTRE:
			RectWidth=m_printArea.w;
			break;
		default:
			RectWidth=m_printArea.w-_x;
			break;
	}
	_x+=m_printArea.x;
	_y+=m_printArea.y;
	_y+=getCharHeight();		// origin at top left please...
	StartX=_x;

	while (*_text)
	{
		Length=getStrWrapLen(_text,RectWidth);
		switch (m_justification)
			{
			case JUST_LEFT:
				_x=StartX;
				break;
			case JUST_RIGHT:
				_x=StartX/*+RectWidth*/-Length;
				break;
			case JUST_CENTRE:
				_x=StartX-(Length/2);
				break;
			}

		int	fy=_y;
		while(*_text && Length>0)
		{
			if(m_wobble)
			{
				fy=_y+(msin((s_wobbleValue+(_x*wstep))&4095)/wscale);
			}
			Size=printChar(*_text++,_x,fy)+getCharGapX();
			_x+=Size;
			Length-=Size;
		}
		_y+=(getCharHeight());
		if(*_text=='\n') _text++;		// kill newline if there is one ( preserve multiple \n )
		while (*_text==' ') _text++;	// kill trailing spaces
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::printTillEndOfLine( int _x, int _y, const char *_text )
{
	ASSERT( m_initialised );

	int 	Size;
	int 	StartX;
	int 	Length=0;
	int		RectWidth;
	int		numCharsPrinted=0;

	switch(m_justification)
	{
		case JUST_CENTRE:
			RectWidth=m_printArea.w;
			break;
		default:
			RectWidth=m_printArea.w-_x;
			break;
	}
	_x+=m_printArea.x;
	_y+=m_printArea.y;
	_y+=getCharHeight();		// origin at top left please...
	StartX=_x;

	while (*_text)
	{
		Length=getStrWrapLen(_text,RectWidth);
		switch (m_justification)
			{
			case JUST_LEFT:
				_x=StartX;
				break;
			case JUST_RIGHT:
				_x=StartX/*+RectWidth*/-Length;
				break;
			case JUST_CENTRE:
				_x=StartX-(Length/2);
				break;
			}

		int	fy=_y;
		while(*_text && Length>0)
		{
			if(m_wobble)
			{
				fy=_y+(msin((s_wobbleValue+(_x*wstep))&4095)/wscale);
			}
			Size=printChar(*_text++,_x,fy)+getCharGapX();
			numCharsPrinted++;
			_x+=Size;
			Length-=Size;
		}
		break;
//		_y+=(getCharHeight());
//		if(*_text=='\n') _text++;		// kill newline if there is one ( preserve multiple \n )
//		while (*_text==' ') _text++;	// kill trailing spaces
	}

	return numCharsPrinted;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::setColour( u8 _r, u8 _g, u8 _b )
{
	m_r = _r;
	m_g = _g;
	m_b = _b;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::setJustification( Justification _justification )
{
	m_justification = _justification;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::setOt( u16 _ot )
{
	m_ot = _ot;
	m_ot=0; // Sorry!!
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::setPrintArea( int _x, int _y, int _w, int _h )
{
	m_printArea.x=_x;
	m_printArea.y=_y;
	m_printArea.w=_w;
	m_printArea.h=_h;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::setTrans( int _trans )
{
	m_trans=_trans;
}
void FontBank::setSMode( int _sMode )
{
	m_sMode=_sMode;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::getCharWidth( char _char )
{
	int	size;

	if( _char!=' ' )
	{
		if( m_fontData->fontTab[_char]==-1 ) _char=1;		// Error - Chartacter does not eixst!
		size=m_spriteBank.getFrameWidth( m_fontData->fontTab[_char] );
	}
	else
	{
		size=m_fontData->spaceWidth;
	}

	return size;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::getCharHeight()
{
	return m_fontData->charHeight+getCharGapY();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	return height of a string when wrapped to print area
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::getStringHeight( const char *_text )
{
	int length=0;
	int	height=0;

	while (*_text)
	{
		length=getStrWrapLen(_text,m_printArea.w);
		while(*_text && length>0)
		{
			length-=getCharWidth(*_text++)+getCharGapX();
		}
		height+=(getCharHeight());
		if(*_text=='\n') _text++;		// kill newline if there is one ( preserve multiple \n )
		while (*_text==' ') _text++;	// kill trailing spaces
	}

	return height;
}
int FontBank::getStringHeight( s32 _textId )
{
	return getStringHeight( TranslationDatabase::getString(_textId) );
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		FontBank::getCharGapX()
{
	return m_fontData->charGapX;
}
int		FontBank::getCharGapY()
{
	return m_fontData->charGapY;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::printChar( char _char,int _x,int _y )
{
	if (_char!=' ')
	{
		if( m_fontData->fontTab[_char]==-1 ) _char=1;		// Error - Chartacter does not eixst!
		POLY_FT4 *Ft4=m_spriteBank.printFT4(m_fontData->fontTab[_char],_x,_y,0,0,m_ot);
		setRGB0(Ft4,m_r,m_g,m_b);
		setShadeTex(Ft4,0);

		Ft4->tpage |= ( m_sMode << 5 );
		setSemiTrans( Ft4,m_trans );
		setShadeTex( Ft4,0 );
	}

	return getCharWidth(_char);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::getStringWidth( const char * text )
{
	return getStrWrapLen( text, m_printArea.w );
}
int FontBank::getStringWidth( s32 _textId )
{
	return getStringWidth( TranslationDatabase::getString(_textId) );
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void FontBank::think(int _frames)
{
	s_wobbleValue+=(_frames*wspeed)&4095;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int FontBank::getStrWrapLen( const char *_text,int _maxWidth )
{
	int		length=0,spaceW;
	char	C;

	spaceW=length+1;	// +1 to prevent infinite loop
	while (*_text && length<=_maxWidth) 
	{
		C=*_text++;
		if (C=='\n') break;
		if (C==' ') spaceW=length;
		length+=getCharWidth(C)+getCharGapX();
	}
	if (length>_maxWidth) length=spaceW;
	
	return length;
}




/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ScalableFontBank::initialise( FontData *_fontData )
{
	FontBank::initialise(_fontData);
	setScale(256);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int ScalableFontBank::printChar( char _char,int _x,int _y )
{
	if (_char!=' ')
	{
		sFrameHdr	*fh;
		POLY_FT4	*Ft4;

		if( m_fontData->fontTab[_char]==-1 ) _char=1;		// Error - Chartacter does not eixst!

		fh=m_spriteBank.getFrameHeader(m_fontData->fontTab[_char]);
		_x+=((fh->W*m_fontScale)>>9)-(fh->W/2);
		_y-=((fh->H*m_fontScale)>>9)-(fh->H/2);

		Ft4=m_spriteBank.printFT4Scaled(fh,_x,_y,0,0,m_ot,m_fontScale);
		setRGB0(Ft4,m_r,m_g,m_b);
		setShadeTex(Ft4,0);

		Ft4->tpage|=(m_sMode<<5);
		setSemiTrans(Ft4,m_trans);
		setShadeTex(Ft4,0);
	}

	return getCharWidth(_char);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int ScalableFontBank::getCharWidth( char _char )
{
	return (FontBank::getCharWidth(_char)*m_fontScale)>>8;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int ScalableFontBank::getCharHeight()
{
	return (FontBank::getCharHeight()*m_fontScale)>>8;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	ScalableFontBank::getCharGapX()
{
	return (m_fontData->charGapX*m_fontScale)>>8;
}
int	ScalableFontBank::getCharGapY()
{
	return (m_fontData->charGapY*m_fontScale)>>8;
}

/*===========================================================================
 end */
