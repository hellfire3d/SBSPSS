/*=========================================================================

	gfactory.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gui\gfactory.h"

#ifndef __GUI_GTEXTBOX_H__
#include "gui\gtextbox.h"
#endif

#ifndef __GUI_GFRAME_H__
#include "gui\gframe.h"
#endif

#ifndef __GUI_GBUTTON_H__
#include "gui\gbutton.h"
#endif

#ifndef __GUI_GREADOUT_H__
#include "gui\greadout.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
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
CGUIObject	*CGUIFactory::createValueButtonFrame(class CGUIObject *_parent,
										 int _x,int _y,int _w,int _h,
										 int _textId,
										 int *_target,int _value,
										 CGUIObject::GUIId _id)
{
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUIValueButton		*vb;

	fr=new ("frame") CGUIGroupFrame();
	fr->init(_parent,_id);
	fr->setObjectXYWH(_x,_y,_w,_h);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,_w,_h);
		tb->setText(_textId);
		vb=new ("valuebutton") CGUIValueButton();
		vb->init(fr);
		vb->setButtonTarget(_target);
		vb->setButtonValue(_value);

	return fr;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CGUIObject	*CGUIFactory::createCycleButtonFrame(class CGUIObject *_parent,
										 int _x,int _y,int _w,int _h,
										 int _textId,
										 int *_target,int *_data,CGUITextReadout::TextReadoutData *_readoutData,
										 CGUIObject::GUIId _id)
{
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUICycleButton		*cb;
	CGUITextReadout		*tr;

	fr=new ("frame") CGUIGroupFrame();
	fr->init(_parent,_id);
	fr->setObjectXYWH(_x,_y,_w,_h);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,_w,(_h*2)/3);
		tb->setText(_textId);
		cb=new ("cyclebutton") CGUICycleButton();
		cb->init(fr);
		cb->setButtonTarget(_target);
		cb->setButtonData(_data);
		tr=new ("textreadout") CGUITextReadout();
		tr->init(fr);
		tr->setObjectXYWH(0,(_h*2)/3,_w,(_h*1)/3);
		tr->setReadoutTarget(_target);
		tr->setReadoutData(_readoutData);

	return fr;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CGUIObject	*CGUIFactory::createSliderButtonFrame(class CGUIObject *_parent,
										  int _x,int _y,int _w,int _h,
										  int _textId,
										  int *_target,int _min,int _max,
										  CGUIObject::GUIId _id)
{
	CGUIGroupFrame		*fr;
	CGUITextBox			*tb;
	CGUISliderButton	*sb;
	CGUIBarReadout		*br;

	fr=new ("frame") CGUIGroupFrame();
	fr->init(_parent,_id);
	fr->setObjectXYWH(_x,_y,_w,_h);
		tb=new ("textbox") CGUITextBox();
		tb->init(fr);
		tb->setObjectXYWH(0,0,_w,(_h*2)/3);
		tb->setText(_textId);
		sb=new ("sliderbutton") CGUISliderButton();
		sb->init(fr);
		sb->setButtonTarget(_target);
		sb->setButtonRange(_min,_max);
		br=new ("barreadout") CGUIBarReadout();
		br->init(fr);
		br->setObjectXYWH(0,(_h*2)/3,_w,(_h*1)/3);
		br->setReadoutTarget(_target);
		br->setReadoutRange(_min,_max);

	return fr;
}


/*===========================================================================
 end */
