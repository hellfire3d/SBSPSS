#include "system\global.h"
#include <string.h>
#include "utils\pak.h"


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
#define UNPAKBUFFERSIZE 4096
#define STRMAX			80
#define BUFFERSIZE		30000
#define TRIGGER			20000
#define MAXUNIQUE		127

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct Block
{
	int		data[128];
	bool	blockrep;
	int		blocksize;
	int		blockoffset;

	u8 *	Dest;
	int		outsize;

	virtual void fputc(u8 Val)
		{
		*Dest=Val;
		Dest++;
		outsize++;
		}

	void writeBlock(void);
};

struct CountBlock : Block
{
	virtual void fputc(u8 Val)
		{
		Dest++;
		outsize++;
		}
};

/*----------------------------------------------------------------------
	Vars
	---- */
static int s_lastAmountOfCompressedData;

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void writeblock(struct block *theblock);

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
void Block::writeBlock(void)
{
	if ((blockrep) && (blocksize == 0))
		{
		/* Terminator */
		fputc(128);
		fputc(0);
		}
	else if (blockrep)
		{
		fputc(blockoffset);
		fputc(blocksize);
		}
	else /* Unique bytes */
		{
		fputc(blocksize & 127);

		for (int i = 0; i <= blocksize; i++)
			fputc(data[i]);
		}

	// Get ready for next block
	blockrep = false;
	blockoffset = 0;
	blocksize = -1;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static bool memequ(u8 const * p1,u8 const * p2,int size)
{
	bool	same;

	same=true;

	for (int f=0;f<size && same;f++)
		same=p1[f]==p2[f];

	return(same);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int lowLevelPak(u8 * Dest,u8 const * buffer,int insize,Block & theblock)
{
	long				begin, end, bestlength;
	int					offset, bestoffset;
	unsigned char const *		theptr;
	unsigned char const *		ptr1;
	unsigned char const *		ptr2;
	unsigned char const *		ptr3;
	long				BACKDIST, FORWARDDIST, MINBLOCK;

	theblock.Dest=Dest;
	theblock.outsize=0;
	theblock.blockrep=false;

	BACKDIST = -128;
	FORWARDDIST = 255;
	MINBLOCK = 3;

	int  inpos = 0;

	theblock.blocksize = -1;
	theblock.data[++theblock.blocksize] = buffer[inpos++];
	theblock.data[++theblock.blocksize] = buffer[inpos++];

	while (inpos < insize)
		{
		/* See if we should load new data into the buffer */

		begin = -inpos;
		end = insize - inpos;

		if (begin < BACKDIST)
			begin = BACKDIST;

		if (end > FORWARDDIST)
			end = FORWARDDIST;

		bestoffset = begin;
		bestlength = 1;
		theptr = buffer + (inpos);

		ptr1 = buffer + (inpos + begin);

		for (offset = begin; offset < 0; offset++)
			{
			if (*ptr1 == *theptr)
				{
				if (memequ(ptr1, theptr, bestlength + 1))
					{
					bestlength++;
					bestoffset = offset;
					ptr2 = ptr1 + bestlength;
					ptr3 = theptr + bestlength;

					while (*ptr2 == *ptr3)
						{
						ptr2++;
						ptr3++;
						bestlength++;
						if (bestlength >= end)
							break;
						}
					}
				}

			if (bestlength >= end)
				{
			 	bestlength = end;
				break;
				}

			ptr1++;
			}


		if (bestlength < MINBLOCK)
			{
			/* See if last block is unique */

			if (theblock.blockrep) /* Flush previous special block */
				{
				theblock.writeBlock();
				theblock.data[++theblock.blocksize] = buffer[inpos++];
				}
			else 
				{
				/* Add to it */

				if (theblock.blocksize >= MAXUNIQUE)
					theblock.writeBlock();

				theblock.data[++theblock.blocksize] = buffer[inpos++];
				}
			}
		else
			{
			/* We have found a match */
			theblock.writeBlock();
			theblock.blockrep = true;
			theblock.blocksize = bestlength;
			theblock.blockoffset = bestoffset;
			inpos += bestlength;
			}
		}

	/* Flush buffer */
	
	theblock.writeBlock();

	/* Terminate file */

	theblock.blockrep = true;
	theblock.blocksize = 0;
	theblock.blockoffset = 0;
	theblock.writeBlock();

	return(theblock.outsize);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int PAK_doPak(u8 * Dest,u8 const * buffer,int insize)
{
	Block	outBlock;
	return(lowLevelPak(Dest,buffer,insize,outBlock));
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int	PAK_findPakSize(u8 const * buffer,int insize)
{
	CountBlock	outBlock;
	return(lowLevelPak(NULL,buffer,insize,outBlock));
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int PAK_doUnpak(u8 * Dest,u8 const * Source)
{
	int			outsize = 0;
	u8 const * 	originalSource;

	originalSource=Source;

	while (1)                           /* ie, continue until end mark */
		{
		u8 const *	From;

		int		size;
		int		ch;

		ch = *Source++;

		if (ch < 128)					/* if it is in range {0..127} then */
			{
			size = (ch + 1);
			From=Source;
			Source+=size;
			}
		else
			{
			size = *Source++;

			if (size == 0)				/* distance == 0  =>  end of file */
				break;

			From=Dest+(s8)ch;
			}

		u8 *	endAddr;
		endAddr=(u8*)(u32(From)+size);

		outsize += size;

		while (From!=endAddr)
			{
			*Dest=*From;
			Dest++;
			From++;
			}
		}

	s_lastAmountOfCompressedData=Source-originalSource;

	return(outsize);
}

int	PAK_getLastAmountOfDataRead(void)
{
	return(s_lastAmountOfCompressedData);
}

/*===========================================================================
 end */




