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


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef __SPR_INGAMEFX_H__
#include <ingamefx.h>
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


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static SpriteBank	*sprites=NULL;
void CBubicleEmitter::init(BubicleEmitterData *_init)
{
	for(int i=0;i<NUM_BUB;i++)
	{
		m_bubicles[i]=NULL;
	}
	m_frameCount=0;

	m_data=*_init;

if(sprites==NULL)
{
	sprites=new ("bubble sprites") SpriteBank();
	sprites->load(INGAMEFX_INGAMEFX_SPR);
}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::think()
{
	int	birthAmount;


	birthAmount=0;
	for(int i=0;i<NUM_BUB;i++)
	{
		if(m_bubicles[i])
		{
			m_bubicles[i]->think();
			if(m_bubicles[i]->isDead())
			{
				delete m_bubicles[i];
				m_bubicles[i]=NULL;
			}
		}
		else
		{
			if(m_frameCount>=m_data.m_birthRate)
			{
				birthAmount=m_data.m_birthAmount;
				m_frameCount=0;
			}
			if(birthAmount)
			{
				m_bubicles[i]=createNewBubicle(&m_data);
				birthAmount--;
			}
		}
	}


	m_frameCount++;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::render()
{
	for(int i=0;i<NUM_BUB;i++)
	{
		if(m_bubicles[i])
			m_bubicles[i]->render();
	}

	POLY_F4		*f4;
	f4=GetPrimF4();
	setPolyF4(f4);
	setXYWH(f4,m_data.m_x,m_data.m_y,m_data.m_w,m_data.m_h);
	setSemiTrans(f4,1);
	setShadeTex(f4,0);
	setRGB0(f4,30,40,50);
	AddPrimToList(f4,m_data.m_bubicleBase.m_ot+1);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicleEmitter::setPos(int _x,int _y)
{
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
	m_data.m_w=_w;
	m_data.m_h=_h;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int fixed=false;
CBubicle *CBubicleEmitter::createNewBubicle(BubicleEmitterData *_init)
{
	CBubicle		*newBub;
	BubicleData		newBubData;
	int				x,y;

	newBub=new ("CBubicle") CBubicle();
	
	x=_init->m_x+getRndRange(_init->m_w);
	y=_init->m_y+getRndRange(_init->m_h);

if(fixed)
{
	newBubData.m_life=_init->m_bubicleBase.m_life;
	newBubData.m_vx=_init->m_bubicleBase.m_vx;
	newBubData.m_vdx=_init->m_bubicleBase.m_vdx;
	newBubData.m_vxmax=_init->m_bubicleBase.m_vxmax;
	newBubData.m_vy=_init->m_bubicleBase.m_vy;
	newBubData.m_vdy=_init->m_bubicleBase.m_vdy;
	newBubData.m_vymax=_init->m_bubicleBase.m_vymax;
	newBubData.m_w=_init->m_bubicleBase.m_w;
	newBubData.m_h=_init->m_bubicleBase.m_h;
	newBubData.m_dvSizeChange=_init->m_bubicleBase.m_dvSizeChange;
	newBubData.m_theta=_init->m_bubicleBase.m_theta;
	newBubData.m_vtheta=_init->m_bubicleBase.m_vtheta;
	newBubData.m_wobbleWidth=_init->m_bubicleBase.m_wobbleWidth;
	newBubData.m_vwobbleWidth=_init->m_bubicleBase.m_vwobbleWidth;
	newBubData.m_vdwobbleWidth=_init->m_bubicleBase.m_vdwobbleWidth;
	newBubData.m_ot=_init->m_bubicleBase.m_ot;
	newBubData.m_colour.m_r=_init->m_bubicleBase.m_colour.m_r;
	newBubData.m_colour.m_g=_init->m_bubicleBase.m_colour.m_r;
	newBubData.m_colour.m_b=_init->m_bubicleBase.m_colour.m_r;
}
else
{
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
}

	newBub->init(&newBubData,x,y);

	return newBub;
}
	

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static int gaycount=0;
void CBubicle::init(BubicleData *_init,int _x,int _y)
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

	if((gaycount++)&0x31)
		m_fhBub=sprites->getFrameHeader(FRM__BUBBLE_SMALL);
	else
		m_fhBub=sprites->getFrameHeader(FRM__BUBBLE_FLOWER);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CBubicle::think()
{
	if(!isDead())
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

		m_frameCount++;		//PKG
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
	if(!isDead())
	{
		POLY_FT4	*ft4;
		int			x,y,w,h;

		x=m_x>>ACCURACY_SHIFT;
		y=m_y>>ACCURACY_SHIFT;
		w=m_data.m_w>>ACCURACY_SHIFT;
		h=m_data.m_h>>ACCURACY_SHIFT;

		x+=(msin(m_data.m_theta)*(m_data.m_wobbleWidth>>ACCURACY_SHIFT))>>12;

		ft4=sprites->printFT4((sFrameHdr*)m_fhBub,0,0,0,0,m_data.m_ot);
		setXYWH(ft4,x,y,w,h);
		setSemiTrans(ft4,1);
		setRGB0(ft4,m_data.m_colour.m_r,m_data.m_colour.m_g,m_data.m_colour.m_b);
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CBubicle::isDead()
{
	return m_frameCount>m_data.m_life||m_y<0;
}


/*===========================================================================
 end */