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

#include <ginio.h>
#include <repread.h>

#include "sprset.h"

#include	"AllFiles.h"

/*----------------------------------------------------------------------
	Function:	void AllFiles::AddFile(const char * Name)
	Purpose:	Add a file to the list, it takes all the current
				properties that the AllFiles object has.
	Params:		Name of file to add
  ---------------------------------------------------------------------- */
void AllFiles::AddFile(const char * Name)
{
	DoCycle(Name,RecurseFiles);
}

/*----------------------------------------------------------------------
	Function:	void AllFiles::FileCallback(char const * FName,int FileNum)
  ---------------------------------------------------------------------- */
#include <gintex.h>
#include <algorithm>

void AllFiles::FileCallback(char const * FName,int FileNum)
{
	FileInfo	MyInfo;
	bool		ThisZeroColZero;


	GFName		FileName(FName);

	GString		Ext(FileName.Ext());
	
	Ext.Lower();

	if (Ext=="gin")
		{
		vector<GString> NonSharedTextures;

		CScene		MyFile;
		MyFile.Load(FName);

		MyFile.GetNonSharedTextures(AllExternalSharedTextures,NonSharedTextures);


		vector<GString> NewEntrys;

		for (int f=0;f<NonSharedTextures.size();f++)
			{
			if (find(UniqueTexturesSoFar.begin(),UniqueTexturesSoFar.end(),NonSharedTextures[f]) == UniqueTexturesSoFar.end())
				{
				NewEntrys.push_back(NonSharedTextures[f]);
				UniqueTexturesSoFar.push_back(NonSharedTextures[f]);
				}
			}


		for (f=0;f<NewEntrys.size();f++)
			FileCallback(NewEntrys[f],0);

		cout<<"Read gin file *"<<FName<<"*"<<endl;
		}
	else
		{
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
		cout<<"Add image file "<<TheFile<<endl;

		if (ForceOffsets)
			MyInfo.SetForceOffsets(XOff,YOff);
		
		AllFileInfos.resize(AllFileInfos.size()+1);
		AllFileInfos[AllFileInfos.size()-1]=MyInfo;
		}
}

void AllFiles::AddMemFrame(char const * FName,Frame &Frame)
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
		cout<<"Add MemFrame "<<TheFile<<endl;

		if (ForceOffsets)
			MyInfo.SetForceOffsets(XOff,YOff);
		
		AllFileInfos.resize(AllFileInfos.size()+1);
		AllFileInfos[AllFileInfos.size()-1]=MyInfo;

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

/*----------------------------------------------------------------------
	Function:	void AllFiles::ReadRepFile(char const * Name)
	Purpose:	Read in rep file for shared textures
	Params:		Name of rep file
  ---------------------------------------------------------------------- */
void AllFiles::ReadRepFile(char const * Name)
{
	vector<RepItem>		MyItems;

	::readRepFile(Name,MyItems);

	for (int f=0;f<MyItems.size();f++)
		AllExternalSharedTextures.push_back(MyItems[f].m_texName);

	cout<<"Read report file "<<Name<<endl;
}

/*===========================================================================
 end */
