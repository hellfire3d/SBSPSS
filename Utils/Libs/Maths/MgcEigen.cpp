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
#include "mathtypes.h"
//#include "debug.h"
#include "math.h"
//#include "MgcRTLib.h"

//---------------------------------------------------------------------------
MgcEigen::MgcEigen (int iSize)
{
//    ASSERT( iSize >= 2 );
    m_iSize = iSize;

    m_aafMat = new real*[m_iSize];
    for (int i = 0; i < m_iSize; i++)
        m_aafMat[i] = new real[m_iSize];

    m_afDiag = new real[m_iSize];
    m_afSubd = new real[m_iSize];
}
//---------------------------------------------------------------------------
MgcEigen::~MgcEigen ()
{
    delete[] m_afSubd;
    delete[] m_afDiag;
    for (int i = 0; i < m_iSize; i++)
        delete[] m_aafMat[i];
    delete[] m_aafMat;
}
//---------------------------------------------------------------------------
void MgcEigen::Tridiagonal2 (real** m_aafMat, real* m_afDiag,
    real* m_afSubd)
{
    // matrix is already tridiagonal
    m_afDiag[0] = m_aafMat[0][0];
    m_afDiag[1] = m_aafMat[1][1];
    m_afSubd[0] = m_aafMat[0][1];
    m_afSubd[1] = 0.0;
    m_aafMat[0][0] = 1.0;
    m_aafMat[0][1] = 0.0;
    m_aafMat[1][0] = 0.0;
    m_aafMat[1][1] = 1.0;
}
//---------------------------------------------------------------------------
void MgcEigen::Tridiagonal3 (real** m_aafMat, real* m_afDiag,
    real* m_afSubd)
{
    real fM00 = m_aafMat[0][0];
    real fM01 = m_aafMat[0][1];
    real fM02 = m_aafMat[0][2];
    real fM11 = m_aafMat[1][1];
    real fM12 = m_aafMat[1][2];
    real fM22 = m_aafMat[2][2];

    m_afDiag[0] = fM00;
    m_afSubd[2] = 0.0;
    if ( fM02 != 0.0 )
    {
        real fLength = (real)sqrt(fM01*fM01+fM02*fM02);
        real fInvLength = 1.0f/fLength;
        fM01 *= fInvLength;
        fM02 *= fInvLength;
        real fQ = 2.0f*fM01*fM12+fM02*(fM22-fM11);
        m_afDiag[1] = fM11+fM02*fQ;
        m_afDiag[2] = fM22-fM02*fQ;
        m_afSubd[0] = fLength;
        m_afSubd[1] = fM12-fM01*fQ;
        m_aafMat[0][0] = 1.0; m_aafMat[0][1] = 0.0;  m_aafMat[0][2] = 0.0;
        m_aafMat[1][0] = 0.0; m_aafMat[1][1] = fM01; m_aafMat[1][2] = fM02;
        m_aafMat[2][0] = 0.0; m_aafMat[2][1] = fM02; m_aafMat[2][2] = -fM01;
    }
    else
    {
        m_afDiag[1] = fM11;
        m_afDiag[2] = fM22;
        m_afSubd[0] = fM01;
        m_afSubd[1] = fM12;
        m_aafMat[0][0] = 1.0; m_aafMat[0][1] = 0.0; m_aafMat[0][2] = 0.0;
        m_aafMat[1][0] = 0.0; m_aafMat[1][1] = 1.0; m_aafMat[1][2] = 0.0;
        m_aafMat[2][0] = 0.0; m_aafMat[2][1] = 0.0; m_aafMat[2][2] = 1.0;
    }
}
//---------------------------------------------------------------------------
void MgcEigen::Tridiagonal4 (real** m_aafMat, real* m_afDiag,
    real* m_afSubd)
{
    // save matrix M
    real fM00 = m_aafMat[0][0];
    real fM01 = m_aafMat[0][1];
    real fM02 = m_aafMat[0][2];
    real fM03 = m_aafMat[0][3];
    real fM11 = m_aafMat[1][1];
    real fM12 = m_aafMat[1][2];
    real fM13 = m_aafMat[1][3];
    real fM22 = m_aafMat[2][2];
    real fM23 = m_aafMat[2][3];
    real fM33 = m_aafMat[3][3];

    m_afDiag[0] = fM00;
    m_afSubd[3] = 0.0;

    m_aafMat[0][0] = 1.0;
    m_aafMat[0][1] = 0.0;
    m_aafMat[0][2] = 0.0;
    m_aafMat[0][3] = 0.0;
    m_aafMat[1][0] = 0.0;
    m_aafMat[2][0] = 0.0;
    m_aafMat[3][0] = 0.0;

    float fLength, fInvLength;

    if ( fM02 != 0.0 || fM03 != 0.0 )
    {
        real fQ11, fQ12, fQ13;
        real fQ21, fQ22, fQ23;
        real fQ31, fQ32, fQ33;

        // build column Q1
        fLength = (real)sqrt(fM01*fM01+fM02*fM02+fM03*fM03);
        fInvLength = 1.0f/fLength;
        fQ11 = fM01*fInvLength;
        fQ21 = fM02*fInvLength;
        fQ31 = fM03*fInvLength;

        m_afSubd[0] = fLength;

        // compute S*Q1
        real fV0 = fM11*fQ11+fM12*fQ21+fM13*fQ31;
        real fV1 = fM12*fQ11+fM22*fQ21+fM23*fQ31;
        real fV2 = fM13*fQ11+fM23*fQ21+fM33*fQ31;

        m_afDiag[1] = fQ11*fV0+fQ21*fV1+fQ31*fV2;

        // build column Q3 = Q1x(S*Q1)
        fQ13 = fQ21*fV2-fQ31*fV1;
        fQ23 = fQ31*fV0-fQ11*fV2;
        fQ33 = fQ11*fV1-fQ21*fV0;
        fLength = (real)sqrt(fQ13*fQ13+fQ23*fQ23+fQ33*fQ33);
        if ( fLength > 0.0 )
        {
            fInvLength = 1.0f/fLength;
            fQ13 *= fInvLength;
            fQ23 *= fInvLength;
            fQ33 *= fInvLength;

            // build column Q2 = Q3xQ1
            fQ12 = fQ23*fQ31-fQ33*fQ21; 
            fQ22 = fQ33*fQ11-fQ13*fQ31;
            fQ32 = fQ13*fQ21-fQ23*fQ11;

            fV0 = fQ12*fM11+fQ22*fM12+fQ32*fM13;
            fV1 = fQ12*fM12+fQ22*fM22+fQ32*fM23;
            fV2 = fQ12*fM13+fQ22*fM23+fQ32*fM33;
            m_afSubd[1] = fQ11*fV0+fQ21*fV1+fQ31*fV2;
            m_afDiag[2] = fQ12*fV0+fQ22*fV1+fQ32*fV2;
            m_afSubd[2] = fQ13*fV0+fQ23*fV1+fQ33*fV2;

            fV0 = fQ13*fM11+fQ23*fM12+fQ33*fM13;
            fV1 = fQ13*fM12+fQ23*fM22+fQ33*fM23;
            fV2 = fQ13*fM13+fQ23*fM23+fQ33*fM33;
            m_afDiag[3] = fQ13*fV0+fQ23*fV1+fQ33*fV2;
        }
        else
        {
            // S*Q1 parallel to Q1, choose any valid Q2 and Q3
            m_afSubd[1] = 0;

            fLength = fQ21*fQ21+fQ31*fQ31;
            if ( fLength > 0.0 )
            {
                fInvLength = 1.0f/fLength;
                real fTmp = fQ11-1.0f;
                fQ12 = -fQ21;
                fQ22 = 1.0f+fTmp*fQ21*fQ21*fInvLength;
                fQ32 = fTmp*fQ21*fQ31*fInvLength;

                fQ13 = -fQ31;
                fQ23 = fQ32;
                fQ33 = 1.0f+fTmp*fQ31*fQ31*fInvLength;

                fV0 = fQ12*fM11+fQ22*fM12+fQ32*fM13;
                fV1 = fQ12*fM12+fQ22*fM22+fQ32*fM23;
                fV2 = fQ12*fM13+fQ22*fM23+fQ32*fM33;
                m_afDiag[2] = fQ12*fV0+fQ22*fV1+fQ32*fV2;
                m_afSubd[2] = fQ13*fV0+fQ23*fV1+fQ33*fV2;

                fV0 = fQ13*fM11+fQ23*fM12+fQ33*fM13;
                fV1 = fQ13*fM12+fQ23*fM22+fQ33*fM23;
                fV2 = fQ13*fM13+fQ23*fM23+fQ33*fM33;
                m_afDiag[3] = fQ13*fV0+fQ23*fV1+fQ33*fV2;
            }
            else
            {
                // Q1 = (+-1,0,0)
                fQ12 = 0.0; fQ22 = 1.0; fQ32 = 0.0;
                fQ13 = 0.0; fQ23 = 0.0; fQ33 = 1.0;

                m_afDiag[2] = fM22;
                m_afDiag[3] = fM33;
                m_afSubd[2] = fM23;
            }
        }

        m_aafMat[1][1] = fQ11; m_aafMat[1][2] = fQ12; m_aafMat[1][3] = fQ13;
        m_aafMat[2][1] = fQ21; m_aafMat[2][2] = fQ22; m_aafMat[2][3] = fQ23;
        m_aafMat[3][1] = fQ31; m_aafMat[3][2] = fQ32; m_aafMat[3][3] = fQ33;
    }
    else
    {
        m_afDiag[1] = fM11;
        m_afSubd[0] = fM01;
        m_aafMat[1][1] = 1.0;
        m_aafMat[2][1] = 0.0;
        m_aafMat[3][1] = 0.0; 

        if ( fM13 != 0.0 )
        {
            fLength = (real)sqrt(fM12*fM12+fM13*fM13);
            fInvLength = 1.0f/fLength;
            fM12 *= fInvLength;
            fM13 *= fInvLength;
            real fQ = 2.0f*fM12*fM23+fM13*(fM33-fM22);

            m_afDiag[2] = fM22+fM13*fQ;
            m_afDiag[3] = fM33-fM13*fQ;
            m_afSubd[1] = fLength;
            m_afSubd[2] = fM23-fM12*fQ;
            m_aafMat[1][2] = 0.0;
            m_aafMat[1][3] = 0.0;
            m_aafMat[2][2] = fM12;
            m_aafMat[2][3] = fM13;
            m_aafMat[3][2] = fM13;
            m_aafMat[3][3] = -fM12;
        }
        else
        {
            m_afDiag[2] = fM22;
            m_afDiag[3] = fM33;
            m_afSubd[1] = fM12;
            m_afSubd[2] = fM23;
            m_aafMat[1][2] = 0.0;
            m_aafMat[1][3] = 0.0;
            m_aafMat[2][2] = 1.0;
            m_aafMat[2][3] = 0.0;
            m_aafMat[3][2] = 0.0;
            m_aafMat[3][3] = 1.0;
        }
    }
}
//---------------------------------------------------------------------------
void MgcEigen::TridiagonalN (int iSize, real** m_aafMat,
    real* m_afDiag, real* m_afSubd)
{
    int i0, i1, i2, i3;

    for (i0 = iSize-1, i3 = iSize-2; i0 >= 1; i0--, i3--)
    {
        real fH = 0.0, fScale = 0.0;

        if ( i3 > 0 )
        {
            for (i2 = 0; i2 <= i3; i2++)
                fScale += (real)fabs(m_aafMat[i0][i2]);
            if ( fScale == 0 )
            {
                m_afSubd[i0] = m_aafMat[i0][i3];
            }
            else
            {
                float fInvScale = 1.0f/fScale;
                for (i2 = 0; i2 <= i3; i2++)
                {
                    m_aafMat[i0][i2] *= fInvScale;
                    fH += m_aafMat[i0][i2]*m_aafMat[i0][i2];
                }
                real fF = m_aafMat[i0][i3];
                real fG = (real)sqrt(fH);
                if ( fF > 0.0 )
                    fG = -fG;
                m_afSubd[i0] = fScale*fG;
                fH -= fF*fG;
                m_aafMat[i0][i3] = fF-fG;
                fF = 0.0;
                float fInvH = 1.0f/fH;
                for (i1 = 0; i1 <= i3; i1++)
                {
                    m_aafMat[i1][i0] = m_aafMat[i0][i1]*fInvH;
                    fG = 0.0;
                    for (i2 = 0; i2 <= i1; i2++)
                        fG += m_aafMat[i1][i2]*m_aafMat[i0][i2];
                    for (i2 = i1+1; i2 <= i3; i2++)
                        fG += m_aafMat[i2][i1]*m_aafMat[i0][i2];
                    m_afSubd[i1] = fG*fInvH;
                    fF += m_afSubd[i1]*m_aafMat[i0][i1];
                }
                real fHalfFdivH = 0.5f*fF*fInvH;
                for (i1 = 0; i1 <= i3; i1++)
                {
                    fF = m_aafMat[i0][i1];
                    fG = m_afSubd[i1] - fHalfFdivH*fF;
                    m_afSubd[i1] = fG;
                    for (i2 = 0; i2 <= i1; i2++)
                    {
                        m_aafMat[i1][i2] -= fF*m_afSubd[i2] +
                            fG*m_aafMat[i0][i2];
                    }
                }
            }
        }
        else
        {
            m_afSubd[i0] = m_aafMat[i0][i3];
        }

        m_afDiag[i0] = fH;
    }

    m_afDiag[0] = m_afSubd[0] = 0;
    for (i0 = 0, i3 = -1; i0 <= iSize-1; i0++, i3++)
    {
        if ( m_afDiag[i0] )
        {
            for (i1 = 0; i1 <= i3; i1++)
            {
                real fSum = 0;
                for (i2 = 0; i2 <= i3; i2++)
                    fSum += m_aafMat[i0][i2]*m_aafMat[i2][i1];
                for (i2 = 0; i2 <= i3; i2++)
                    m_aafMat[i2][i1] -= fSum*m_aafMat[i2][i0];
            }
        }
        m_afDiag[i0] = m_aafMat[i0][i0];
        m_aafMat[i0][i0] = 1;
        for (i1 = 0; i1 <= i3; i1++)
            m_aafMat[i1][i0] = m_aafMat[i0][i1] = 0;
    }

    // re-ordering if MgcEigen::QLAlgorithm is used subsequently
    for (i0 = 1, i3 = 0; i0 < iSize; i0++, i3++)
        m_afSubd[i3] = m_afSubd[i0];
    m_afSubd[iSize-1] = 0;
}
//---------------------------------------------------------------------------
bool MgcEigen::QLAlgorithm (int iSize, real* m_afDiag, real* m_afSubd,
    real** m_aafMat)
{
    const int iMaxIter = 32;

    for (int i0 = 0; i0 < iSize; i0++)
    {
        int i1;
        for (i1 = 0; i1 < iMaxIter; i1++)
        {
            int i2;
            for (i2 = i0; i2 <= iSize-2; i2++)
            {
                real fTmp = (real) (fabs(m_afDiag[i2])+fabs(m_afDiag[i2+1]));
                if ( fabs(m_afSubd[i2]) + fTmp == fTmp )
                    break;
            }
            if ( i2 == i0 )
                break;

            real fG = (m_afDiag[i0+1]-m_afDiag[i0])/(2.0f*m_afSubd[i0]);
            real fR = (real)sqrt(fG*fG+1.0);
            if ( fG < 0.0 )
                fG = m_afDiag[i2]-m_afDiag[i0]+m_afSubd[i0]/(fG-fR);
            else
                fG = m_afDiag[i2]-m_afDiag[i0]+m_afSubd[i0]/(fG+fR);
            real fSin = 1.0, fCos = 1.0, fP = 0.0;
            for (int i3 = i2-1; i3 >= i0; i3--)
            {
                real fF = fSin*m_afSubd[i3];
                real fB = fCos*m_afSubd[i3];
                if ( fabs(fF) >= fabs(fG) )
                {
                    fCos = fG/fF;
                    fR = (real)sqrt(fCos*fCos+1.0);
                    m_afSubd[i3+1] = fF*fR;
                    fSin = 1.0f/fR;
                    fCos *= fSin;
                }
                else
                {
                    fSin = fF/fG;
                    fR = (real)sqrt(fSin*fSin+1.0);
                    m_afSubd[i3+1] = fG*fR;
                    fCos = 1.0f/fR;
                    fSin *= fCos;
                }
                fG = m_afDiag[i3+1]-fP;
                fR = (m_afDiag[i3]-fG)*fSin+2.0f*fB*fCos;
                fP = fSin*fR;
                m_afDiag[i3+1] = fG+fP;
                fG = fCos*fR-fB;

                for (int i4 = 0; i4 < iSize; i4++)
                {
                    fF = m_aafMat[i4][i3+1];
                    m_aafMat[i4][i3+1] = fSin*m_aafMat[i4][i3]+fCos*fF;
                    m_aafMat[i4][i3] = fCos*m_aafMat[i4][i3]-fSin*fF;
                }
            }
            m_afDiag[i0] -= fP;
            m_afSubd[i0] = fG;
            m_afSubd[i2] = 0.0;
        }
        if ( i1 == iMaxIter )
            return false;
    }

    return true;
}
//---------------------------------------------------------------------------
void MgcEigen::DecreasingSort (int iSize, real* afEigval,
    real** aafEigvec)
{
    // sort eigenvalues in decreasing order, e[0] >= ... >= e[iSize-1]
    for (int i0 = 0, i1; i0 <= iSize-2; i0++)
    {
        // locate maximum eigenvalue
        i1 = i0;
        real fMax = afEigval[i1];
        int i2;
        for (i2 = i0+1; i2 < iSize; i2++)
        {
            if ( afEigval[i2] > fMax )
            {
                i1 = i2;
                fMax = afEigval[i1];
            }
        }

        if ( i1 != i0 )
        {
            // swap eigenvalues
            afEigval[i1] = afEigval[i0];
            afEigval[i0] = fMax;

            // swap eigenvectors
            for (i2 = 0; i2 < iSize; i2++)
            {
                real fTmp = aafEigvec[i2][i0];
                aafEigvec[i2][i0] = aafEigvec[i2][i1];
                aafEigvec[i2][i1] = fTmp;
            }
        }
    }
}
//---------------------------------------------------------------------------
void MgcEigen::IncreasingSort (int iSize, real* afEigval,
    real** aafEigvec)
{
    // sort eigenvalues in increasing order, e[0] <= ... <= e[iSize-1]
    for (int i0 = 0, i1; i0 <= iSize-2; i0++)
    {
        // locate minimum eigenvalue
        i1 = i0;
        real fMin = afEigval[i1];
        int i2;
        for (i2 = i0+1; i2 < iSize; i2++)
        {
            if ( afEigval[i2] < fMin )
            {
                i1 = i2;
                fMin = afEigval[i1];
            }
        }

        if ( i1 != i0 )
        {
            // swap eigenvalues
            afEigval[i1] = afEigval[i0];
            afEigval[i0] = fMin;

            // swap eigenvectors
            for (i2 = 0; i2 < iSize; i2++)
            {
                real fTmp = aafEigvec[i2][i0];
                aafEigvec[i2][i0] = aafEigvec[i2][i1];
                aafEigvec[i2][i1] = fTmp;
            }
        }
    }
}
//---------------------------------------------------------------------------
void MgcEigen::SetMatrix (real** aafMat)
{
    for (int iRow = 0; iRow < m_iSize; iRow++)
    {
        for (int iCol = 0; iCol < m_iSize; iCol++)
            m_aafMat[iRow][iCol] = aafMat[iRow][iCol];
    }
}
//---------------------------------------------------------------------------
void MgcEigen::EigenStuff2 ()
{
    Tridiagonal2(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::EigenStuff3 ()
{
    Tridiagonal3(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::EigenStuff4 ()
{
    Tridiagonal4(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::EigenStuffN ()
{
    TridiagonalN(m_iSize,m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::EigenStuff ()
{
    switch ( m_iSize )
    {
        case 2:
            Tridiagonal2(m_aafMat,m_afDiag,m_afSubd);
            break;
        case 3:
            Tridiagonal3(m_aafMat,m_afDiag,m_afSubd);
            break;
        case 4:
            Tridiagonal4(m_aafMat,m_afDiag,m_afSubd);
            break;
        default:
            TridiagonalN(m_iSize,m_aafMat,m_afDiag,m_afSubd);
            break;
    }
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::DecrSortEigenStuff2 ()
{
    Tridiagonal2(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    DecreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::DecrSortEigenStuff3 ()
{
    Tridiagonal3(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    DecreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::DecrSortEigenStuff4 ()
{
    Tridiagonal4(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    DecreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::DecrSortEigenStuffN ()
{
    TridiagonalN(m_iSize,m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    DecreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::DecrSortEigenStuff ()
{
    switch ( m_iSize )
    {
        case 2:
            Tridiagonal2(m_aafMat,m_afDiag,m_afSubd);
            break;
        case 3:
            Tridiagonal3(m_aafMat,m_afDiag,m_afSubd);
            break;
        case 4:
            Tridiagonal4(m_aafMat,m_afDiag,m_afSubd);
            break;
        default:
            TridiagonalN(m_iSize,m_aafMat,m_afDiag,m_afSubd);
            break;
    }
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    DecreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::IncrSortEigenStuff2 ()
{
    Tridiagonal2(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    IncreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::IncrSortEigenStuff3 ()
{
    Tridiagonal3(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    IncreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::IncrSortEigenStuff4 ()
{
    Tridiagonal4(m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    IncreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::IncrSortEigenStuffN ()
{
    TridiagonalN(m_iSize,m_aafMat,m_afDiag,m_afSubd);
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    IncreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------
void MgcEigen::IncrSortEigenStuff ()
{
    switch ( m_iSize )
    {
        case 2:
            Tridiagonal2(m_aafMat,m_afDiag,m_afSubd);
            break;
        case 3:
            Tridiagonal3(m_aafMat,m_afDiag,m_afSubd);
            break;
        case 4:
            Tridiagonal4(m_aafMat,m_afDiag,m_afSubd);
            break;
        default:
            TridiagonalN(m_iSize,m_aafMat,m_afDiag,m_afSubd);
            break;
    }
    QLAlgorithm(m_iSize,m_afDiag,m_afSubd,m_aafMat);
    IncreasingSort(m_iSize,m_afDiag,m_aafMat);
}
//---------------------------------------------------------------------------

#ifdef EIGEN_TEST

int main ()
{
    MgcEigen kES(3);

    kES.Matrix(0,0) = 2.0;  kES.Matrix(0,1) = 1.0;  kES.Matrix(0,2) = 1.0;
    kES.Matrix(1,0) = 1.0;  kES.Matrix(1,1) = 2.0;  kES.Matrix(1,2) = 1.0;
    kES.Matrix(2,0) = 1.0;  kES.Matrix(2,1) = 1.0;  kES.Matrix(2,2) = 2.0;

    kES.IncrSortEigenStuff3();

    cout.setf(ios::fixed);

    cout << "eigenvalues = " << endl;
    int iRow;
    for (iRow = 0; iRow < 3; iRow++)
        cout << kES.GetEigenvalue(iRow) << ' ';
    cout << endl;

    cout << "eigenvectors = " << endl;
    for (iRow = 0; iRow < 3; iRow++)
    {
        for (int iCol = 0; iCol < 3; iCol++)
            cout << kES.GetEigenvector(iRow,iCol) << ' ';
        cout << endl;
    }

    // eigenvalues =
    //    1.000000 1.000000 4.000000
    // eigenvectors =
    //    0.411953  0.704955 0.577350
    //    0.404533 -0.709239 0.577350
    //   -0.816485  0.004284 0.577350

    return 0;
}

#endif
