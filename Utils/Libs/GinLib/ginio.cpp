/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */
#include "ginio.h"


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
using namespace std;

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
	Data
	---- */
vector<GinChunk *>	GinFile::ChunkHandlers;

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GinChunk::GinChunk(void)
{
	Size=0;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GinChunk::~GinChunk(void)
{
	DumpData();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void GinChunk::Load(Gifstream & In)
{
	Data.resize(Size);
	In.read((char *)(&Data[0]),Size);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void GinFile::AddHandler(GinChunk * New)
{
	ChunkHandlers.push_back(New);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
GinChunk * GinFile::GetHandler(char const * Name)
{
	GString ChunkName(Name);

	GinChunk * RetChunk;

	RetChunk=NULL;

	for (int f=0;f<ChunkHandlers.size() && !RetChunk;f++)
		{
		if (ChunkName==GString(ChunkHandlers[f]->GetName()))
			RetChunk=ChunkHandlers[f]->MakeNew();
		}


	return(RetChunk);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool GinFile::GinHdr::ReadHdr(Gifstream & In)
{
	vector<char>	Str;
	bool				Done;
	GString				RetStr;

	Done=false;

	Str.reserve(1000);
	

	while (In && !In.eof() && !Done)
		{
		char	c=In.get();
		
		if (!c)
			Done=true;
		
		Str.push_back(c);
		}

	if (Done)
		{
		Name=&Str[0];
		
		In.Align(4);

		if (In)
			{
			Size=In.Get32();
			}
		else
			Done=false;
		}


	return(Done);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void GinChunk::DumpData(void)
{
	Size=0;
	Data.resize(0);
}

/*===========================================================================
 end */
