/*=========================================================================

	TexGrab.h

	Author:		Dave
	Created:	011200
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__TEXGRAB_LIB_H__
#define	__TEXGRAB_LIB_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#include <vector>
#include <gtypes.h>
#include <gstring.hpp>
#include <misc.hpp>
#include <gfname.hpp>
#include <pcre.h>

#include "sprset.h"
#include "allfiles.h"

using namespace std;

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Class defintions
	---------------- */

class	CTexGrab
{
public:
		CTexGrab();
		~CTexGrab();

		int		AddFile(const char *Name);
		int		AddMemFrame(const char *Name,Frame &Frame);

		void	Process();

		void	CompressTpages(bool f)					{m_compressTpages=f;}
		void	NoWriteTpages(bool f)					{m_noWriteTpages=f;}
		void	UseHalfTpage(bool f)					{m_useHalfTpage=f;}
		void	AnimatedHeadersOnly(bool f)				{m_AnimatedHeadersOnly=f;}
		void	DontOutputBoxes(bool f)					{m_DontOutputBoxes=f;}
		void	AlignHeaders(bool f)					{m_AlignHeaders=f;}
		void	AllowRotate(bool f)						{MyFiles.SetAllowRotate(f);}
		void	FileRecursion(bool f)					{MyFiles.SetFileRecursion(f);}
		void	NoSort()								{MyFiles.setNoSort();}
		void	SetDebug(bool f)						{DebugFlag=f;}

		void	CrossHair(bool f)						{MyFiles.SetCrossHair(f);}
		void	ZeroColZero(bool f)						{MyFiles.SetZeroColZero(f);}
		void	MoveUVs(bool f)							{MyFiles.SetMoveUVs(f);}
		void	ShrinkToFit(bool f)						{MyFiles.SetShrinkToFit(f);}

		void	SetOutFile(const char *Name)			{m_OutFile=Name;}
		void	SetSprFile(const char *Name)			{m_SprFile=Name;}
		void	SetDebugOut(const char *Name)			{m_OutLbm=Name;}
		void	SetRepFile(const char *Name)			{m_ReportFile=Name;}
		void	SetIncFile(const char *Name)			{m_IncOutFile=Name;}
		void	SetRawTPageFile(const char *Name)		{m_rawTpageFile=Name;}

		void	MaxSize(int Size)						{MyFiles.SetMaxSize(Size);}
		void	SetTPage(int Base,int W,int H)			{m_PageBase=Base; m_WidthPages=W; m_HeightPages=H; m_PagePlacements=true;}
		void	ForceOffsets(int XOfs,int YOfs)			{MyFiles.SetXOff(XOfs); MyFiles.SetYOff(YOfs); MyFiles.SetForceOffsets(true);}
		
		vector<FileInfo>	const &CTexGrab::GetTexInfoList();

		std::vector<sTexOutInfo> &GetTexInfo()			{return(TexInfo);}


protected:

		AllFiles	MyFiles;
		SprSet		MySprSet;


		GString		m_OutFile;
		GString		m_OutLbm;
		GString		m_SprFile;
		GString		m_ReportFile;
		GString		m_IncOutFile;
		GString		m_rawTpageFile;

		int			m_PageBase;
		int			m_WidthPages;
		int			m_HeightPages;
		bool		m_PagePlacements;
		bool		m_compressTpages;
		bool		m_noWriteTpages;
		bool		m_useHalfTpage;
		bool		m_AnimatedHeadersOnly;
		bool		m_DontOutputBoxes;
		bool		m_AlignHeaders;
		bool		DebugFlag;		
		std::vector<sTexOutInfo> TexInfo;

};

/*---------------------------------------------------------------------- */

#endif

/*===========================================================================
 end */
