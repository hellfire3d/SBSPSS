/*=========================================================================

	sprbank.cpp

	Author:		PKG
	Created: 
	Project:	PRLSR
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef __GFX_SPRBANK_H__
#include "gfx\sprbank.h"
#endif

#ifndef _FILEIO_HEADER_
#include "FileIO/FileIO.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "Mem\Memory.h"
#endif

#ifndef __PRIM_HEADER__
#include "gfx\prim.h"
#endif

#ifndef __MATHTABLE_HEADER__
#include "utils\mathtab.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */


/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

// Linked list of loaded instances
static SpriteBankInstance	*s_bankList = NULL;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SpriteBankInstance::SpriteBankInstance( SpriteBankInstance *_next )
{
	m_frameHdr=NULL;
	m_file=FileEquate( -1 );
	m_refCount=0;
	m_next=_next;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SpriteBankInstance::~SpriteBankInstance()
{
	ASSERT( !m_frameHdr );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void SpriteBankInstance::load( FileEquate _file )
{
	if( m_refCount )
	{
		ASSERT( _file == m_file );
		m_refCount++;
	}
	else
	{
		m_tpageDesc=TPLoadTexWithHeaders(_file,&m_frameHdr);

		m_file=_file;
		m_refCount=1;
	}

}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:	true if there are no more references to this bank
  ---------------------------------------------------------------------- */
int SpriteBankInstance::dump()
{
	int ret=false;
	ASSERT(m_frameHdr);

	m_refCount--;
	if( m_refCount == 0 )
	{
		TPFree(m_tpageDesc);
		MemFree(m_frameHdr);
		m_frameHdr=NULL;
		ret=true;
	}
	return ret;
}





/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SpriteBank::SpriteBank()
{
	m_SpriteBankInstance = NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
SpriteBank::~SpriteBank()
{
	ASSERT(!m_SpriteBankInstance);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		also creates the instance if required
	Returns:
  ---------------------------------------------------------------------- */
void SpriteBank::load( FileEquate _file )
{
	ASSERT(!m_SpriteBankInstance);

	if( !s_bankList )
	{
		// first sprite bank load
		s_bankList=new ("SpriteBank::load") SpriteBankInstance( NULL );
		s_bankList->load( _file );
		m_SpriteBankInstance=s_bankList;
	}
	else
	{
//		ASSERT(!"Sprite Bank Already Loaded");
		SpriteBankInstance *sbiPtr=s_bankList;
		do
		{
			if( sbiPtr->getFile() == _file )
			{
				// file already loaded - reference it
				sbiPtr->load( _file );
				m_SpriteBankInstance=sbiPtr;
				return;
			}
			sbiPtr=sbiPtr->getNext();
		}
		while( sbiPtr );

		// file not loaded - create new instance
		sbiPtr=s_bankList;
		s_bankList=new ("SpriteBank::load") SpriteBankInstance( sbiPtr );
		s_bankList->load( _file );
		m_SpriteBankInstance=s_bankList;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:		also deletes the instance if required
	Returns:
  ---------------------------------------------------------------------- */

void SpriteBank::dump()
{
	ASSERT( m_SpriteBankInstance );
	if( m_SpriteBankInstance->dump() )
	{
		// just removed last last reference - delete instance
		if( m_SpriteBankInstance==s_bankList)
		{
			// first entry in bank list
			s_bankList=m_SpriteBankInstance->getNext();
		}
		else
		{
			SpriteBankInstance *sbiPtr=s_bankList;
			while( sbiPtr->getNext() != m_SpriteBankInstance )
			{
				sbiPtr=sbiPtr->getNext();
			};
			sbiPtr->setNext( sbiPtr->getNext()->getNext() );
		}

		delete m_SpriteBankInstance;
	}
	m_SpriteBankInstance=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SpriteBank::prepareFT4(POLY_FT4 *_ft4,sFrameHdr *_fh,int _x,int _y,bool _xFlip,bool _yFlip)
{
		int		W=_fh->W,H=_fh->H;

		setShadeTexPolyFT4(_ft4);
		setShadeTex(_ft4,0);
		setRGB0(_ft4,128,128,128);
		if (_xFlip)
			{
			_x-=_fh->XOfs;
			_x-=W;
			}
		else
			_x+=_fh->XOfs;

		if (_yFlip)
			{
			_y-=_fh->YOfs;
			_y-=H;
			}
		else
			_y+=_fh->YOfs;

		setXYWH(_ft4,_x,_y,W,H);
		setUVTp(_fh,_ft4,_xFlip,_yFlip);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SpriteBank::prepareGT4(POLY_GT4 *_gt4,sFrameHdr *_fh,int _x,int _y,bool _xFlip,bool _yFlip)
{
		int		W=_fh->W,H=_fh->H;

		setShadeTexPolyGT4(_gt4);
		setShadeTex(_gt4,0);
//		setRGB0(_gt4,128,128,128);
		if (_xFlip)
			{
			_x-=_fh->XOfs;
			_x-=W;
			}
		else
			_x+=_fh->XOfs;

		if (_yFlip)
			{
			_y-=_fh->YOfs;
			_y-=H;
			}
		else
			_y+=_fh->YOfs;

		setXYWH(_gt4,_x,_y,W,H);
		setUVTp(_fh,_gt4,_xFlip,_yFlip);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	Pass Z scale as 8 bit fraction, 256 = 1:1, 512 = enlarge X2, etc.
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SpriteBank::prepareFT4Scaled(POLY_FT4 *_ft4,sFrameHdr *_fh,int _x,int _y,bool _xFlip,bool _yFlip,long _z)
{
	int W=_fh->W,H=_fh->H;

	W *= _z;
	H *= _z;
	W>>=8;
	H>>=8;

	// Why not center the thing in here!? (pkg)
	_x-=(((_fh->W*_z)>>8)-_fh->W)/2;
	_y-=(((_fh->H*_z)>>8)-_fh->H)/2;


	setShadeTexPolyFT4(_ft4);
	if (_xFlip)
		{
		_x-=_fh->XOfs;
		_x-=W;
		}
	else
		_z+=_fh->XOfs;

	_y+=_fh->YOfs;

	setXYWH(_ft4,_x,_y,W,H);
	setUVTp(_fh,_ft4,_xFlip,_yFlip);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:	NB: Still needs to be aspect corrected.. (PKG)
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SpriteBank::prepareFT4RotatedScaled(POLY_FT4 *_ft4,sFrameHdr *_fh,int _xCentre,int _yCentre,int _xScale,int _yScale,int _rot)
{
	int		halfW,halfH;
	int		ca,sa;
	int		cw,ch,sw,sh;
	int		width,height;

	halfW=(_fh->W*_xScale)>>(12+1);
	halfH=(_fh->H*_yScale)>>(12+1);
	ca=mcos(_rot);
	sa=msin(_rot);
	cw=(ca*halfW)>>12;
	ch=(ca*halfH)>>12;
	sw=(sa*halfW)>>12;
	sh=(sa*halfH)>>12;

	_ft4->x0=_xCentre-cw+sh;	_ft4->y0=_yCentre-sw-ch;
	_ft4->x1=_xCentre+cw+sh;	_ft4->y1=_yCentre+sw-ch;
	_ft4->x2=_xCentre-cw-sh;	_ft4->y2=_yCentre-sw+ch;
	_ft4->x3=_xCentre+cw-sh;	_ft4->y3=_yCentre+sw+ch;

	setShadeTexPolyFT4(_ft4);
	setShadeTex(_ft4,0);
	setRGB0(_ft4,128,128,128);
//	setUVTp(_fh,_ft4,false,false);
////////////////////////////////	
	int		U=_fh->U;
	int		V=_fh->V;
	int		W=_fh->W;
	int		H=_fh->H;
	
	if (!_fh->Rotated) 
	{
		_ft4->u0=U;
		_ft4->u1=U+W;
		_ft4->u2=U;
		_ft4->u3=U+W;

		_ft4->v0=V;
		_ft4->v1=V;
		_ft4->v2=V+H-1;
		_ft4->v3=V+H-1;
	} 
	else 
	{
		_ft4->v0=V+W-1;
		_ft4->v2=V+W-1;
		_ft4->v1=V-1;
		_ft4->v3=V-1;

		_ft4->u0=U;
		_ft4->u1=U;
		_ft4->u2=U+H;
		_ft4->u3=U+H;
	}
//	if (_rot>1024 && _rot<3078 && )
	{
		_ft4->u1--; _ft4->u3--;
		_ft4->v2--; _ft4->v3--;
	}
	_ft4->tpage=_fh->TPage;
	_ft4->clut=_fh->Clut;
////////////////////////////////	


}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void SpriteBank::setUVTp(sFrameHdr *_fh,POLY_FT4 *_ft4,int _xFlip,int _yFlip)
{
	int		U=_fh->U;
	int		V=_fh->V;
	int		W=_fh->W;
	int		H=_fh->H;

	if (!_fh->Rotated) 
		{
		if (_xFlip) 
			{
			_ft4->u0=U+W-1;
			_ft4->u1=U-1;
			_ft4->u2=U+W-1;
	 		_ft4->u3=U-1;
			} 
		else 
			{
			_ft4->u0=U;
			_ft4->u1=U+W;
			_ft4->u2=U;
 			_ft4->u3=U+W;
	 		}
		if (_yFlip) 
			{
			_ft4->v0=V+H-1;
			_ft4->v1=V+H-1;
			_ft4->v2=V-1;
			_ft4->v3=V-1;
			} 
		else 
			{
			_ft4->v0=V;
			_ft4->v1=V;
			_ft4->v2=V+H;
			_ft4->v3=V+H;
			}
		} 
	else 
		{
		if (_xFlip) 
			{
			_ft4->v0=V;
			_ft4->v2=V;
			_ft4->v1=V+W;
			_ft4->v3=V+W;
			} 
		else 
			{
			_ft4->v0=V+W-1;
			_ft4->v2=V+W-1;
			_ft4->v1=V-1;
			_ft4->v3=V-1;
			}
		if (_yFlip) 
			{
			_ft4->u0=U+H-1;
			_ft4->u1=U+H-1;
			_ft4->u2=U-1;
			_ft4->u3=U-1;
			} 
		else 
			{
			_ft4->u0=U;
			_ft4->u1=U;
			_ft4->u2=U+H;
			_ft4->u3=U+H;
			}
		}
	_ft4->tpage=_fh->TPage;
	_ft4->clut=_fh->Clut;
}

void SpriteBank::setUVTp(sFrameHdr *_fh,POLY_GT4 *_gt4,int _xFlip,int _yFlip)
{
	int		U=_fh->U;
	int		V=_fh->V;
	int		W=_fh->W;
	int		H=_fh->H;

	if (!_fh->Rotated) 
		{
		if (_xFlip) 
			{
			_gt4->u0=U+W-1;
			_gt4->u1=U-1;
			_gt4->u2=U+W-1;
	 		_gt4->u3=U-1;
			} 
		else 
			{
			_gt4->u0=U;
			_gt4->u1=U+W;
			_gt4->u2=U;
 			_gt4->u3=U+W;
	 		}
		if (_yFlip) 
			{
			_gt4->v0=V+H-1;
			_gt4->v1=V+H-1;
			_gt4->v2=V-1;
			_gt4->v3=V-1;
			} 
		else 
			{
			_gt4->v0=V;
			_gt4->v1=V;
			_gt4->v2=V+H;
			_gt4->v3=V+H;
			}
		} 
	else 
		{
		if (_xFlip) 
			{
			_gt4->v0=V;
			_gt4->v2=V;
			_gt4->v1=V+W;
			_gt4->v3=V+W;
			} 
		else 
			{
			_gt4->v0=V+W-1;
			_gt4->v2=V+W-1;
			_gt4->v1=V-1;
			_gt4->v3=V-1;
			}
		if (_yFlip) 
			{
			_gt4->u0=U+H-1;
			_gt4->u1=U+H-1;
			_gt4->u2=U-1;
			_gt4->u3=U-1;
			} 
		else 
			{
			_gt4->u0=U;
			_gt4->u1=U;
			_gt4->u2=U+H;
			_gt4->u3=U+H;
			}
		}
	_gt4->tpage=_fh->TPage;
	_gt4->clut=_fh->Clut;
}
