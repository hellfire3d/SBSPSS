/*=========================================================================

	AllFiles.CPP	

	Author:  Gary 
	Created: 011200
	Project: SpongeBob
	Purpose: File cycler stuff

===========================================================================*/

					
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

#include	"AllFiles.h"

/*----------------------------------------------------------------------
	Function:	void AllFiles::AddFile(const char * Name)
	Purpose:	Add a file to the list, it takes all the current
				properties that the AllFiles object has.
	Params:		Name of file to add
  ---------------------------------------------------------------------- */
int	AllFiles::AddFile(const char * Name)
{
int	ListSize=AllFileInfos.size();
	DoCycle(Name,RecurseFiles);
	return(ListSize);
}

/*----------------------------------------------------------------------
	Function:	void AllFiles::FileCallback(char const * FName,int FileNum)
  ---------------------------------------------------------------------- */
//#include <gintex.h>
#include <algorithm>

void AllFiles::FileCallback(char const * FName,int FileNum)
{
	FileInfo	MyInfo;
	bool		ThisZeroColZero;


	GFName		FileName(FName);

	GString		Ext(FileName.Ext());
	
	Ext.Lower();
	GFName	Name(FName);

	if (Name.File()[0]=='+')
		ThisZeroColZero=true;
	else
		ThisZeroColZero=ZeroColZero;

	if (Name.File()[0]=='!')
		m_allocateAs16bit=true;
	else
		m_allocateAs16bit=false;

	if (Name.File()[0] == '@')
	{
		ThisZeroColZero = true;
		m_allocateAs16bit = true;
	}

	GString TheFile(FName);
	TheFile.Lower();
		
	MyInfo.SetInfo(TheFile,CrossHair,ThisZeroColZero,MoveUVs,AllowRotate,ShrinkToFit,m_allocateAs16bit);
	if (DebugFlag) cout<<"Add image file "<<TheFile<<endl;

	if (ForceOffsets)
		MyInfo.SetForceOffsets(XOff,YOff);
	
	AllFileInfos.resize(AllFileInfos.size()+1);
	AllFileInfos[AllFileInfos.size()-1]=MyInfo;

}

int	AllFiles::AddMemFrame(char const * FName,Frame &Frame)
{
	FileInfo	MyInfo;
	bool		ThisZeroColZero;
	GFName	Name(FName);

		if (Name.File()[0]=='+')
			ThisZeroColZero=true;
		else
			ThisZeroColZero=ZeroColZero;

		if (Name.File()[0]=='!')
			m_allocateAs16bit=true;
		else
			m_allocateAs16bit=false;

		if (Name.File()[0] == '@')
		{
			ThisZeroColZero = true;
			m_allocateAs16bit = true;
		}

		GString TheFile(FName);
		TheFile.Lower();
			
		MyInfo.SetInfo(TheFile,CrossHair,ThisZeroColZero,MoveUVs,AllowRotate,ShrinkToFit,m_allocateAs16bit,&Frame);
		if (DebugFlag) cout<<"Add MemFrame "<<TheFile<<endl;

		if (ForceOffsets)
			MyInfo.SetForceOffsets(XOff,YOff);
int	ListSize=AllFileInfos.size();
		AllFileInfos.resize(ListSize+1);
		AllFileInfos[ListSize]=MyInfo;
	return(ListSize);
}

/*----------------------------------------------------------------------
	Function:	void AllFiles::FileCallback(char const * FName,int FileNum)
  ---------------------------------------------------------------------- */
void AllFiles::SortOrder()
{
	if (!m_noSort)
		{
		int i,j;
		FileInfo MyInfo;
		int count = AllFileInfos.size();

		for (i=0; i<count; i++)
			{
			for (j=0; j<count; j++)
				{
				if (strcmp(AllFileInfos[i].GetFileName(), AllFileInfos[j].GetFileName())<0)
					{
					MyInfo = AllFileInfos[i];
					AllFileInfos[i] = AllFileInfos[j];
					AllFileInfos[j] = MyInfo;
					}
				}
			}
		}
}

/*===========================================================================
 end */
