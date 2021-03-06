#include "common.h"

void
CVector::Normalise(void)
{
	float sq = MagnitudeSqr();
	if (sq > 0.0f) {
		float invsqrt = RecipSqrt(sq);
		x *= invsqrt;
		y *= invsqrt;
		z *= invsqrt;
	} else
		x = 1.0f;
}

CVector
CrossProduct(const CVector &v1, const CVector &v2)
{
	return CVector(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

CVector
Multiply3x3(const CMatrix &mat, const CVector &vec)
{
	// TODO: VU0 code
	return CVector(mat.rx * vec.x + mat.fx * vec.y + mat.ux * vec.z,
	               mat.ry * vec.x + mat.fy * vec.y + mat.uy * vec.z,
	               mat.rz * vec.x + mat.fz * vec.y + mat.uz * vec.z);
}

CVector
Multiply3x3(const CVector &vec, const CMatrix &mat)
{
	return CVector(mat.rx * vec.x + mat.ry * vec.y + mat.rz * vec.z,
	               mat.fx * vec.x + mat.fy * vec.y + mat.fz * vec.z,
	               mat.ux * vec.x + mat.uy * vec.y + mat.uz * vec.z);
}

CVector
operator*(const CMatrix &mat, const CVector &vec)
{
	// TODO: VU0 code
	return CVector(mat.rx * vec.x + mat.fx * vec.y + mat.ux * vec.z + mat.px,
	               mat.ry * vec.x + mat.fy * vec.y + mat.uy * vec.z + mat.py,
	               mat.rz * vec.x + mat.fz * vec.y + mat.uz * vec.z + mat.pz);
}

void
RwV3dTransformPoints(CVector * pointsOut, const CVector * pointsIn, RwInt32 numPoints, const RwMatrix * matrix)
{
	while(numPoints--){
		float x = pointsIn->x*matrix->right.x + pointsIn->y*matrix->up.x + pointsIn->z*matrix->at.x + matrix->pos.x;
		float y = pointsIn->x*matrix->right.y + pointsIn->y*matrix->up.y + pointsIn->z*matrix->at.y + matrix->pos.y;
		float z = pointsIn->x*matrix->right.z + pointsIn->y*matrix->up.z + pointsIn->z*matrix->at.z + matrix->pos.z;
		pointsOut->x = x;
		pointsOut->y = y;
		pointsOut->z = z;
		pointsOut++;
		pointsIn++;
	}
}
