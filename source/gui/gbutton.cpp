/*=========================================================================

	gbutton.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gbutton.h"

#ifndef __PAD_PADS_H__
#include "pad\pads.h"
#endif

#ifndef __SOUND_SOUND_H__
#include "sound\sound.h"
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
void CGUIToggleButton::init(CGUIObject *_parent,GUIId _id)
{
	CGUIObject::init(_parent,_id);
	m_target=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIToggleButton::setButtonTarget(int *_target)
{
	m_target=_target;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIToggleButton::think(int _frames)
{
	ASSERT(getTarget());
	
	CGUIObject::think(_frames);
	if(isSelected())
	{
		int pad;

		pad=PadGetRepeat(0);
		if(pad&PAD_CROSS)
		{
			int *target;
			target=getTarget();
			if(*target==true)
			{
				*target=false;
			}
			else
			{
				*target=true;
			}
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__SELECT);
		}
	}
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIValueButton::init(CGUIObject *_parent,GUIId _id)
{
	CGUIToggleButton::init(_parent,_id);
	m_value=-1;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIValueButton::setButtonValue(int _value)
{
	m_value=_value;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIValueButton::think(int _frames)
{
	ASSERT(getTarget());
	
	CGUIObject::think(_frames);
	if(isSelected())
	{
		if(PadGetDown(0)&PAD_CROSS)
		{
			*getTarget()=m_value;
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__SELECT);
		}
	}
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUICycleButton::init(CGUIObject *_parent,GUIId _id)
{
	CGUIToggleButton::init(_parent,_id);
	m_data=0;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUICycleButton::setButtonData(int *_data)
{
	  m_data=_data;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUICycleButton::think(int _frames)
{
	ASSERT(getTarget());
	ASSERT(getData());
	
	CGUIObject::think(_frames);
	if(isSelected())
	{
		int pad;

		pad=PadGetRepeat(0);
		if(pad&(PAD_CROSS|PAD_RIGHT))
		{
			int	*target;
			int	*data;
			int	tmp;

			target=getTarget();
			data=getData();
			do
			{
				tmp=*data;
				data++;
				if(*target==tmp)
				{
					break;
				}
			}
			while(tmp<*data);
			if(tmp>*data)
			{
				data=getData();
			}
			*target=*data;
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__SELECT);
		}
		else if(pad&PAD_LEFT)
		{
			int	*target;
			int	*data;
			int	tmp;

			target=getTarget();
			data=getData();
			do
			{
				tmp=*data;
				if(*target==tmp)
				{
					break;
				}
				data++;
			}
			while(tmp<*data);
			if(data==getData())
			{
				do
				{
					tmp=*data;
					data++;
				}
				while(tmp<*data);
				data--;
			}
			else
			{
				data--;
			}
			*target=*data;
			CSoundMediator::playSfx(CSoundMediator::SFX_FRONT_END__SELECT);
		}
	}
}






/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISliderButton::init(CGUIObject *_parent,GUIId _id)
{
	CGUIToggleButton::init(_parent,_id);
	m_min=m_max=0;
	m_scrollSpeed=DEFAULT_SCROLL_SPEED;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISliderButton::setButtonRange(int _min,int _max)
{
	m_min=_min;
	m_max=_max;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISliderButton::setScrollSpeed(int _scrollSpeed)
{
	m_scrollSpeed=_scrollSpeed;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUISliderButton::think(int _frames)
{
	ASSERT(getTarget());
	ASSERT(m_min!=m_max);
	
	CGUIObject::think(_frames);
	if(isSelected())
	{
		int padRepeat,padDown;
		int	*target=getTarget();
		int	makeNoise=false;
		CSoundMediator::SFXID	noise=CSoundMediator::SFX_FRONT_END__SELECT;
		
		padRepeat=PadGetRepeat(0);
		padDown=PadGetDown(0);
		if(padDown&PAD_LEFT)
		{
			*target-=1;
			makeNoise=true;
			noise=CSoundMediator::SFX_FRONT_END__SELECT;
		}
		else if(padDown&PAD_RIGHT)
		{
			*target+=1;
			makeNoise=true;
		}
		else if(padRepeat&PAD_LEFT)
		{
			*target-=(_frames*m_scrollSpeed);
			makeNoise=true;
		}
		else if(padRepeat&PAD_RIGHT)
		{
			*target+=(_frames*m_scrollSpeed);
			makeNoise=true;
		}

		if(*target<m_min)
		{
			*target=m_min;
			noise=CSoundMediator::SFX_FRONT_END__ERROR;
		}
		else if(*target>m_max)
		{
			*target=m_max;
			noise=CSoundMediator::SFX_FRONT_END__ERROR;
		}

		if(makeNoise)
		{
			CSoundMediator::playSfx(noise);
		}
	}
}


/*===========================================================================
 end */