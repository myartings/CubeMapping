#pragma once

typedef float M3DMatrix44f[16];		// A 4 X 4 matrix, column major (floats) - OpenGL style
typedef double M3DMatrix44d[16];	// A 4 x 4 matrix, column major (doubles) - OpenGL style

typedef float	M3DVector3f[3];		// Vector of three floats (x, y, z)
typedef double	M3DVector3d[3];		// Vector of three doubles (x, y, z)

class math3d
{
public:
	math3d(void);
	~math3d(void);

public:
	static void m3dInvertMatrix44(float mInverse[16], const float m[16]);
	static float DetIJ(const float m[16], const int i, const int j);
	static void m3dMatrixMultiply44(float product[16], const float a[16], const float b[16] );

	static void m3dExtractRotationMatrix33(float dst[9], const float src[16]);
};

inline void math3d::m3dExtractRotationMatrix33(float dst[9], const float src[16])
{	
	memcpy(dst, src, sizeof(float) * 3); // X column
	memcpy(dst + 3, src + 4, sizeof(float) * 3); // Y column
	memcpy(dst + 6, src + 8, sizeof(float) * 3); // Z column
}