//===========================================================================
//
//		BITS.CPP
//
//		Author:  Graeme Webb
//		Created: 3rd August 1996
//		Project: General C++ Library
//		Purpose: Bit stream object
//
//		Copyright (c) 1996 Graeme Webb
//
//===========================================================================
// Revision history:
//===========================================================================

#include "gtypes.h"
#include "bits.hpp"

//===========================================================================

BitStream::BitStream(byte *ptr)
{
	baseptr = (byte *) ptr;
	currptr = (byte *) ptr;
	bitpos = 0;
	mask = 0x01;
}

BitStream::~BitStream()
{
}

void BitStream::IncCurPtr(void)
{
	currptr++;
}


void BitStream::PutBit(uint bit)
{
	if (mask == 0x01)
	{
		*currptr = (bit ? mask : 0);
		bitpos++;
		mask <<= 1;
	}
	else
	{
		*currptr |= (bit ? mask : 0);
		bitpos++;
		if (mask == 0x80)
		{
			IncCurPtr();
			mask = 0x01;
		}
		else
		{
			mask <<= 1;
		}
	}
}

void BitStream::PutBits(ulong bits, uint len)
{
	uint i;

	for (i = 0; i < len; i++)
	{
		PutBit(bits & 1);
		bits >>= 1;
	}
}

uint BitStream::GetBit()
{
	uint bit;

	bit = (*currptr & mask) ? 1 : 0;
	bitpos++;

	if (mask == 0x80)
	{
		IncCurPtr();
		mask = 0x01;
	}
	else
	{
		mask <<= 1;
	}

	return bit;
}

ulong BitStream::GetBits(uint len)
{
	uint i;
	ulong bits, mbit;

	bits = 0;
	mbit = 1;

	for (i = 0; i < len; i++)
	{
		if (GetBit())
			bits |= mbit;
		mbit <<= 1;
	}

	return bits;
}

static inline ulong RoundUp(ulong value, uint align)
{
	value += align - 1;
	return (value - value % align);
}

void BitStream::Align(uint bytes)
{
	ulong bytepos, nextpos;

	bytepos = ByteLength();
	nextpos = RoundUp(bytepos,bytes);

	currptr = baseptr + bytepos;

	while (bytepos < nextpos)
	{
		*currptr=0;
		IncCurPtr();
		bytepos++;
	}

	bitpos = bytepos * 8;
	mask = 0x01;
}

//===========================================================================
