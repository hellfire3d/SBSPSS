/*=========================================================================

	EXPUPROP.CPP

	Author:  Dave
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "AsciiExp.h"


/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*	Graphics
	-------- */

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

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
const char	UPROP_ID[]			=	MAKE_LONG( 'P', 'R', 'O', 'P' );

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void AsciiExp::ExportProp( INode* node )
{
ObjectState	os;
Object *	obj;
TSTR		UPBuffer;
Uint32		Len;
Uint32		ModNum=nCurNode;
char		*TxtPtr;

	os = node->EvalWorldState( ip->GetAnimRange().Start() );
	obj = node->EvalWorldState( ip->GetAnimRange().Start() ).obj;

	if (!os.obj)				return;
//	if (!obj->IsRenderable())	return;
	if (node->IsNodeHidden())	return;
//	if (!obj || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)	return; // Safety net. This shouldn't happen.

	node->GetUserPropBuffer(UPBuffer);
	Len=UPBuffer.Length();//strlen(UPBuffer);
	Len=(Len+3)&0xfffc;
	TxtPtr=UPBuffer.data();
	WriteChunkHdr( (char*)UPROP_ID, 0 );
	fwrite( &ModNum, sizeof(Uint32), 1, expStream );
	fwrite( &Len, sizeof(Uint32), 1, expStream );
	fwrite( TxtPtr, sizeof(char), Len, expStream );
}
