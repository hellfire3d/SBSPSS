/*
	Program:		unlznp.c
	Author:		Nick Pelling
	Function:	Decompresses an LZNP-compressed file
	Note:			Copyright (c) 1993-1994 Nick Pelling. All rights reserved.
*/

/*
	Notes:

		The basic procedure is:

		(a)	Read a control byte, and examine each bit in turn, LSB...MSB
		(b)	if bit is clear, copy a byte from input stream
		(c)	...else read another byte in.
				if >= 0x60, this means "copy a pair from nearby"
		(d)	...else split it up into a length, and high 4 bits of offset
		(e)	read the low order 8 bits, and merge to form (-ve) offset
		(f)	if (offset == 0), then exit: this terminates decompression
		(g)	if length is maximum length, read superlength from next byte
		(h)	copy length bytes from (-ve) offset. Note: this may overlap!
		(i)	If no more bits in control byte, read another control byte
		(j)	Loop.

	...and that's it.
*/

#include "lznp.h"

#define SMALLEST_LEN	2						/* smallest len usable at all */
#define MIN_LEN		3						/* smallest short length encodable */
#define MAX_LEN		(MIN_LEN + 6-2)	/* largest  short length encodable */
#define MIN_SUPERLEN	(MIN_LEN + 6-1)	/* shortest long length encodable */
#define MAX_SUPERLEN	(MIN_SUPERLEN + 254)	/* largest  long length encodable */
#define SUPERLEN_CODE (MIN_SUPERLEN - MIN_LEN)	/* means "read superlength" */


typedef unsigned char UBYTE;				/* just for convenience */


int LZNP_Decode(UBYTE *in, UBYTE *out)
{
	int	i, j;
	unsigned int flags;								/* now works with 16-bit ints */
	UBYTE * OriginalOut=out;

	for (flags=0;;)
	{
		if (((flags >>= 1) & 0xff00) == 0)
		{
			flags = (*in++) | 0xff00;				/* uses higher byte cleverly */
		}													/* (to count eight) */

		if (!(flags & 1))
		{
			*out++ = *in++;
		}
		else
		{
			i = *in++;

			if (i >= 0x60)
			{
				i = 0x100 - i;			/* i = copy offset */
				j = SMALLEST_LEN;		/* j = copy length */
			}
			else
			{
				j = i >> 4;
				i = (i & 0x0F) << 8;
				i |= *in++;

				if (i == 0)				/* offset of zero terminates LZNP data */
					break;

				if (j != SUPERLEN_CODE)
					j += MIN_LEN;
				else
					j = MIN_SUPERLEN + (*in++);
			}

			for (i=-i,j++; --j; out++)
			{
				out[0] = out[i];
			}
		}
	}

	/* Return size */
	return(out-OriginalOut);
}


