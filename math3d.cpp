#include "StdAfx.h"
#include "math3d.h"

math3d::math3d(void)
{
}

math3d::~math3d(void)
{
}

void math3d::m3dInvertMatrix44(float mInverse[16], const float m[16])
{
	int i, j;
	float det, detij;

	// calculate 4x4 determinant
	det = 0.0f;
	for (i = 0; i < 4; i++)
	{
		det += (i & 0x1) ? (-m[i] * DetIJ(m, 0, i)) : (m[i] * DetIJ(m, 0,i));
	}
	det = 1.0f / det;

	// calculate inverse
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			detij = DetIJ(m, j, i);
			mInverse[(i*4)+j] = ((i+j) & 0x1) ? (-detij * det) : (detij *det); 
		}
	}
}


float math3d::DetIJ(const float m[16], const int i, const int j)
{
	int x, y, ii, jj;
	float ret, mat[3][3];

	x = 0;
	for (ii = 0; ii < 4; ii++)
	{
		if (ii == i) continue;
		y = 0;
		for (jj = 0; jj < 4; jj++)
		{
			if (jj == j) continue;
			mat[x][y] = m[(ii*4)+jj];
			y++;
		}
		x++;
	}

	ret =  mat[0][0]*(mat[1][1]*mat[2][2]-mat[2][1]*mat[1][2]);
	ret -= mat[0][1]*(mat[1][0]*mat[2][2]-mat[2][0]*mat[1][2]);
	ret += mat[0][2]*(mat[1][0]*mat[2][1]-mat[2][0]*mat[1][1]);

	return ret;
}

#define A(row,col)  a[(col<<2)+row]
#define B(row,col)  b[(col<<2)+row]
#define P(row,col)  product[(col<<2)+row]

///////////////////////////////////////////////////////////////////////////////
// Multiply two 4x4 matricies
void math3d::m3dMatrixMultiply44(float product[16], const float a[16], const float b[16] )
{
	for (int i = 0; i < 4; i++) {
		float ai0=A(i,0),  ai1=A(i,1),  ai2=A(i,2),  ai3=A(i,3);
		P(i,0) = ai0 * B(0,0) + ai1 * B(1,0) + ai2 * B(2,0) + ai3 * B(3,0);
		P(i,1) = ai0 * B(0,1) + ai1 * B(1,1) + ai2 * B(2,1) + ai3 * B(3,1);
		P(i,2) = ai0 * B(0,2) + ai1 * B(1,2) + ai2 * B(2,2) + ai3 * B(3,2);
		P(i,3) = ai0 * B(0,3) + ai1 * B(1,3) + ai2 * B(2,3) + ai3 * B(3,3);
	}
}


////////////////////////////////////////////////////////////////////////
// Assemble the camera matrix
void GetCameraMatrix(M3DMatrix44f m, bool bRotationOnly = false)
{
//	M3DVector3f x, z;
//
//	// Make rotation matrix
//	// Z vector is reversed
//	z[0] = -vForward[0];
//	z[1] = -vForward[1];
//	z[2] = -vForward[2];
//
//	// X vector = Y cross Z 
//	m3dCrossProduct3(x, vUp, z);
//
//	// Matrix has no translation information and is
//	// transposed.... (rows instead of columns)
//#define M(row,col)  m[col*4+row]
//	M(0, 0) = x[0];
//	M(0, 1) = x[1];
//	M(0, 2) = x[2];
//	M(0, 3) = 0.0;
//	M(1, 0) = vUp[0];
//	M(1, 1) = vUp[1];
//	M(1, 2) = vUp[2];
//	M(1, 3) = 0.0;
//	M(2, 0) = z[0];
//	M(2, 1) = z[1];
//	M(2, 2) = z[2];
//	M(2, 3) = 0.0;
//	M(3, 0) = 0.0;
//	M(3, 1) = 0.0;
//	M(3, 2) = 0.0;
//	M(3, 3) = 1.0;
//#undef M
//
//
//	if(bRotationOnly)
//		return;
//
//	// Apply translation too
//	M3DMatrix44f trans, M;
//	m3dTranslationMatrix44(trans, -vOrigin[0], -vOrigin[1], -vOrigin[2]);  
//
//	m3dMatrixMultiply44(M, m, trans);
//
//	// Copy result back into m
//	memcpy(m, M, sizeof(float)*16);
}
