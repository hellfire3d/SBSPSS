//===========================================================================
//
//		BITS.HPP
//
//		Author:  Graeme Webb
//		Created: 3rd August 1996
//		Project: General C++ Library
//		Purpose: Bit stream declarations
//
//		Copyright (c) 1996 Graeme Webb
//
//===========================================================================
// Revision history:
//===========================================================================

#ifndef __BITS_HPP__
#define __BITS_HPP__

//===========================================================================
#include <gtypes.h>

//===========================================================================

class BitStream
{
	byte *	baseptr;
	byte *	currptr;
	ulong	bitpos;
	byte	mask;
	void	IncCurPtr(void);

public:

	BitStream(byte *ptr);
	~BitStream();

	void PutBit(uint bit);
	void PutBits(ulong bits, uint len);
	uint GetBit();
	ulong GetBits(uint len);
	void Align(uint bytes=1);

	byte *CurrPtr() { return currptr; }
	ulong BitLength() { return bitpos; }
	ulong ByteLength() { return (bitpos+7)/8; }
};

//===========================================================================
#endif
