/*=========================================================================

	AllFiles.h

	Author:		Gary
	Created:	011200
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ALLFILES_H__
#define	__ALLFILES_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

//#include <vector>
//#include <gtypes.h>
//#include <gstring.hpp>
//#include <misc.hpp>
//#include <gfname.hpp>
//#include <pcre.h>

//#include <ginio.h>
//#include <repread.h>

//#include "sprset.h"

using namespace std;

/*----------------------------------------------------------------------
	AllFiles Class
	Hold's a list of all the files to be processed on along with the options that apply to them
	------------------------------------------------------------------------------------------- */


class AllFiles : protected FileCycler
{
public:
	AllFiles(void)
		{
		RecurseFiles=false;
		CrossHair=false;
		ZeroColZero=false;
		MoveUVs=false;
		AllowRotate=true;
		ForceOffsets=false;
		ShrinkToFit=true;
		m_allocateAs16bit=false;
		MaxSize=0;
		m_noSort=false;
		}

	void	SetFileRecursion(bool NewRecurseFiles)	{RecurseFiles=NewRecurseFiles;}
	void	SetCrossHair(bool NewCrossHair)			{CrossHair=NewCrossHair;}
	void	SetZeroColZero(bool NewColZero)			{ZeroColZero=NewColZero;}
	void	SetMoveUVs(bool NewMoveUVs)				{MoveUVs=NewMoveUVs;}
	void	SetAllowRotate(bool NewAllowRotate)		{AllowRotate=NewAllowRotate;}
	void	SetForceOffsets(bool NewForceOffsets)	{ForceOffsets=NewForceOffsets;}
	void	SetShrinkToFit(bool NewSetShrinkToFit)	{ShrinkToFit=NewSetShrinkToFit;}
	void	SetXOff(int NewXOff)					{XOff=NewXOff;}
	void	SetYOff(int NewYOff)					{YOff=NewYOff;}
	void	SetMaxSize(int New)						{MaxSize=New;}

	int		GetMaxSize(void) const					{return(MaxSize);}

	FIVec const & GetFileInfoVector(void) const		{return(AllFileInfos);}

	void setAllocateAs16Bit(bool newVal)			{m_allocateAs16bit=newVal;}
	void setNoSort(void)							{m_noSort=true;}

	void	SortOrder();
	void	AddFile(const char * Name);
	void	AddMemFrame(char const * FName,Frame &Frame);
	void	ReadRepFile(char const * Name);


protected:
	vector<GString>		UniqueTexturesSoFar;
	vector<GString>		AllExternalSharedTextures;

	void	FileCallback(char const * FName,int FileNum);

	bool	ShrinkToFit;
	bool	RecurseFiles;
	bool	CrossHair;
	bool	ZeroColZero;
	bool	MoveUVs;
	bool	AllowRotate;
	bool	ForceOffsets;
	
	bool	m_allocateAs16bit;
	bool	m_noSort;

	int		XOff,YOff;

	FIVec	AllFileInfos;
	int		MaxSize;
};


#endif	/* _ */

/*===========================================================================
 end */
