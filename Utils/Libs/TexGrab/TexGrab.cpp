/*=========================================================================

	TexGrab.CPP	

	Author:  Dave @ Fareham (based on work by Gary Liddon)
	Created: 011200
	Project: SpongeBob
	Purpose: Main file for the new TexGrab lib, based on ParkGrab

===========================================================================*/

					
/*----------------------------------------------------------------------
	Includes
	-------- */

#include	"TexGrab.h"

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */


/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CTexGrab::CTexGrab()
{
		m_PagePlacements=false;
		m_compressTpages=false;
		m_noWriteTpages=false;
		m_useHalfTpage=false;
		m_AnimatedHeadersOnly=false;
		m_DontOutputBoxes=false;
		m_AlignHeaders=false;

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CTexGrab::~CTexGrab()
{
}

  /*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTexGrab::Process()
{
		MySprSet.setDebug(DebugFlag);
		MySprSet.setHalfTpage(m_useHalfTpage);
		MySprSet.setAnimatedHeader(m_AnimatedHeadersOnly);
		MySprSet.setDontOutputBoxes(m_DontOutputBoxes);
		MySprSet.setAlignHeaders(m_AlignHeaders);
		MySprSet.setCompressTpages(m_compressTpages);
		MySprSet.setNoWriteTpages(m_noWriteTpages);

		MyFiles.SortOrder();

		if (MyFiles.GetMaxSize())
			MySprSet.SetMaxSize(MyFiles.GetMaxSize());

		MySprSet.AddFiles(MyFiles.GetFileInfoVector());

		if (m_IncOutFile)
			MySprSet.SetIncOutFile(m_IncOutFile);

		if (m_SprFile)
			MySprSet.WriteSprFile(m_SprFile);

		if (m_OutFile)
			{
			if (m_PagePlacements)
				{
				MySprSet.Write(m_OutFile,m_PageBase,m_WidthPages,m_HeightPages);
				MySprSet.CreateTexInfo(TexInfo);

				if (m_OutLbm)
					MySprSet.WriteLBM(m_OutLbm);

				if (m_ReportFile)
					MySprSet.WriteReport(m_ReportFile);

				if (m_rawTpageFile)
					MySprSet.writeRawTPage(m_rawTpageFile);

				}
			else
				GObject::Error(ERR_FATAL,"-t option hasn't been specified");
			}
}

/*----------------------------------------------------------------------
	Function:	void	CTexGrab::AddFile(char *Name)
	Purpose:	Add bitmaps to file list
	Params:		Name = Filename
  ---------------------------------------------------------------------- */
int		CTexGrab::AddFile(const char *Name)
{
GString	UpperName(Name);
			
		UpperName.Upper();

int		Idx=MyFiles.AddFile(UpperName);
		return(Idx);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:	
	Params:		
  ---------------------------------------------------------------------- */
int		CTexGrab::AddMemFrame(const char *Name,Frame &Frame)
{
GString	UpperName(Name);
			
		UpperName.Upper();

int		Idx=MyFiles.AddMemFrame(UpperName,Frame);
		return(Idx);
}

vector<FileInfo>	const &CTexGrab::GetTexInfoList()
{
		return(MyFiles.GetFileInfoVector());

}
/*


/*===========================================================================
 end */
