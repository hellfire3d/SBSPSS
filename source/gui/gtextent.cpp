NB - This is not used.. but left IN VSS incase it needs to be resurrected (PKG)


/*=========================================================================

	gtextent.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gtextent.h"

#ifndef __GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CGUIKeyboard : public CGUIObjectWithFont
{
public:
	virtual void		init(CGUIObject *_parent,GUIId _id=noId);
	
	virtual void		setTextDetails(char *_store,int _maxLength);
	
	virtual void		render();
	virtual void		think(int _frames);

	virtual void		setFlags(GUI_FLAGS _flags);
	virtual void		clearFlags(GUI_FLAGS _flags);
	
	virtual int			isUnselectable()							{return m_unselectable;}


protected:
	virtual GUI_FLAGS	getInitialFlags()							{return FLAG_DRAWBORDER;}


private:
	struct TextEntryLine
	{
		char **m_chars;
		int  m_numChars;
	};

	void				renderKeyboard();
	
	static TextEntryLine	s_textEntryLines[];
	static int				s_numTextEntryLines;

	char				*m_store;						// Where the string is held ( NB: MUST BE 1 BYTE LONGER THAN MAXLENGTH COS OF TRAILING 0 )
	int					m_maxLength;
	
	int					m_cursorX,m_cursorY;
	int					m_currentChar;
	int					m_unselectable;
	int					m_flashCount;
	
};


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static char	*s_textEntryText1[]={	"A","B","C","D","E","F","G","H","I","J","K","L","M"	};
static int	s_textEntryLength1=13;
static char *s_textEntryText2[]={	"N","O","P","Q","R","S","T","U","V","W","X","Y","Z" };
static int	s_textEntryLength2=13;
static char *s_textEntryText3[]={	"a","b","c","d","e","f","g","h","i","j","k","l","m" };
static int	s_textEntryLength3=13;
static char *s_textEntryText4[]={	"n","o","p","q","r","s","t","u","v","w","x","y","z" };
static int	s_textEntryLength4=13;
static char *s_textEntryText5[]={	".","0","1","2","3","4","5","6","7","8","9","<","=" };
static int	s_textEntryLength5=13;

CGUIKeyboard::TextEntryLine CGUIKeyboard::s_textEntryLines[]=
{
	{	s_textEntryText1, s_textEntryLength1 },
	{	s_textEntryText2, s_textEntryLength2 },
	{	s_textEntryText3, s_textEntryLength3 },
	{	s_textEntryText4, s_textEntryLength4 },
	{	s_textEntryText5, s_textEntryLength5 },
};
int CGUIKeyboard::s_numTextEntryLines=sizeof(CGUIKeyboard::s_textEntryLines)/sizeof(CGUIKeyboard::TextEntryLine);


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIKeyboard::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithFont::init(_parent,_id);
	m_store=NULL;
	m_maxLength=0;
	m_unselectable=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIKeyboard::setTextDetails(char *_store,int _maxLength)
{
	m_store=_store;
	m_maxLength=_maxLength;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIKeyboard::render()
{
	FontBank		*fb;

	if(isSelected())
	{
		fb=getFontBank();
		fb->print(getW()/2,10,m_store);

		renderKeyboard();

		CGUIObjectWithFont::render();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIKeyboard::think(int _frames)
{
	CGUIObjectWithFont::think(_frames);
	m_flashCount+=_frames;
	if(isSelected())
	{
		int		pad;

		pad=PadGetRepeat(0);
		if(pad&PAD_LEFT)
		{
			if(--m_cursorX<0)m_cursorX=s_textEntryLines[m_cursorY].m_numChars-1;
		}
		if(pad&PAD_RIGHT)
		{
			if(++m_cursorX==s_textEntryLines[m_cursorY].m_numChars)m_cursorX=0;
		}
		if(pad&PAD_UP)
		{
			if(--m_cursorY<0)m_cursorY=s_numTextEntryLines-1;
		}
		if(pad&PAD_DOWN)
		{
			if(++m_cursorY==s_numTextEntryLines)m_cursorY=0;
		}

		if(PadGetDown(0)&PAD_CROSS)
		{
			char	currentChar;
			currentChar=s_textEntryLines[m_cursorY].m_chars[m_cursorX][0];
			if(currentChar=='<')
			{
			}
			else if(currentChar=='=')
			{
				unselect();
			}
			else
			{
				GUI_DBGMSG("-%c-",currentChar);
			}
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIKeyboard::setFlags(GUI_FLAGS _flags)
{
	CGUIObjectWithFont::setFlags(_flags);
	if(_flags&FLAG_SELECTED)
	{
		m_unselectable=false;
		m_cursorX=m_cursorY=0;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIKeyboard::clearFlags(GUI_FLAGS _flags)
{
	CGUIObjectWithFont::setFlags(_flags);
	if(_flags&FLAG_SELECTED)
	{
		m_unselectable=true;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
enum
{
	KEYBOARD_EDGE_PAD=15,
	KEYBOARD_BOTTOM_PAD=18,
	KEYBOARD_Y_PAD=7,
};
void CGUIKeyboard::renderKeyboard()
{
	FontBank		*fb;
	int				i,j;
	int				xBase,yBase;
	int				xStep,yStep;
	int				x,y;
	TextEntryLine	*line;


	fb=getFontBank();

	yStep=fb->getCharHeight()+KEYBOARD_Y_PAD;
	yBase=getH()-KEYBOARD_BOTTOM_PAD-((s_numTextEntryLines*yStep)-KEYBOARD_Y_PAD);

	y=yBase;
	line=s_textEntryLines;
	for(i=0;i<s_numTextEntryLines;i++)
	{
		xStep=(getW()-(KEYBOARD_EDGE_PAD*2))/(line->m_numChars+1);
		xBase=(getW()-xStep*line->m_numChars)/2;
		x=xBase;
		for(j=0;j<line->m_numChars;j++)
		{
			if(!(j==m_cursorX&&i==m_cursorY&&m_flashCount&16))
			{
				fb->print(x,y,line->m_chars[j]);
			}
			x+=xStep;
		}
		y+=yStep;
		line++;
	}
}

















/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextEntry::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObjectWithFont::init(_parent,_id);
	m_keyboard=new ("keyboard") CGUIKeyboard();
	m_keyboard->init(NULL);
	m_keyboard->setObjectXYWH(64,64,512-128,256-128);
	m_keyboard->setOt(10);
//	m_keyboard->select();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextEntry::shutdown()
{
	m_keyboard->shutdown();
	delete m_keyboard;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextEntry::setTextDetails(char *_store,int _maxLength)
{
	m_keyboard->setTextDetails(_store,_maxLength);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextEntry::render()
{
	if(isSelected())
	{
		m_keyboard->render();
	}
	CGUIObjectWithFont::render();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextEntry::think(int _frames)
{
	CGUIObjectWithFont::think(_frames);
	if(isSelected())
	{
		if(m_keyboard->isSelected())
		{
			m_keyboard->think(_frames);
		}
		else if(PadGetDown(0)&PAD_CROSS)
		{
GUI_DBGMSG("selecting keyboard!");			
			m_keyboard->select();
		}
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUITextEntry::setFlags(GUI_FLAGS _flags)
{
	CGUIObjectWithFont::setFlags(_flags);
//	m_keyboard->setFlags(_flags);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CGUITextEntry::isUnselectable()
{
	return m_keyboard->isUnselectable();
}


/*===========================================================================
 end */
