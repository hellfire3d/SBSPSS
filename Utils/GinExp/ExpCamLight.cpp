/*=========================================================================

	.CPP

	Author:  Tim Swann @ CLIMAX
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

void AsciiExp::ExportCamera(INode* node)
{
	INode *			target = node->GetTarget();
	CameraState		cs;
	TimeValue		t = 0;
	Interval		valid = FOREVER;
	Interval		animRange = ip->GetAnimRange();
	ObjectState		os = node->EvalWorldState(t);
	CameraObject *	cam = (CameraObject *)os.obj;


	cam->EvalCameraState(t, valid, &cs);

	if (target)
	{
		WriteChunkHdr( (char*)CAMERA_ID, 0 );
		fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );

		DumpCamPosSample( node );
		DumpCamPosSample( target );
	}
}


/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */

void AsciiExp::DumpCamPosSample(INode* node) 
{
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	TimeValue	t;
	int			delta = GetTicksPerFrame() * 1;
	Matrix3		tm;
	AffineParts ap;
	Point3		pos;
	Point3		opos;


	for (t=start; t<=end; t+=delta)
	{
		tm = node->GetNodeTM(t);

		decomp_affine(tm, &ap);

		pos = ap.t;

		opos.x = pos.x;
		opos.y = pos.z;
		opos.z = pos.y;

		fwrite( &opos.x, sizeof( float ), 1, expStream );		// WRITE CAMERA POSITION
		fwrite( &opos.y, sizeof( float ), 1, expStream );
		fwrite( &opos.z, sizeof( float ), 1, expStream );
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void AsciiExp::ExportLight(INode* node) 
{
	Interval			valid = FOREVER;
	Matrix3				mtx = node->GetNodeTM( 0 );
	ObjectState			os = node->EvalWorldState( 0 );
	GenLight *			light;
	LightState			ls;
	AffineParts			ap;
	Point3				pos;
	Point3				opos;
	float				ang[4];
	Point3				oang;
	Quat				rot;
	INode *				target;
	float				hotsize;
	float				fallsize;
	float				attenStart;
	float				attenEnd;



	if (!os.obj)	return;

	// EXPORT LIGHT
	WriteChunkHdr( (char*)LIGHT_ID, 0 );

	light = (GenLight*)os.obj;
	light->EvalLightState( 0, valid, &ls );
	fwrite( &ls.type, sizeof(long), 1, expStream );

	decomp_affine( mtx, &ap );

	pos = ap.t;

	opos.x = pos.x;
	opos.y = pos.z;
	opos.z = pos.y;

	fwrite( &opos.x, sizeof( float ), 1, expStream );		// WRITE LIGHT POSITION
	fwrite( &opos.y, sizeof( float ), 1, expStream );
	fwrite( &opos.z, sizeof( float ), 1, expStream );

	rot.x = ap.q.x;
	rot.y = ap.q.z;
	rot.z = ap.q.y;
	rot.w = ap.q.w;
	QuatToEuler( rot, ang);

	oang.x = ang[0];
	oang.y = ang[1];
	oang.z = ang[2];
//	fwrite( &oang.x, sizeof( float ), 1, expStream );		// WRITE LIGHT ROTATION
//	fwrite( &oang.y, sizeof( float ), 1, expStream );
//	fwrite( &oang.z, sizeof( float ), 1, expStream );

	target = node->GetTarget();
	if (target)
	{
		mtx = target->GetNodeTM( 0 );
		decomp_affine( mtx, &ap );

		pos = ap.t;

		opos.x = pos.x;
		opos.y = pos.z;
		opos.z = pos.y;
	}
	else
	{
//		opos.x = 0.f;
//		opos.y = 0.f;
//		opos.z = 0.f;
	}
	fwrite( &opos.x, sizeof( float ), 1, expStream );		// WRITE LIGHT POSITION
	fwrite( &opos.y, sizeof( float ), 1, expStream );
	fwrite( &opos.z, sizeof( float ), 1, expStream );

	fwrite( &ls.color.r, sizeof( float ), 1, expStream );
	fwrite( &ls.color.g, sizeof( float ), 1, expStream );
	fwrite( &ls.color.b, sizeof( float ), 1, expStream );
	fwrite( &ls.intens, sizeof( float ), 1, expStream );



	hotsize = 0.f;
	fallsize = 0.f;
	attenStart = 0.f;
	attenEnd = 0.f;

	if (ls.type != OMNI_LIGHT)
	{
		hotsize = ls.hotsize;
		fallsize = ls.fallsize;
	}
	if (ls.type != DIR_LIGHT && ls.useAtten)
	{
		attenStart = ls.attenStart;
		attenEnd = ls.attenEnd;
	}

	fwrite( &hotsize, sizeof( float ), 1, expStream );
	fwrite( &fallsize, sizeof( float ), 1, expStream );
	fwrite( &attenStart, sizeof( float ), 1, expStream );
	fwrite( &attenEnd, sizeof( float ), 1, expStream );
}


/*===========================================================================
 end */
