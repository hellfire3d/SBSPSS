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
void CGUIButton::setButtonTarget(int *_target)
{
	m_target=_target;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIButton::setButtonData(int *_data)
{
	  m_data=_data;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CGUIButton::think(int _frames)
{
	ASSERT(m_target);
	ASSERT(m_data);
	
	CGUIObject::think(_frames);
	if(isSelected())
	{
		int pad;

		pad=PadGetRepeat(0);
		if(pad&PAD_CROSS)
		{
			int	*data;
			int	tmp;

			data=m_data;
			tmp=*data;
			do
			{
				tmp=*data;
				data++;
				if(*m_target==tmp)
				{
					break;
				}
			}
			while(tmp<*data);
			if(tmp>*data)
			{
				data=m_data;
			}
			*m_target=*data;
		}
	}
}

/*===========================================================================
 end */