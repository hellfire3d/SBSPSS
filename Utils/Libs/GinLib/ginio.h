/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

#ifndef __GINIO_H__
#define __GINIO_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#pragma warning( disable : 4786 )
#include <vector>
#include <fstream>
#include <misc.hpp>
#include <gstring.hpp>

/*	Glib
	---- */

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class GinChunk : public GObject
{
public:
			GinChunk(void);
	virtual	~GinChunk(void);

	void LoadFile(Gifstream & In);

	virtual char const * GetName(void) const
		{return("DEFAULT");	}

	void SetNameSize(char const * NewName,int NewSize)
		{
		Name=NewName;
		Size=NewSize;
		}
	virtual void Load(Gifstream & In);

	virtual GinChunk * MakeNew(void)
		{return(new GinChunk);}

protected:


	void		DumpData(void);

	GString	Name;
	int		Size;
	std::vector<u8> Data;
};


class GinFile : public GObject
{
public:
	GinFile(void)
		{
		ReadUnknownChunks=false;
		}

	~GinFile(void)
		{
		int	i;
		for (i=0; i<ChunkHandlers.size(); i++) delete ChunkHandlers[i];
		ChunkHandlers.clear();

		for (i=0; i<Chunks.size(); i++) delete Chunks[i];
		Chunks.clear();
		}
	

	void	ResetChunks()
		{
		for (int f=0;f<Chunks.size();f++)	delete Chunks[f];
		Chunks.clear();
		}
	void SetReadUnknownChunks(bool New)
		{
		ReadUnknownChunks=New;
		}

	static void AddHandler(GinChunk * New);

	void Load(char const * File)
		{
		ResetChunks();
		Gifstream In(Gifstream::LITTLE_ENDIAN);

		In.open(File,std::ios::binary);

		if (In)
			{
			GinHdr	MyHdr;

			while (MyHdr.ReadHdr(In))
				{
				int			ChunkLenAligned;
				int			ChunkPos;

				GinChunk *	ChunkReader;

				ChunkReader=GetHandler(MyHdr.Name);

				if (!ChunkReader && ReadUnknownChunks)
					ChunkReader=new GinChunk;
				
				ChunkLenAligned=GU_AlignVal(MyHdr.Size,4);

				ChunkPos=In.tellg();

				if (ChunkReader)
					{
					ChunkReader->SetNameSize(MyHdr.Name,MyHdr.Size);
					ChunkReader->Load(In);
					Chunks.push_back(ChunkReader);
					}

				if (In)
					In.seekg(ChunkLenAligned+ChunkPos,std::ios::beg);
				else
					{
					Error(ERR_FATAL,"Error loading chunk");
					}
				}

			In.close();
			}
		else
			Error(ERR_FATAL,"Can't open gin file %s",File);
		}

	 int FindChunks(char const * Type,std::vector<GinChunk const *> & RetChunks)
		{
		int NumOfChunks;

		NumOfChunks=0;

		for (int f=0;f<Chunks.size();f++)
			{
			GString		Name;

			Name=Chunks[f]->GetName();

			if (Name==Type)
				{
				RetChunks.push_back(Chunks[f]);
				NumOfChunks++;
				}
			}

		return(NumOfChunks);
		}

protected:

	struct GinHdr
		{
			GinHdr(void)
				{Size=0;}

			bool ReadHdr(Gifstream & In);

			bool PeekHdr(Gifstream & In)
				{
				int		InPos;
				bool	Ret;

				InPos=In.tellg();

				Ret=ReadHdr(In);

				if (Ret)
					In.seekg(InPos,std::ios::beg);

				return(Ret);
				}

			GString	Name;
			int		Size;
		};


	std::vector<GinChunk *> Chunks;
	bool		ReadUnknownChunks;

	static std::vector<GinChunk *>	ChunkHandlers;
	static GinChunk * GetHandler(char const * Name);
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */



/*---------------------------------------------------------------------- */

#endif	/* __GINIO_H__ */

/*===========================================================================
 end */
