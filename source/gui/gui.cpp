/*=========================================================================

	gui.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gui.h"

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef	__GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_UIGFX_H__
#include <uigfx.h>
#endif


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

static SpriteBank	*s_uiSpriteBank=0;

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::init(CGUIObject *_parent,GUIId _id)
{
	ASSERT(this!=m_this);

	m_id=_id;
	m_x=m_y=m_w=m_h=0;
	m_flags=getInitialFlags();
	m_this=this;
	m_parent=_parent;
	m_child=NULL;
	m_next=NULL;

	// Link in with the parent
	if(m_parent)
	{
		if(m_parent->m_child)
		{
			CGUIObject *pGUI;
			pGUI=m_parent->m_child;
			while(pGUI->m_next)
			{
				pGUI=pGUI->m_next;
			}
			pGUI->m_next=this;
		}
		else
		{
			m_parent->m_child=this;
		}
		setOt(m_parent->getOt()-1);
	}
	else
	{
		// This object is a bastard :)
		GUI_DBGMSG("INFO: GUI object without parent created!");
		setOt(INITIAL_OT);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::shutdown()
{
	ASSERT(this==m_this);

	if(m_child)m_child->shutdown();			m_child=NULL;
	if(m_next)m_next->shutdown();			m_next=NULL;
	m_parent=NULL;
	m_this=NULL;

	// Is this actually safe? Possibly not.. (PKG)
	delete this;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#ifdef __USER_paul__
int forceBorderDraw=false;
#endif
void CGUIObject::render()
{
	ASSERT(this==m_this);

	if(m_child)m_child->render();
	if(m_next)m_next->render();

	if(isHidden())
	{
		return;
	}

	if(getFlags(FLAG_DRAWBORDER))
	{
		POLY_F4		*f4;
		int			x,y,w,h;
		int			ot;

		x=getX()+getParentX();
		y=getY()+getParentY();
		w=getW();
		h=getH();
		ot=getOt();

		// Border
		drawBambooBorder(x,y,w,h,ot);

		// Background
		f4=GetPrimF4();
		setXYWH(f4,x,y,w,h);
		setRGB0(f4,  0,  0, 90);
		setSemiTrans(f4,true);
		AddPrimToList(f4,ot);

		/*
		g4=GetPrimG4();
		setXYWH(g4,x,y,w,h/2);
		setRGB0(g4,107,105, 98);
		setRGB1(g4,107,105, 98);
		setRGB2(g4,  0,  0, 90);
		setRGB3(g4,  0,  0, 90);
		setSemiTrans(g4,true);
		AddPrimToList(g4,ot);

		g4=GetPrimG4();
		setXYWH(g4,x,y+h/2,w,h/2);
		setRGB0(g4,  0,  0, 90);
		setRGB1(g4,  0,  0, 90);
		setRGB2(g4,107,105, 98);
		setRGB3(g4,107,105, 98);
		setSemiTrans(g4,true);
		AddPrimToList(g4,ot);
		*/
	}

#ifdef __USER_paul__
	if(forceBorderDraw)
	{
		int			x,y,w,h;
		int			ot;
		int			r,g,b;

		x=getX()+getParentX();
		y=getY()+getParentY();
		w=getW();
		h=getH();
		ot=getOt();
		r=g=b=200;

		DrawLine(x  ,y  ,x+w,y  ,r,g,b,ot);
		DrawLine(x  ,y  ,x  ,y+h,r,g,b,ot);
		DrawLine(x+w,y  ,x+w,y+h,r,g,b,ot);
		DrawLine(x  ,y+h,x+w,y+h,r,g,b,ot);
		x+=1;y+=1;w-=2;h-=2;
		DrawLine(x  ,y  ,x+w,y  ,r,g,b,ot);
		DrawLine(x  ,y  ,x  ,y+h,r,g,b,ot);
		DrawLine(x+w,y  ,x+w,y+h,r,g,b,ot);
		DrawLine(x  ,y+h,x+w,y+h,r,g,b,ot);
		x+=1;y+=1;w+=1;h+=1;
		DrawLine(x  ,y  ,x+w,y  ,0,0,0,ot);
		DrawLine(x  ,y  ,x  ,y+h,0,0,0,ot);
		DrawLine(x+w,y  ,x+w,y+h,0,0,0,ot);
		DrawLine(x  ,y+h,x+w,y+h,0,0,0,ot);
	}
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#include "utils\utils.h"
int wob=0;
void CGUIObject::think(int _frames)
{
	ASSERT(this==m_this);


if(m_id==0&&++wob==20)
{
	setObjectXYWH(16+getRndRange(10),100+getRndRange(5),512-64,120);
	wob=0;
}

	if(m_child)m_child->think(_frames);
	if(m_next)m_next->think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::recalc()
{
	ASSERT(this==m_this);

	if(m_child)m_child->recalc();
	if(m_next)m_next->recalc();
}








/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithFont::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObject::init(_parent,_id);
	m_fontBank=new ("GUIObjectWithFont:fontBank") FontBank();
	m_fontBank->initialise(&standardFont);
	m_fontBank->setJustification(FontBank::JUST_CENTRE);
	m_fontBank->setOt(getOt());
	m_fontBank->setColour(DEFAULT_FONT_R,DEFAULT_FONT_G,DEFAULT_FONT_B);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithFont::shutdown()
{
	CGUIObject::shutdown();
	m_fontBank->dump();
	delete(m_fontBank);
	m_fontBank=NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithFont::setFlags(GUI_FLAGS _flags)
{
	CGUIObject::setFlags(_flags);
	if(_flags&FLAG_SELECTED)
	{
		FontBank	*fb=getFontBank();
		fb->setColour(CGUIObjectWithFont::SELECTED_FONT_R,CGUIObjectWithFont::SELECTED_FONT_G,CGUIObjectWithFont::SELECTED_FONT_B);
		fb->setWobble(true);

	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithFont::clearFlags(GUI_FLAGS _flags)
{
	CGUIObject::clearFlags(_flags);
	if(_flags&FLAG_SELECTED)
	{
		FontBank	*fb=getFontBank();
		fb->setColour(CGUIObjectWithFont::DEFAULT_FONT_R,CGUIObjectWithFont::DEFAULT_FONT_G,CGUIObjectWithFont::DEFAULT_FONT_B);
		fb->setWobble(false);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithFont::setOt(int _ot)
{
	CGUIObject::setOt(_ot);
	getFontBank()->setOt(_ot);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithFont::recalc()
{
	int	x,y,w,h;

	CGUIObject::recalc();
	x=getX()+getParentX()+BORDERWIDTH;
	y=getY()+getParentY()+BORDERHEIGHT;
	w=getW()-(BORDERWIDTH*2);
	h=getH()-(BORDERHEIGHT*2);
	getFontBank()->setPrintArea(x,y,w,h);
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithSpriteBank::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObject::init(_parent,_id);
	m_spriteBank=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithSpriteBank::shutdown()
{
	ASSERT(m_spriteBank);

	CGUIObject::shutdown();
	m_spriteBank->dump();
	delete m_spriteBank;
	m_spriteBank=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObjectWithSpriteBank::setSpriteBank(FileEquate _fe)
{
	m_spriteBank=new ("spritebank") SpriteBank();
	m_spriteBank->load(_fe);
}






/*----------------------------------------------------------------------
	Function:
	Purpose:	NB: This permenantly keeps a copy of the UI sprite bank
				loaded in vram! Can be changed if necessary.. (PKG)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void initGUIStuff()
{
	ASSERT(!s_uiSpriteBank);

	s_uiSpriteBank=new ("UI Sprites") SpriteBank();
	s_uiSpriteBank->load(UI_UIGFX_SPR);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void drawBambooBorder(int _x,int _y,int _w,int _h,int _ot)
{
	sFrameHdr	*vbam,*hbam;
	int			totalSize,numSprites,step;
	int			x1,y1,x2,y2;
	int			i;


	vbam=s_uiSpriteBank->getFrameHeader(FRM__VBAMBOO);
	hbam=s_uiSpriteBank->getFrameHeader(FRM__HBAMBOO);

	// Top & bottom
	totalSize=_w-vbam->W-hbam->W;
	numSprites=(totalSize/hbam->W)+1;
	if(numSprites>1)
	{
		step=(totalSize<<4)/(numSprites-1);
		x1=(_x+(vbam->W/2)+(hbam->W/2))<<4;
		y1=_y-(hbam->H/2);
		y2=y1+_h;
		for(i=0;i<numSprites;i++)
		{
			s_uiSpriteBank->printFT4(hbam,(x1>>4)-(hbam->W/2),y1,0,0,_ot);
			s_uiSpriteBank->printFT4(hbam,(x1>>4)-(hbam->W/2),y2,0,0,_ot);
			x1+=step;
		}
	}

	// Left & right
	totalSize=_h-hbam->H-vbam->H;
	numSprites=(totalSize/vbam->H)+1;
	if(numSprites>1)
	{
		step=(totalSize<<4)/(numSprites-1);
		y1=(_y+(hbam->H/2)+(vbam->H/2))<<4;
		x1=_x-(vbam->W/2);
		x2=x1+_w;
		for(i=0;i<numSprites;i++)
		{
			s_uiSpriteBank->printFT4(vbam,x1,(y1>>4)-(vbam->H/2),0,0,_ot);
			s_uiSpriteBank->printFT4(vbam,x2,(y1>>4)-(vbam->H/2),0,0,_ot);
			y1+=step;
		}
	}
}


/*===========================================================================
 end */
