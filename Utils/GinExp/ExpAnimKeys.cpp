//************************************************************************** 
//* Animout.cpp	- Ascii File Exporter
//* 
//* By Christer Janson
//* Kinetix Development
//*
//* January 20, 1997 CCJ Initial coding
//*
//* This module handles controller key output and controller sampling.
//*
//* Copyright (c) 1997, All Rights Reserved. 
//***************************************************************************

#include "asciiexp.h"

#define ALMOST_ZERO 1.0e-3f
BOOL EqualPoint3(Point3 p1, Point3 p2);

/****************************************************************************

  TM Animation output
  
****************************************************************************/

// Get hold of the transform controllers for the node... 
void AsciiExp::ExportAnimKeys( INode* node ) 
{
	BOOL bPosAnim;
	BOOL bRotAnim;
	BOOL bScaleAnim;
	BOOL bDoKeys = FALSE;

	// We can only export keys if all TM controllers are "known" to us.
	// The reason for that is that some controllers control more than what
	// they should. Consider a path position controller, if you turn on
	// follow and banking, this position controller will also control
	// rotation. If a node that had a path position controller also had a
	// TCB rotation controller, the TCB keys would not describe the whole
	// rotation of the node.
	// For that reason we will only export keys if all controllers
	// position, rotation and scale are linear, hybrid (bezier) or tcb.

/*	if (!GetAlwaysSample())
	{
		Control* pC = node->GetTMController()->GetPositionController();
		Control* rC = node->GetTMController()->GetRotationController();
		Control* sC = node->GetTMController()->GetScaleController();

		if (IsKnownController(pC) && IsKnownController(rC) && IsKnownController(sC))
		{
			bDoKeys = TRUE;
		}
	}
*/
	if (bDoKeys)
	{
		// Only dump the track header if any of the controllers have keys
		if (node->GetTMController()->GetPositionController()->NumKeys() ||
			node->GetTMController()->GetRotationController()->NumKeys() ||
			node->GetTMController()->GetScaleController()->NumKeys())
		{
//			fprintf(pStream,"%s\t%s {\n", indent.data(), ID_TM_ANIMATION); 
//			fprintf(pStream,"%s\t\t%s \"%s\"\n", indent.data(), ID_NODE_NAME, FixupName(node->GetName()));

			DumpPosKeys(node->GetTMController()->GetPositionController());
			DumpRotKeys(node->GetTMController()->GetRotationController());
			DumpScaleKeys(node->GetTMController()->GetScaleController());

//			fprintf(pStream,"%s\t}\n", indent.data());
		}
	}
	else if (CheckForAnimation(node, bPosAnim, bRotAnim, bScaleAnim))
	{
//		fprintf(pStream,"%s\t%s {\n", indent.data(), ID_TM_ANIMATION); 
//		fprintf(pStream,"%s\t\t%s \"%s\"\n", indent.data(), ID_NODE_NAME, FixupName(node->GetName()));

		if (bPosAnim)		DumpPosSample(node);
		if (bRotAnim)		DumpRotSample(node);
		if (bScaleAnim)		DumpScaleSample(node);

//		fprintf(pStream,"%s\t}\n", indent.data());
	}
}


// To really see if a node is animated we can step through the animation range
// and decompose the TM matrix for every frame and examine the components.
// This way we can identify position, rotation and scale animation separately.
// 
// Some controllers makes it problematic to examine the TMContollers instead of
// the actual TMMatrix. For example, a path controller is a position controller, 
// but if you turn on follow and banking, it will also affect the rotation component.
// If we want to, we can examine the position, rotation and scale controllers and
// if they all are Linear, Hybrid (bezier) or TCB, then we could export the actual keys.
// This is not at all difficult, but the importer has to know the exact interpolation
// algorithm in order to use it. The source code to the interpolation routines are available
// to ADN members.
// 
// For an example of how to export actual keys, look at DumpPoint3Keys() below.
// This method will check the actual controller to determine if the controller is known.
// If we know how to work this controller, its actual keys will be exported,
// otherwise the controller will be sampled using the user specified sampling frequency.

BOOL AsciiExp::CheckForAnimation(INode* node, BOOL& bPos, BOOL& bRot, BOOL& bScale)
{
	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Point3 firstPos;
	float rotAngle, firstRotAngle;
	Point3 rotAxis;
	Point3 firstScaleFactor;

	bPos = bRot = bScale = FALSE;

	for (t=start; t<=end; t+=delta)
	{
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		AngAxisFromQ(ap.q, &rotAngle, rotAxis);

		if (t != start)
		{
			// We examine the rotation angle to see if the rotation component
			// has changed.
			// Although not entierly true, it should work.
			// It is rare that the rotation axis is animated without
			// the rotation angle being somewhat affected.
			bPos = TRUE;
			bRot = TRUE;
			bScale = TRUE;
//			if (!EqualPoint3(ap.t, firstPos))					bPos = TRUE;
//			if (fabs(rotAngle - firstRotAngle) > ALMOST_ZERO)	bRot = TRUE;
//			if (!EqualPoint3(ap.k, firstScaleFactor))			bScale = TRUE;
		}
		else
		{
			firstPos = ap.t;
			firstRotAngle = rotAngle;
			firstScaleFactor = ap.k;
		}

		// No need to continue looping if all components are animated
		if (bPos && bRot && bScale)		break;
	}

	return bPos || bRot || bScale;
}


void AsciiExp::DumpPosQuatSample(INode* node)
{	
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_POS_TRACK);

	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	TimeValue	t;
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	Point3		prevPos;
	Quat		prevQ;
	Quat		q;
	char		name[256];


	sprintf( name, "%s", node->GetName() );
	fprintf( tempStream, "OUTPUTTING - %s\n", name );				// DEBUG FILE
//	fwrite( &name, sizeof(char), NAME_LENGTH, boneStream );					// WRITE BONE NAME

	prevQ.Identity();

	for (t=start; t<=end; t+=delta)
	{
		// TRANSLATION
		tm = node->GetNodeTM(t);// * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		fwrite( &ap.t.x, sizeof(float), 1, expStream );			// WRITE BONE X-POS
		fwrite( &ap.t.z, sizeof(float), 1, expStream );			// WRITE BONE Y-POS
		fwrite( &ap.t.y, sizeof(float), 1, expStream );			// WRITE BONE Z-POS

		fwrite( &ap.q.x, sizeof(float), 1, expStream );				// WRITE BONE ROT X-AXIS
		fwrite( &ap.q.z, sizeof(float), 1, expStream );				// WRITE BONE ROT Y-AXIS
		fwrite( &ap.q.y, sizeof(float), 1, expStream );				// WRITE BONE ROT Z-AXIS
		fwrite( &ap.q.w, sizeof(float), 1, expStream );				// WRITE BONE ROT W ANGLE
	}
}

void AsciiExp::DumpPosSample(INode* node)
{	
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_POS_TRACK);

	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	TimeValue	t;
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	Point3		prevPos;
	Quat		prevQ;
	Quat		q;
	char		name[256];


	sprintf( name, "%s", node->GetName() );
	fprintf( tempStream, "OUTPUTTING - %s\n", name );				// DEBUG FILE
//	fwrite( &name, sizeof(char), NAME_LENGTH, boneStream );					// WRITE BONE NAME

	prevQ.Identity();

	for (t=start; t<=end; t+=delta)
	{
		// TRANSLATION
		tm = node->GetNodeTM(t);// * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		Point3	fpos;
		Point3	pos = ap.t;

		prevPos = pos;

		fpos.x = pos.x;
		fpos.y = pos.z;
		fpos.z = -pos.y;

		fprintf( tempStream, "	POS = %f %f %f\n", pos.x, pos.z, -pos.y );
//		fwrite( &fpos.x, sizeof(float), 1, boneStream );			// WRITE BONE X-POS
//		fwrite( &fpos.y, sizeof(float), 1, boneStream );			// WRITE BONE Y-POS
//		fwrite( &fpos.z, sizeof(float), 1, boneStream );			// WRITE BONE Z-POS


		// ROTATION	
		tm = node->GetNodeTM(t);// * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		if (t == start)
		{
			q = ap.q;
			prevQ = ap.q;		// SAVE BASE ROTATION, THEN USE THIS AS ROTATION ORIGIN
		}
		else
		{
			q = ap.q / prevQ;
		}

		fprintf( tempStream, "	QUAT = %f %f %f %f\n", q.x, q.y, q.z, q.w );
//		fwrite( &q.x, sizeof(float), 1, boneStream );				// WRITE BONE ROT X-AXIS
//		fwrite( &q.y, sizeof(float), 1, boneStream );				// WRITE BONE ROT Y-AXIS
//		fwrite( &q.z, sizeof(float), 1, boneStream );				// WRITE BONE ROT Z-AXIS
//		fwrite( &q.w, sizeof(float), 1, boneStream );				// WRITE BONE ROT W ANGLE
	}
}


void AsciiExp::DumpRotSample(INode* node)
{
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_ROT_TRACK);

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Quat prevQ;

	prevQ.Identity();

	for (t=start; t<=end; t+=delta)
	{
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));

		decomp_affine(tm, &ap);

		// Rotation keys should be relative, so we need to convert these
		// absolute samples to relative values.

		Quat q = ap.q / prevQ;
		prevQ = ap.q;

		// No point in exporting null keys...
//		if (q.IsIdentity())		continue;

		// Output the sample
		fprintf( tempStream, "	QUAT = %f %f %f %f\n", q.x, q.y, q.z, q.w );
//		fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_ROT_SAMPLE, t, Format(q));
	}

//	fprintf(pStream,"%s\t\t}\n", indent.data());
}


void AsciiExp::DumpScaleSample(INode* node)
{	
//	fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_SCALE_TRACK);

	TimeValue start = ip->GetAnimRange().Start();
	TimeValue end = ip->GetAnimRange().End();
	TimeValue t;
	int delta = GetTicksPerFrame();
	Matrix3 tm;
	AffineParts ap;
	Point3	prevFac;

	for (t=start; t<=end; t+=delta)
	{
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

		// Skip identical keys 
		if (t!= start && EqualPoint3(ap.k, prevFac))	continue;

		prevFac = ap.k;

		// Output the sample
//		fprintf(pStream, "%s\t\t\t%s %d\t%s %s\n", indent.data(), ID_SCALE_SAMPLE, t, Format(ap.k), Format(ap.u));
	}

//	fprintf(pStream,"%s\t\t}\n", indent.data());
}


// Output point3 keys if this is a known point3 controller that
// supports key operations. Otherwise we will sample the controller 
// once for each frame to get the value.
// Point3 controllers can control, for example, color.
void AsciiExp::DumpPoint3Keys(Control* cont)
{
	// Bug out if no controller.
	if (!cont)	return;

	int i;
	IKeyControl *ikc = NULL;

	// If the user wants us to always sample, we will ignore the KeyControlInterface
//	if (!GetAlwaysSample())		ikc = GetKeyControlInterface(cont);

	// TCB point3
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_POINT3_CLASS_ID, 0))
	{
//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POINT3_TCB); 
		for (i=0; i<ikc->GetNumKeys(); i++)
		{
			ITCBPoint3Key key;
			ikc->GetKey(i, &key);
			fprintf( tempStream, "	KEYPOS = %f %f %f\n", key.val.x, key.val.y, key.val.z );
//			fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_TCB_POINT3_KEY, key.time, Format(key.val));
			// Add TCB specific data
//			fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	// Bezier point3
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_POINT3_CLASS_ID, 0))
	{
//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POINT3_BEZIER);
		for (i=0; i<ikc->GetNumKeys(); i++)
		{
			IBezPoint3Key key;
			ikc->GetKey(i, &key);
			fprintf( tempStream, "	KEYPOS = %f %f %f\n", key.val.x, key.val.y, key.val.z );
//			fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_BEZIER_POINT3_KEY, key.time, Format(key.val));
//			fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	// Bezier color
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_COLOR_CLASS_ID, 0))
	{
//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_COLOR_BEZIER); 
		for (i=0; i<ikc->GetNumKeys(); i++)
		{
			IBezPoint3Key key;
			ikc->GetKey(i, &key);
			fprintf( tempStream, "	KEYPOS = %f %f %f\n", key.val.x, key.val.y, key.val.z );
//			fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_BEZIER_POINT3_KEY, key.time, Format(key.val));
//			fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	else
	{
		// Unknown controller, no key interface or sample on demand -
		// This might be a procedural controller or something else we
		// don't know about. The last resort is to get the value from the 
		// controller at every n frames.

		TSTR name;
		cont->GetClassName(name);
//		fprintf(pStream,"%s\t\t%s \"%s\" {\n", indent.data(), ID_CONTROL_POINT3_SAMPLE, FixupName(name));

		// If it is animated at all...
		if (cont->IsAnimated())
		{
			// Get the range of the controller animation 
			Interval range; 
			// Get range of full animation
			Interval animRange = ip->GetAnimRange(); 
			TimeValue t = cont->GetTimeRange(TIMERANGE_ALL).Start();
			Point3 value;

			// While we are inside the animation... 
			while (animRange.InInterval(t))
			{
				// Sample the controller
				range = FOREVER;
				cont->GetValue(t, &value, range);

				// Set time to start of controller validity interval 
				t = range.Start();

				// Output the sample
				fprintf( tempStream, "	KEYPOS = %f %f %f\n", value.x, value.y, value.z );
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_POINT3_KEY, t, Format(value));

				// If the end of the controller validity is beyond the 
				// range of the animation
				if (range.End() > cont->GetTimeRange(TIMERANGE_ALL).End())
				{
					break;
				}
				else
				{
					t = (range.End()/GetTicksPerFrame()+1) * GetTicksPerFrame();
				}
			}
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
}


// Output float keys if this is a known float controller that
// supports key operations. Otherwise we will sample the controller 
// once for each frame to get the value.
void AsciiExp::DumpFloatKeys(Control* cont)
{
	if (!cont)	return;

	int i;
	IKeyControl *ikc = NULL;

	// If the user wants us to always sample, we will ignore the KeyControlInterface
//	if (!GetAlwaysSample())		ikc = GetKeyControlInterface(cont);
	
	// TCB float
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_FLOAT_CLASS_ID, 0))
	{
//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_FLOAT_TCB); 
		for (i=0; i<ikc->GetNumKeys(); i++)
		{
			ITCBFloatKey key;
			ikc->GetKey(i, &key);
//			fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_TCB_FLOAT_KEY, key.time, Format(key.val));
//			fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	// Bezier float
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID, 0))
	{
//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_FLOAT_BEZIER); 
		for (i=0; i<ikc->GetNumKeys(); i++)
		{
			IBezFloatKey key;
			ikc->GetKey(i, &key);
//			fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_BEZIER_FLOAT_KEY, key.time, Format(key.val));
//			fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_FLOAT_CLASS_ID, 0))
	{
//		fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_FLOAT_LINEAR); 
		for (i=0; i<ikc->GetNumKeys(); i++)
		{
			ILinFloatKey key;
			ikc->GetKey(i, &key);
//			fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_FLOAT_KEY, key.time, Format(key.val));
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
	else
	{
		// Unknown controller, no key interface or sample on demand -
		// This might be a procedural controller or something else we
		// don't know about. The last resort is to get the value from the 
		// controller at every n frames.
		
		TSTR name;
		cont->GetClassName(name);
//		fprintf(pStream,"%s\t\t%s \"%s\" {\n", indent.data(), ID_CONTROL_FLOAT_SAMPLE, FixupName(name));
		
		// If it is animated at all...
		if (cont->IsAnimated())
		{
			// Get the range of the controller animation 
			Interval range; 
			// Get range of full animation
			Interval animRange = ip->GetAnimRange(); 
			TimeValue t = cont->GetTimeRange(TIMERANGE_ALL).Start();
			float value;
			
			// While we are inside the animation... 
			while (animRange.InInterval(t))
			{
				// Sample the controller
				range = FOREVER;
				cont->GetValue(t, &value, range);
				
				// Set time to start of controller validity interval 
				t = range.Start();
				
				// Output the sample
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_FLOAT_KEY, t, Format(value));
				
				// If the end of the controller validity is beyond the 
				// range of the animation
				if (range.End() > cont->GetTimeRange(TIMERANGE_ALL).End())
				{
					break;
				}
				else
				{
					t = (range.End()/GetTicksPerFrame()+1) * GetTicksPerFrame();
				}
			}
		}
//		fprintf(pStream,"%s\t\t}\n", indent.data());
	}
}


void AsciiExp::DumpPosKeys(Control* cont) 
{
	if (!cont) return;
	
	int i;
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	// TCB position
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POS_TCB); 
			for (i=0; i<numKeys; i++)
			{
				ITCBPoint3Key key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_TCB_POS_KEY, key.time, Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	// Bezier position
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0))
	{
		int numKeys;
		if(numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POS_BEZIER); 
			for (i=0; i<numKeys; i++)
			{
				IBezPoint3Key key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_BEZIER_POS_KEY, key.time, Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	// Linear position
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0))
	{
		int numKeys;
		if(numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_POS_LINEAR); 
			for (i=0; i<numKeys; i++)
			{
				ILinPoint3Key key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_POS_KEY, key.time, Format(key.val));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
}


void AsciiExp::DumpRotKeys(Control* cont)
{
	if (!cont)	return;
	
	int i;
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_ROT_TCB); 
			for (i=0; i<numKeys; i++)
			{
				ITCBRotKey key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_TCB_ROT_KEY, key.time, Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_ROT_BEZIER); 
			for (i=0; i<numKeys; i++)
			{
				IBezQuatKey key;
				ikc->GetKey(i, &key);
				 // Quaternions are converted to AngAxis when written to file
				// There is no intan/outtan for Quat Rotations
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_ROT_KEY, key.time, Format(key.val));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_ROTATION_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_ROT_LINEAR); 
			for (i=0; i<numKeys; i++)
			{
				ILinRotKey key;
				ikc->GetKey(i, &key);
				 // Quaternions are converted to AngAxis when written to file
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_ROT_KEY, key.time, Format(key.val));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
}

void AsciiExp::DumpScaleKeys(Control* cont) 
{
	if (!cont)	return;
	
	int i;
	IKeyControl *ikc = GetKeyControlInterface(cont);
	
	if (ikc && cont->ClassID() == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_SCALE_TCB); 
			for (i=0; i<numKeys; i++)
			{
				ITCBScaleKey key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_TCB_SCALE_KEY, key.time, Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%s\t%s\t%s\n", Format(key.tens), Format(key.cont), Format(key.bias), Format(key.easeIn), Format(key.easeOut));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_SCALE_BEZIER); 
			for (i=0; i<numKeys; i++)
			{
				IBezScaleKey key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s", indent.data(), ID_BEZIER_SCALE_KEY, key.time, Format(key.val));
//				fprintf(pStream, "\t%s\t%s\t%d\n", Format(key.intan), Format(key.outtan), key.flags);
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
	else if (ikc && cont->ClassID() == Class_ID(LININTERP_SCALE_CLASS_ID, 0))
	{
		int numKeys;
		if (numKeys = ikc->GetNumKeys())
		{
//			fprintf(pStream,"%s\t\t%s {\n", indent.data(), ID_CONTROL_SCALE_LINEAR); 
			for (i=0; i<numKeys; i++)
			{
				ILinScaleKey key;
				ikc->GetKey(i, &key);
//				fprintf(pStream, "%s\t\t\t%s %d\t%s\n", indent.data(), ID_SCALE_KEY, key.time, Format(key.val));
			}
//			fprintf(pStream,"%s\t\t}\n", indent.data());
		}
	}
}


// Not truly the correct way to compare floats of arbitary magnitude...
BOOL EqualPoint3(Point3 p1, Point3 p2)
{
	if (fabs(p1.x - p2.x) > ALMOST_ZERO)	return FALSE;
	if (fabs(p1.y - p2.y) > ALMOST_ZERO)	return FALSE;
	if (fabs(p1.z - p2.z) > ALMOST_ZERO)	return FALSE;
	return TRUE;
}


// Determine if a TM controller is known by the system.
AsciiExp::IsKnownController(Control* cont)
{
	ulong partA, partB;

	if (!cont)	return FALSE;

	partA = cont->ClassID().PartA();
	partB = cont->ClassID().PartB();

	if (partB != 0x00)	return FALSE;

	switch (partA)
	{
		case TCBINTERP_POSITION_CLASS_ID:
		case TCBINTERP_ROTATION_CLASS_ID:
		case TCBINTERP_SCALE_CLASS_ID:
		case HYBRIDINTERP_POSITION_CLASS_ID:
		case HYBRIDINTERP_ROTATION_CLASS_ID:
		case HYBRIDINTERP_SCALE_CLASS_ID:
		case LININTERP_POSITION_CLASS_ID:
		case LININTERP_ROTATION_CLASS_ID:
		case LININTERP_SCALE_CLASS_ID:
			return TRUE;
	}

	return FALSE;
}
