/***********************/
/*** Selection Class ***/
/***********************/

#include "stdafx.h"
#include "Select.h"

/*****************************************************************************/
CSelect::CSelect()
{
		Invalidate();
}

/*****************************************************************************/
void	CSelect::Invalidate()
{
		Sel.SetRectEmpty();
		Mode=0;
}

/*****************************************************************************/
bool	CSelect::Handle(CPoint &Pnt,UINT Flags)
{
		Flags&=(MK_LBUTTON | MK_RBUTTON);
		if (!Mode && !Flags) return(false);
		if (!Mode && Flags)
		{ // Start new Selection
			Start(Pnt,Flags);
		}
// Order the points
		Sel.SetRect(Pos0,Pnt);
		Sel.NormalizeRect();
		Sel.InflateRect(0,0,1,1);	// extend to make valid sel (right and bottom are +1)

		if (Mode && !Flags) 
		{
			End(Pnt,Flags);
		}
		return(true);
}

/*****************************************************************************/
void	CSelect::Start(CPoint &Pnt,UINT Flags)
{
		Invalidate();
		Mode=Flags;
		Pos0=Pnt;
}

/*****************************************************************************/
void	CSelect::End(CPoint &Pnt,UINT Flags)
{
		Mode=0;
}

/*****************************************************************************/
bool	CSelect::IsValid()
{
	if (Sel.left==-1 || Sel.top==-1) return(false);		// Invalid coords
	return(!Sel.IsRectEmpty());
}

/*****************************************************************************/
bool	CSelect::HasSelection()
{
	if (Mode) return(false);							// Still Selecting?
	return(IsValid());
}
