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

#include	"game\game.h"

/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
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
	}
	else
	{
		// This object is a bastard :)
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
		TPOLY_F4		*f4;
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
		f4=GetPrimTF4();
		setXYWH(f4,x,y,w,h);
		setRGB0(f4,  0,  0, 90);
		setTSemiTrans(f4,true);
		AddPrimToList(f4,ot);
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

		DrawLine(x  ,y  ,x+w,y  ,r,g,b,0);
		DrawLine(x  ,y  ,x  ,y+h,r,g,b,0);
		DrawLine(x+w,y  ,x+w,y+h,r,g,b,0);
		DrawLine(x  ,y+h,x+w,y+h,r,g,b,0);
		x+=1;y+=1;w-=2;h-=2;
		DrawLine(x  ,y  ,x+w,y  ,r,g,b,0);
		DrawLine(x  ,y  ,x  ,y+h,r,g,b,0);
		DrawLine(x+w,y  ,x+w,y+h,r,g,b,0);
		DrawLine(x  ,y+h,x+w,y+h,r,g,b,0);
		x+=1;y+=1;w+=1;h+=1;
		DrawLine(x  ,y  ,x+w,y  ,0,0,0,0);
		DrawLine(x  ,y  ,x  ,y+h,0,0,0,0);
		DrawLine(x+w,y  ,x+w,y+h,0,0,0,0);
		DrawLine(x  ,y+h,x+w,y+h,0,0,0,0);
	}
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIObject::think(int _frames)
{
	ASSERT(this==m_this);

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

//	s_uiSpriteBank=new ("UI Sprites") SpriteBank();
//	s_uiSpriteBank->load(SPRITES_SPRITES_SPR);
	s_uiSpriteBank=CGameScene::getSpriteBank();

}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void drawBambooBorder(int _x,int _y,int _w,int _h,int _ot)
{
	sFrameHdr	*vbam,*hbam,*corner;
	int			totalSize,numSprites,step;
	int			x1,y1,x2,y2;
	int			i,x,y;

	vbam=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOVERTICAL);
	hbam=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOHORIZONTAL);
	corner=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOTOPLEFT);	// All corners are the same size

	// Top and bottom
	totalSize=_w-((corner->W+corner->W)/2)-hbam->W;
	numSprites=(totalSize/hbam->W)+1;
	if(numSprites>1)
	{
		step=(totalSize<<4)/(numSprites-1);
		x1=1+(_x+(corner->W/2)+(hbam->W/2)-(hbam->W/2)+hbam->W-hbam->W-1)<<4;
		y1=_y-(hbam->H/2);
		y2=y1+_h;
		for(i=0;i<numSprites;i++)
		{
			x=x1>>4;
			s_uiSpriteBank->printFT4(hbam,x,y1,0,0,_ot);
			s_uiSpriteBank->printFT4(hbam,x,y2,0,0,_ot);
			x1+=step;
		}
	}

	// Left and right
	totalSize=_h-((corner->H+corner->H)/2)-vbam->H;
	numSprites=(totalSize/vbam->H)+1;
	if(numSprites>1)
	{
		step=(totalSize<<4)/(numSprites-1);
		x1=_x-(vbam->W/2);
		x2=x1+_w;
		y1=1+(_y+(corner->H/2)+(vbam->H/2)-(vbam->H/2)+vbam->H-vbam->H-1)<<4;
		for(i=0;i<numSprites;i++)
		{
			y=y1>>4;
			s_uiSpriteBank->printFT4(vbam,x1,y,0,0,_ot);
			s_uiSpriteBank->printFT4(vbam,x2,y,0,0,_ot);
			y1+=step;
		}
	}
	else
	{
		x1=_x-(vbam->W/2);
		x2=x1+_w;
		y1=1+(_y+(corner->H/2)+(vbam->H/2)-(vbam->H/2)+vbam->H-vbam->H-1);
		s_uiSpriteBank->printFT4(vbam,x1,y1,0,0,_ot);
		s_uiSpriteBank->printFT4(vbam,x2,y1,0,0,_ot);
	}

	// Draw Corners
	corner=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOTOPLEFT);
	s_uiSpriteBank->printFT4(corner,_x-(corner->W/2),_y-(corner->H/2),0,0,_ot);
	corner=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOTOPRIGHT);
	s_uiSpriteBank->printFT4(corner,_x+_w-(corner->W/2),_y-(corner->H/2),0,0,_ot);
	corner=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOBOTRIGHT);
	s_uiSpriteBank->printFT4(corner,_x+_w-(corner->W/2),_y+_h-(corner->H/2),0,0,_ot);
	corner=s_uiSpriteBank->getFrameHeader(FRM__BAMBOOBOTLEFT);
	s_uiSpriteBank->printFT4(corner,_x-(corner->W/2),_y+_h-(corner->H/2),0,0,_ot);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
typedef struct
{
	int	gapfromheadtobubble;
	int speechmarkgapfromtop;
	int	speechmarkheight;
}_sbb;
_sbb sbb=
{
	32,
	5,
	10,
};
void drawSpeechBubbleBorder(int _x,int _y,int _w,int _h,int _ot,int _faceFrame)
{
	sFrameHdr	*cornerFh;
	POLY_FT4	*ft4;
	LINE_F2		*f2;
	int			centreX,centreY;
	sFrameHdr	*faceFh;
	TPOLY_F4		*f4;
	TPOLY_F3		*f3;


	// Squeeze in the corners a bit..
	_x+=4;
	_y+=5;
	_w-=8;
	_h-=10;

	// Sprite corner pieces
	cornerFh=s_uiSpriteBank->getFrameHeader(FRM__SPEECHBUBBLECORNER);
	ft4=s_uiSpriteBank->printFT4(cornerFh,_x-cornerFh->W,_y-cornerFh->H,0,0,_ot);setSemiTrans(ft4,true);
	ft4=s_uiSpriteBank->printFT4(cornerFh,_x+_w+cornerFh->W+1,_y-cornerFh->H,1,0,_ot);setSemiTrans(ft4,true);
	ft4=s_uiSpriteBank->printFT4(cornerFh,_x-cornerFh->W,_y+_h+cornerFh->H+1,0,1,_ot);setSemiTrans(ft4,true);
	ft4=s_uiSpriteBank->printFT4(cornerFh,_x+_w+cornerFh->W+1,_y+_h+cornerFh->H+1,1,1,_ot);setSemiTrans(ft4,true);

	// Black edge lines
	f2=DrawLine(_x,_y-cornerFh->H-1,_x+_w,_y-cornerFh->H-1,0,0,0,_ot);setSemiTrans(f2,true);
	f2=DrawLine(_x+_w+cornerFh->W+1,_y,_x+_w+cornerFh->W+1,_y+_h,0,0,0,_ot);setSemiTrans(f2,true);
	f2=DrawLine(_x+_w,_y+_h+cornerFh->H+1,_x,_y+_h+cornerFh->H+1,0,0,0,_ot);setSemiTrans(f2,true);

	// White middle
	f4=GetPrimTF4();
	setXYWH(f4,_x,_y-cornerFh->H,_w+1,cornerFh->H);
	setRGB0(f4,255,255,255);
	setTSemiTrans(f4,true);
	AddPrimToList(f4,_ot);
	f4=GetPrimTF4();
	setXYWH(f4,_x-cornerFh->W,_y,_w+(cornerFh->W*2)+1,_h+1);
	setRGB0(f4,255,255,255);
	setTSemiTrans(f4,true);
	AddPrimToList(f4,_ot);
	f4=GetPrimTF4();
	setXYWH(f4,_x,_y+_h+1,_w+1,cornerFh->H);
	setRGB0(f4,255,255,255);
	setTSemiTrans(f4,true);
	AddPrimToList(f4,_ot);

	if(_faceFrame!=-1)
	{
		// Speaking characters head
		centreX=_x-cornerFh->W-sbb.gapfromheadtobubble-(64/2);
		centreY=_y+(_h/2);
		faceFh=s_uiSpriteBank->getFrameHeader(_faceFrame);
		ft4=s_uiSpriteBank->printFT4(faceFh,centreX-(faceFh->W/2),centreY-(faceFh->H/2),0,0,_ot);//setSemiTrans(ft4,true);

		// White speech bubble triangle shaped piece with black outlines.. (!?)
		int speechmarkEndX,speechmarkEndY;
		speechmarkEndX=_x-cornerFh->W-sbb.gapfromheadtobubble;
		speechmarkEndY=centreY;
		f2=DrawLine(_x-cornerFh->W-1,_y,_x-cornerFh->W-1,_y+sbb.speechmarkgapfromtop-1,0,0,0,_ot);setSemiTrans(f2,true);
		f2=DrawLine(_x-cornerFh->W-1,_y+sbb.speechmarkgapfromtop,speechmarkEndX,speechmarkEndY,0,0,0,_ot);setSemiTrans(f2,true);
		f2=DrawLine(speechmarkEndX,speechmarkEndY,_x-cornerFh->W-1,_y+sbb.speechmarkgapfromtop+sbb.speechmarkheight,0,0,0,_ot);setSemiTrans(f2,true);
		f2=DrawLine(_x-cornerFh->W-1,_y+sbb.speechmarkgapfromtop+sbb.speechmarkheight+1,_x-cornerFh->W-1,_y+_h,0,0,0,_ot);setSemiTrans(f2,true);
		f3=GetPrimTF3();
		setXY3(f3,_x-cornerFh->W,_y+sbb.speechmarkgapfromtop,
				  speechmarkEndX,speechmarkEndY,
				  _x-cornerFh->W,_y+sbb.speechmarkgapfromtop+sbb.speechmarkheight);
		setRGB0(f3,255,255,255);
		setTSemiTrans(f3,true);
		AddPrimToList(f3,_ot);
	}
	else
	{
		// Fill in the left hand black edge line
		f2=DrawLine(_x-cornerFh->W-1,_y,_x-cornerFh->W-1,_y+_h,0,0,0,_ot);setSemiTrans(f2,true);
	}

	// Background
	/*
	f4=GetPrimTF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,  0,  0, 90);
	setTSemiTrans(f4,true);
	AddPrimToList(f4,_ot);
	*/
}


/*===========================================================================
 end */
