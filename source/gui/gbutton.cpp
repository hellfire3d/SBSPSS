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
		}
	}
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
	
	CGUIToggleButton::think(_frames);
	if(isSelected())
	{
		int pad;

		pad=PadGetRepeat(0);
		if(pad&PAD_CROSS)
		{
			int	*target;
			int	*data;
			int	tmp;

			target=getTarget();
			data=getData();
			do
			{
				tmp=*data;
				if(*target==*data)
				{
					break;
				}
				data++;
			}
			while(tmp<*data);
			if(tmp>*data)
			{
				data=getData();
			}
			*target=*data;
		}
	}
}


/*===========================================================================
 end */