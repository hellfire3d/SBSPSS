/*=========================================================================

	bubicles.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "bubicles.h"

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif

#ifndef	__UTILS_HEADER__
#include "utils\utils.h"
#endif

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
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

int				CBubicleFactory::s_initialised=false;
CBubicleEmitter	*CBubicleFactory::s_emitters;
CBubicle		*CBubicleFactory::s_bubicles;

int			CBubicleFactory::s_frameTypeCounter=0;
const int	CBubicleFactory::s_frameTabSize=8;
const int	CBubicleFactory::s_frameTabSizeMask=0x07;
sFrameHdr	*CBubicleFactory::s_frameTab[s_frameTabSize];
const int	CBubicleFactory::s_frameTabSrc[s_frameTabSize]=
{
	FRM__BUBBLE_1,FRM__BUBBLE_2,FRM__BUBBLE_2,FRM__BUBBLE_1,
	FRM__BUBBLE_2,FRM__BUBBLE_1,FRM__BUBBLE_2,FRM__BUBBLE_3,
};

DVECTOR		CBubicleFactory::s_mapPositionOffset={0,0};

int s_numLiveBubicleEmitters=0;
int s_numLiveBubicles=0;




/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::create()
{
	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::init(BubicleEmitterData *_init)
{
	m_data=*_init;
	m_frameCount=0;
	m_spawnFrameCount=0;
	m_active=true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::think(int _frames)
{
	m_frameCount+=_frames;
	m_spawnFrameCount+=_frames;

	if(m_spawnFrameCount>=m_data.m_birthRate)
	{
		m_spawnFrameCount=0;
		for(int i=0;i<m_data.m_birthAmount;i++)
		{
			if(CBubicleFactory::spawnParticle(&m_data)==NULL)
				break;	// Ran out of bubicles :(
		}
	}

	if(m_data.m_life!=-1&&m_frameCount>m_data.m_life)
	{
		m_active=false;
		s_numLiveBubicleEmitters--;
		ASSERT(s_numLiveBubicleEmitters>=0);	// Woah, something bad has happened!
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#ifdef SHOW_BUBICLE_EMITTERS
void CBubicleEmitter::render()
{
	DVECTOR		mapPos={0,0}
	POLY_F4		*f4;
	if(m_data.m_applyMapOffset)
	{
		mapPos=*CBubicleFactory::getMapOffset();
	}
	f4=GetPrimF4();
	setPolyF4(f4);
	setXYWH(f4,m_data.m_x-mapPos->vx,m_data.m_y-mapPos->vy,m_data.m_w,m_data.m_h);
	setSemiTrans(f4,1);
	setShadeTex(f4,0);
	setRGB0(f4,30,40,50);
	AddPrimToList(f4,m_data.m_bubicleBase.m_ot+1);
}
#endif


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::kill()
{
	ASSERT(m_active);

	s_numLiveBubicleEmitters--;
	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::setPos(int _x,int _y)
{
	ASSERT(m_active);
	
	m_data.m_x=_x;
	m_data.m_y=_y;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::setSize(int _w,int _h)
{
	ASSERT(m_active);
	
	m_data.m_w=_w;
	m_data.m_h=_h;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::create()
{
	m_active=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::init(BubicleData *_init,int _x,int _y,int _applyMapOffset)
{
	m_data=*_init;

	m_x=(_x-(m_data.m_w>>1))<<ACCURACY_SHIFT;
	m_y=(_y-(m_data.m_h>>1))<<ACCURACY_SHIFT;

	m_data.m_vxmax;
	m_data.m_vymax;
	m_data.m_w<<=ACCURACY_SHIFT;
	m_data.m_h<<=ACCURACY_SHIFT;

	m_typeSizeChange=getRnd()&1;
	m_vSizeChange=0;	
	m_frameCount=0;
	m_applyMapOffset=_applyMapOffset;

	m_fhBub=CBubicleFactory::getBubbleFrameHeader();

	m_active=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::think(int _frames)
{
	ASSERT(isActive());

	for(int i=0;i<_frames;i++)		// Good or bad way to do this? Alternative is a bunch of multiplies..
	{
		if(m_x!=m_data.m_vxmax)
		{
			m_x+=m_data.m_vx;
			if(m_data.m_vxmax<0)
			{
				m_data.m_vx+=m_data.m_vdx;
				if(m_data.m_vx<m_data.m_vxmax)
					m_data.m_vx=m_data.m_vxmax;
			}
			else if(m_data.m_vxmax>0)
			{
				m_data.m_vx+=m_data.m_vdx;
				if(m_data.m_vx>m_data.m_vxmax)
					m_data.m_vx=m_data.m_vxmax;
			}
		}

		if(m_y!=m_data.m_vymax)
		{
			m_y+=m_data.m_vy;
			if(m_data.m_vymax<0)
			{
				m_data.m_vy+=m_data.m_vdy;
				if(m_data.m_vy<m_data.m_vymax)
					m_data.m_vy=m_data.m_vymax;
			}
			else if(m_data.m_vymax>0)
			{
				m_data.m_vy+=m_data.m_vdy;
				if(m_data.m_vy>m_data.m_vymax)
					m_data.m_vy=m_data.m_vymax;
			}
		}

		m_data.m_theta=(m_data.m_theta+m_data.m_vtheta)&4095;

		m_data.m_wobbleWidth+=m_data.m_vwobbleWidth;
		m_data.m_vwobbleWidth+=m_data.m_vdwobbleWidth;


		if(m_typeSizeChange)
		{
			if(m_data.m_w<m_data.m_h)
				m_vSizeChange+=m_data.m_dvSizeChange;
			else if(m_data.m_w>m_data.m_h)
				m_vSizeChange-=m_data.m_dvSizeChange;
			m_data.m_w+=m_vSizeChange;
		}
		else
		{
			if(m_data.m_h<m_data.m_w)
				m_vSizeChange+=m_data.m_dvSizeChange;
			else if(m_data.m_h>m_data.m_w)
				m_vSizeChange-=m_data.m_dvSizeChange;
			m_data.m_h+=m_vSizeChange;
		}
	}
	
	m_frameCount+=_frames;

	if(m_frameCount>m_data.m_life)
	{
		m_active=false;
		s_numLiveBubicles--;
		ASSERT(s_numLiveBubicles>=0);		// Woah, something bad has happened!
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::render()
{
	ASSERT(isActive());

	DVECTOR		mapPos={0,0};
	POLY_FT4	*ft4;
	int			x,y,w,h;

	if(m_applyMapOffset)
	{
		mapPos=*CBubicleFactory::getMapOffset();
	}

	x=(m_x>>ACCURACY_SHIFT)-mapPos.vx;
	y=(m_y>>ACCURACY_SHIFT)-mapPos.vy;
	w=m_data.m_w>>ACCURACY_SHIFT;
	h=m_data.m_h>>ACCURACY_SHIFT;

	x+=(msin(m_data.m_theta)*(m_data.m_wobbleWidth>>ACCURACY_SHIFT))>>12;

	ft4=CGameScene::getSpriteBank()->printFT4(m_fhBub,0,0,0,0,m_data.m_ot);
	setXYWH(ft4,x,y,w,h);
	setSemiTrans(ft4,1);
	setRGB0(ft4,m_data.m_colour.m_r,m_data.m_colour.m_g,m_data.m_colour.m_b);
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::init()
{
	int				i;
	CBubicleEmitter	*emt;
	CBubicle		*bub;
	SpriteBank		*SprBank=CGameScene::getSpriteBank();
	for(i=0;i<s_frameTabSize;i++)
	{
		s_frameTab[i]=SprBank->getFrameHeader(s_frameTabSrc[i]);
	}

	emt=s_emitters=(CBubicleEmitter*)MemAlloc(sizeof(CBubicleEmitter)*NUM_EMITTERS,"BubicleEmitters");
	for(i=0;i<NUM_EMITTERS;i++,emt++)
		emt->create();
	
	bub=s_bubicles=(CBubicle*)MemAlloc(sizeof(CBubicle)*NUM_BUBICLES,"Bubicles");
	for(i=0;i<NUM_BUBICLES;i++,bub++)
		bub->create();

	SYSTEM_DBGMSG("CBubicleFactory::init ( allocated %d bytes )",(sizeof(CBubicleEmitter)*NUM_EMITTERS)+(sizeof(CBubicle)*NUM_BUBICLES));

	s_initialised=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::shutdown()
{
	ASSERT(s_initialised);
	
	MemFree(s_emitters);
	MemFree(s_bubicles);

	s_initialised=false;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::think(int _frames)
{
	ASSERT(s_initialised);

	int				frames;
	int				i,count;
	CBubicleEmitter	*emt;
	CBubicle		*bub;

	emt=s_emitters;
	count=s_numLiveBubicleEmitters;
	for(i=0;i<count;i++)
	{
		while(!emt->isActive())
		{
			emt++;
		}
		emt->think(_frames);
		emt++;
	}
	
	bub=s_bubicles;
	count=s_numLiveBubicles;
	for(i=0;i<count;i++)
	{
		while(!bub->isActive())
		{
			bub++;
		}
		bub->think(_frames);
		bub++;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::render()
{
	ASSERT(s_initialised);

	int				i;
	CBubicleEmitter	*emt;
	CBubicle		*bub;

#ifdef SHOW_BUBICLE_EMITTERS
	emt=s_emitters;
	for(i=0;i<NUM_EMITTERS;i++,emt++)
	{
		if(emt->isActive())
			emt->render();
	}
#endif
	
	bub=s_bubicles;
	for(i=0;i<NUM_BUBICLES;i++,bub++)
	{
		if(bub->isActive())
			bub->render();
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CBubicleEmitter	*CBubicleFactory::spawnEmitter(BubicleEmitterData *_init)
{
	ASSERT(s_initialised);

	int				i;
	CBubicleEmitter	*emt;
	
	emt=s_emitters;
	for(i=0;i<NUM_EMITTERS;i++,emt++)
	{
		if(!emt->isActive())
		{
			emt->init(_init);
			s_numLiveBubicleEmitters++;
			return emt;
		}
	}

	SYSTEM_DBGMSG("Out of CBubicleEmitters!");
	return NULL;
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CBubicle *CBubicleFactory::spawnParticle(BubicleEmitterData *_init)
{
	ASSERT(s_initialised);

	int				i;
	CBubicle		*bub;

	bub=s_bubicles;
	for(i=0;i<NUM_BUBICLES;i++,bub++)
	{
		if(!bub->isActive())
		{
			BubicleData		newBubData;
			int				x,y;

			x=_init->m_x+getRndRange(_init->m_w);
			y=_init->m_y+getRndRange(_init->m_h);

			newBubData.m_life=_init->m_bubicleBase.m_life+getRndRange(_init->m_bubicleRange.m_life);
			newBubData.m_vx=_init->m_bubicleBase.m_vx+getRndRange(_init->m_bubicleRange.m_vx);
			newBubData.m_vdx=_init->m_bubicleBase.m_vdx+getRndRange(_init->m_bubicleRange.m_vdx);
			newBubData.m_vxmax=_init->m_bubicleBase.m_vxmax+getRndRange(_init->m_bubicleRange.m_vxmax);
			newBubData.m_vy=_init->m_bubicleBase.m_vy+getRndRange(_init->m_bubicleRange.m_vy);
			newBubData.m_vdy=_init->m_bubicleBase.m_vdy+getRndRange(_init->m_bubicleRange.m_vdy);
			newBubData.m_vymax=_init->m_bubicleBase.m_vymax+getRndRange(_init->m_bubicleRange.m_vymax);
			newBubData.m_w=_init->m_bubicleBase.m_w+getRndRange(_init->m_bubicleRange.m_w);
			newBubData.m_h=_init->m_bubicleBase.m_h+getRndRange(_init->m_bubicleRange.m_h);
			newBubData.m_dvSizeChange=_init->m_bubicleBase.m_dvSizeChange+getRndRange(_init->m_bubicleRange.m_dvSizeChange);
			newBubData.m_theta=_init->m_bubicleBase.m_theta+getRndRange(_init->m_bubicleRange.m_theta);
			newBubData.m_vtheta=_init->m_bubicleBase.m_vtheta+getRndRange(_init->m_bubicleRange.m_vtheta);
			newBubData.m_wobbleWidth=_init->m_bubicleBase.m_wobbleWidth+getRndRange(_init->m_bubicleRange.m_wobbleWidth);
			newBubData.m_vwobbleWidth=_init->m_bubicleBase.m_vwobbleWidth+getRndRange(_init->m_bubicleRange.m_vwobbleWidth);
			newBubData.m_vdwobbleWidth=_init->m_bubicleBase.m_vdwobbleWidth+getRndRange(_init->m_bubicleRange.m_vdwobbleWidth);
			newBubData.m_ot=_init->m_bubicleBase.m_ot+getRndRange(_init->m_bubicleRange.m_ot);
			newBubData.m_colour.m_r=_init->m_bubicleBase.m_colour.m_r+getRndRange(_init->m_bubicleRange.m_colour.m_r);
			newBubData.m_colour.m_g=_init->m_bubicleBase.m_colour.m_r+getRndRange(_init->m_bubicleRange.m_colour.m_g);
			newBubData.m_colour.m_b=_init->m_bubicleBase.m_colour.m_r+getRndRange(_init->m_bubicleRange.m_colour.m_b);

			bub->init(&newBubData,x,y,_init->m_applyMapOffset);
			s_numLiveBubicles++;
			return bub;
		}
	}

//	SYSTEM_DBGMSG("Out of CBubicles!");
	return NULL;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
sFrameHdr *CBubicleFactory::getBubbleFrameHeader()
{
	s_frameTypeCounter=(s_frameTypeCounter+1)&s_frameTabSizeMask;

	return s_frameTab[s_frameTypeCounter];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleFactory::setMapOffset(DVECTOR *_offset)
{
	s_mapPositionOffset=*_offset;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const DVECTOR *CBubicleFactory::getMapOffset()
{
	return &s_mapPositionOffset;
}


/*===========================================================================
 end */