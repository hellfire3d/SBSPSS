// Magic Software, Inc.
// http://www.magic-software.com
// Copyright (c) 2000, All Rights Reserved
//
// Source code from Magic Software is supplied under the terms of a license
// agreement and may not be copied or disclosed except in accordance with the
// terms of that agreement.  The various license agreements may be found at
// the Magic Software web site.  This file is subject to the license
//
// FREE SOURCE CODE
// http://www.magic-software.com/License/free.pdf

#include "MgcEigen.h"
#include "vector3.h"
//#include "MgcLinearSystem.h"
#include "MgcAppr3DLineFit.h"

//----------------------------------------------------------------------------
void MgcOrthogonalLineFit (int iQuantity, const Vector3* akPoint,
    Vector3& rkOffset, Vector3& rkDirection)
{
    // compute average of points
    rkOffset = akPoint[0];
    int i;
    for (i = 1; i < iQuantity; i++)
        rkOffset += akPoint[i];
    real fInvQuantity = 1.0f/iQuantity;
    rkOffset *= fInvQuantity;

    // compute sums of products
    real fSumXX = 0.0, fSumXY = 0.0, fSumXZ = 0.0;
    real fSumYY = 0.0, fSumYZ = 0.0, fSumZZ = 0.0;
    for (i = 0; i < iQuantity; i++) 
    {
        Vector3 kDiff = akPoint[i] - rkOffset;
        fSumXX += kDiff.x*kDiff.x;
        fSumXY += kDiff.x*kDiff.y;
        fSumXZ += kDiff.x*kDiff.z;
        fSumYY += kDiff.y*kDiff.y;
        fSumYZ += kDiff.y*kDiff.z;
        fSumZZ += kDiff.z*kDiff.z;
    }

    // setup the eigensolver
    MgcEigen kES(3);
    kES.Matrix(0,0) = fSumYY+fSumZZ;
    kES.Matrix(0,1) = -fSumXY;
    kES.Matrix(0,2) = -fSumXZ;
    kES.Matrix(1,0) = kES.Matrix(0,1);
    kES.Matrix(1,1) = fSumXX+fSumZZ;
    kES.Matrix(1,2) = -fSumYZ;
    kES.Matrix(2,0) = kES.Matrix(0,2);
    kES.Matrix(2,1) = kES.Matrix(1,2);
    kES.Matrix(2,2) = fSumXX+fSumYY;

    // compute eigenstuff, smallest eigenvalue is in last position
    kES.DecrSortEigenStuff3();

    // unit-length direction for best-fit line
    rkDirection.x = kES.GetEigenvector(0,2);
    rkDirection.y = kES.GetEigenvector(1,2);
    rkDirection.z = kES.GetEigenvector(2,2);
}
//----------------------------------------------------------------------------
bool MgcOrthogonalLineFit (int iQuantity, const Vector3* akPoint,
    const bool* abValid, Vector3& rkOffset, Vector3& rkDirection)
{
    // compute average of points
	rkOffset.Zero();
    int i, iValidQuantity = 0;
    for (i = 0; i < iQuantity; i++)
    {
        if ( abValid[i] )
        {
            rkOffset += akPoint[i];
            iValidQuantity++;
        }
    }
    if ( iValidQuantity == 0 )
        return false;

    real fInvQuantity = 1.0f/iQuantity;
    rkOffset *= fInvQuantity;

    // compute sums of products
    real fSumXX = 0.0, fSumXY = 0.0, fSumXZ = 0.0;
    real fSumYY = 0.0, fSumYZ = 0.0, fSumZZ = 0.0;
    for (i = 0; i < iQuantity; i++) 
    {
        if ( abValid[i] )
        {
            Vector3 kDiff = akPoint[i] - rkOffset;
            fSumXX += kDiff.x*kDiff.x;
            fSumXY += kDiff.x*kDiff.y;
            fSumXZ += kDiff.x*kDiff.z;
            fSumYY += kDiff.y*kDiff.y;
            fSumYZ += kDiff.y*kDiff.z;
            fSumZZ += kDiff.z*kDiff.z;
        }
    }

    // setup the eigensolver
    MgcEigen kES(3);
    kES.Matrix(0,0) = fSumYY+fSumZZ;
    kES.Matrix(0,1) = -fSumXY;
    kES.Matrix(0,2) = -fSumXZ;
    kES.Matrix(1,0) = kES.Matrix(0,1);
    kES.Matrix(1,1) = fSumXX+fSumZZ;
    kES.Matrix(1,2) = -fSumYZ;
    kES.Matrix(2,0) = kES.Matrix(0,2);
    kES.Matrix(2,1) = kES.Matrix(1,2);
    kES.Matrix(2,2) = fSumXX+fSumYY;

    // compute eigenstuff, smallest eigenvalue is in last position
    kES.DecrSortEigenStuff3();

    // unit-length direction for best-fit line
    rkDirection.x = kES.GetEigenvector(0,2);
    rkDirection.y = kES.GetEigenvector(1,2);
    rkDirection.z = kES.GetEigenvector(2,2);

    return true;
}
//----------------------------------------------------------------------------
