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

#ifndef MGCEIGEN_H
#define MGCEIGEN_H

#include "mathtypes.h"


class MgcEigen
{
public:
    MgcEigen (int iSize);
    ~MgcEigen ();

    // set the matrix for eigensolving
    real& Matrix (int iRow, int iCol);
    void SetMatrix (real** aafMat);

    // get the results of eigensolving (eigenvectors are columns of matrix)
    real GetEigenvalue (int i) const;
    real GetEigenvector (int iRow, int iCol) const;
    real* GetEigenvalue ();
    real** GetEigenvector ();

    // solve eigensystem
    void EigenStuff2 ();
    void EigenStuff3 ();
    void EigenStuff4 ();
    void EigenStuffN ();
    void EigenStuff  ();

    // solve eigensystem, use decreasing sort on eigenvalues
    void DecrSortEigenStuff2 ();
    void DecrSortEigenStuff3 ();
    void DecrSortEigenStuff4 ();
    void DecrSortEigenStuffN ();
    void DecrSortEigenStuff  ();

    // solve eigensystem, use increasing sort on eigenvalues
    void IncrSortEigenStuff2 ();
    void IncrSortEigenStuff3 ();
    void IncrSortEigenStuff4 ();
    void IncrSortEigenStuffN ();
    void IncrSortEigenStuff  ();

protected:
    int m_iSize;
    real** m_aafMat;
    real* m_afDiag;
    real* m_afSubd;

    // Householder reduction to tridiagonal form
    static void Tridiagonal2 (real** aafMat, real* afDiag,
        real* afSubd);
    static void Tridiagonal3 (real** aafMat, real* afDiag,
        real* afSubd);
    static void Tridiagonal4 (real** aafMat, real* afDiag,
        real* afSubd);
    static void TridiagonalN (int iSize, real** aafMat, real* afDiag,
        real* afSubd);

    // QL algorithm with implicit shifting, applies to tridiagonal matrices
    static bool QLAlgorithm (int iSize, real* afDiag, real* afSubd,
        real** aafMat);

    // sort eigenvalues from largest to smallest
    static void DecreasingSort (int iSize, real* afEigval,
        real** aafEigvec);

    // sort eigenvalues from smallest to largest
    static void IncreasingSort (int iSize, real* afEigval,
        real** aafEigvec);
};

//---------------------------------------------------------------------------
inline real& MgcEigen::Matrix (int iRow, int iCol)
{
    return m_aafMat[iRow][iCol];
}
//---------------------------------------------------------------------------
inline real MgcEigen::GetEigenvalue (int i) const
{
    return m_afDiag[i];
}
//---------------------------------------------------------------------------
inline real MgcEigen::GetEigenvector (int iRow, int iCol) const
{
    return m_aafMat[iRow][iCol];
}
//---------------------------------------------------------------------------
inline real* MgcEigen::GetEigenvalue ()
{
    return m_afDiag;
}
//---------------------------------------------------------------------------
inline real** MgcEigen::GetEigenvector ()
{
    return m_aafMat;
}
//---------------------------------------------------------------------------

#endif
