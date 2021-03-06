#include "pch.h"
#include "math3d.h"


const Vector3 Vector3::ZERO(0, 0, 0);
const Vector3 Vector3::ONE(1, 1, 1);
const Vector3 Vector3::UP(0, 1, 0);
const Vector3 Vector3::DOWN(0, -1, 0);
const Vector3 Vector3::LEFT(-1, 0, 0);
const Vector3 Vector3::RIGHT(1, 0, 0);
const Vector3 Vector3::FORWARD(0, 0, 1);
const Vector3 Vector3::BACK(0, 0, -1);

const Matrix Matrix::IDENTITY(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

const AffineTransform AffineTransform::IDENTITY(Matrix::IDENTITY);


float Vec2Dot(const Vector2* a, const Vector2* b)
{
    return a->x * b->x + a->y * b->y;
}


float Vec3Dot(const Vector3* a, const Vector3* b)
{
	return a->x * b->x + a->y * b->y + a->z * b->z;
}


float Vec3Length(const Vector3* a)
{
	float s = Vec3Dot(a, a);
	s = sqrt(s);
	return s;
}


void Vec3Normalize(Vector3* dst, const Vector3* src)
{
	float s = Vec3Length(src);
	*dst = *src / s;
}


float Vec3Distance(const Vector3* a, const Vector3* b)
{
	Vector3 d = *b - *a;
	float s = Vec3Length(&d);
	return s;
}


float Vec3LengthSq(const Vector3* a)
{
	float s = Vec3Dot(a, a);
	return s;
}


float Vec3DistanceSq(const Vector3* a, const Vector3* b)
{
	Vector3 d = *b - *a;
	float s = Vec3LengthSq(&d);
	return s;
}


void Vec3Cross(Vector3* dst, const Vector3* a, const Vector3* b)
{
	*dst = Vector3(
		a->y * b->z - a->z * b->y,
		-a->x * b->z + a->z * b->x,
		a->x * b->y - a->y * b->x);
}


void MatrixIdentity(Matrix* dst)
{
    *dst = Matrix::IDENTITY;
}


void MatrixTranspose(Matrix* dst, const Matrix* src)
{
    Matrix tmp;
    for (int i = 0; i < 4; i++)
    {
        for (int j = i; j < 4; j++)
        {
            tmp[i][j] = (*src)[j][i];
        }
    }
    *dst = tmp;
}


void MatrixMultiply(Matrix* dst, const Matrix* a, const Matrix* b)
{
	float a00 = (*a)[0][0];
	float a01 = (*a)[0][1];
	float a02 = (*a)[0][2];
	float a03 = (*a)[0][3];

	float a10 = (*a)[1][0];
	float a11 = (*a)[1][1];
	float a12 = (*a)[1][2];
	float a13 = (*a)[1][3];

	float a20 = (*a)[2][0];
	float a21 = (*a)[2][1];
	float a22 = (*a)[2][2];
	float a23 = (*a)[2][3];

	float a30 = (*a)[3][0];
	float a31 = (*a)[3][1];
	float a32 = (*a)[3][2];
	float a33 = (*a)[3][3];

	float b00 = (*b)[0][0];
	float b01 = (*b)[0][1];
	float b02 = (*b)[0][2];
	float b03 = (*b)[0][3];

	float b10 = (*b)[1][0];
	float b11 = (*b)[1][1];
	float b12 = (*b)[1][2];
	float b13 = (*b)[1][3];

	float b20 = (*b)[2][0];
	float b21 = (*b)[2][1];
	float b22 = (*b)[2][2];
	float b23 = (*b)[2][3];

	float b30 = (*b)[3][0];
	float b31 = (*b)[3][1];
	float b32 = (*b)[3][2];
	float b33 = (*b)[3][3];

#if 0
	Matrix matrix;

	matrix[0][0] = a00*b00+a01*b10+a02*b20+a03*b30;
	matrix[0][1] = a00*b01+a01*b11+a02*b21+a03*b31;
	matrix[0][2] = a00*b02+a01*b12+a02*b22+a03*b32;
	matrix[0][3] = a00*b03+a01*b13+a02*b23+a03*b33;

	matrix[1][0] = a10*b00+a11*b10+a12*b20+a13*b30;
	matrix[1][1] = a10*b01+a11*b11+a12*b21+a13*b31;
	matrix[1][2] = a10*b02+a11*b12+a12*b22+a13*b32;
	matrix[1][3] = a10*b03+a11*b13+a12*b23+a13*b33;

	matrix[2][0] = a20*b00+a21*b10+a22*b20+a23*b30;
	matrix[2][1] = a20*b01+a21*b11+a22*b21+a23*b31;
	matrix[2][2] = a20*b02+a21*b12+a22*b22+a23*b32;
	matrix[2][3] = a20*b03+a21*b13+a22*b23+a23*b33;

	matrix[3][0] = a30*b00+a31*b10+a32*b20+a33*b30;
	matrix[3][1] = a30*b01+a31*b11+a32*b21+a33*b31;
	matrix[3][2] = a30*b02+a31*b12+a32*b22+a33*b32;
	matrix[3][3] = a30*b03+a31*b13+a32*b23+a33*b33;

	*dst = matrix;
#endif

#if 1
	* dst = Matrix(
		a00*b00 + a01*b10 + a02*b20 + a03*b30,
		a00*b01 + a01*b11 + a02*b21 + a03*b31,
		a00*b02 + a01*b12 + a02*b22 + a03*b32,
		a00*b03 + a01*b13 + a02*b23 + a03*b33,

		a10*b00 + a11*b10 + a12*b20 + a13*b30,
		a10*b01 + a11*b11 + a12*b21 + a13*b31,
		a10*b02 + a11*b12 + a12*b22 + a13*b32,
		a10*b03 + a11*b13 + a12*b23 + a13*b33,

		a20*b00 + a21*b10 + a22*b20 + a23*b30,
		a20*b01 + a21*b11 + a22*b21 + a23*b31,
		a20*b02 + a21*b12 + a22*b22 + a23*b32,
		a20*b03 + a21*b13 + a22*b23 + a23*b33,

		a30*b00 + a31*b10 + a32*b20 + a33*b30,
		a30*b01 + a31*b11 + a32*b21 + a33*b31,
		a30*b02 + a31*b12 + a32*b22 + a33*b32,
		a30*b03 + a31*b13 + a32*b23 + a33*b33);
#endif
}


bool MatrixInverse(Matrix* dst, const Matrix* matrix, float* pdet)
{
	float det;
	Matrix tmp;

	const float* m = reinterpret_cast<const float*>(matrix);
	float* inv = reinterpret_cast<float*>(&tmp);
	float* invOut = reinterpret_cast<float*>(dst);

	inv[0] = m[5]  * m[10] * m[15] - 
		m[5]  * m[11] * m[14] - 
		m[9]  * m[6]  * m[15] + 
		m[9]  * m[7]  * m[14] +
		m[13] * m[6]  * m[11] - 
		m[13] * m[7]  * m[10];

	inv[4] = -m[4]  * m[10] * m[15] + 
		m[4]  * m[11] * m[14] + 
		m[8]  * m[6]  * m[15] - 
		m[8]  * m[7]  * m[14] - 
		m[12] * m[6]  * m[11] + 
		m[12] * m[7]  * m[10];

	inv[8] = m[4]  * m[9] * m[15] - 
		m[4]  * m[11] * m[13] - 
		m[8]  * m[5] * m[15] + 
		m[8]  * m[7] * m[13] + 
		m[12] * m[5] * m[11] - 
		m[12] * m[7] * m[9];

	inv[12] = -m[4]  * m[9] * m[14] + 
		m[4]  * m[10] * m[13] +
		m[8]  * m[5] * m[14] - 
		m[8]  * m[6] * m[13] - 
		m[12] * m[5] * m[10] + 
		m[12] * m[6] * m[9];

	inv[1] = -m[1]  * m[10] * m[15] + 
		m[1]  * m[11] * m[14] + 
		m[9]  * m[2] * m[15] - 
		m[9]  * m[3] * m[14] - 
		m[13] * m[2] * m[11] + 
		m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] - 
		m[0]  * m[11] * m[14] - 
		m[8]  * m[2] * m[15] + 
		m[8]  * m[3] * m[14] + 
		m[12] * m[2] * m[11] - 
		m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] + 
		m[0]  * m[11] * m[13] + 
		m[8]  * m[1] * m[15] - 
		m[8]  * m[3] * m[13] - 
		m[12] * m[1] * m[11] + 
		m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] - 
		m[0]  * m[10] * m[13] - 
		m[8]  * m[1] * m[14] + 
		m[8]  * m[2] * m[13] + 
		m[12] * m[1] * m[10] - 
		m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] - 
		m[1]  * m[7] * m[14] - 
		m[5]  * m[2] * m[15] + 
		m[5]  * m[3] * m[14] + 
		m[13] * m[2] * m[7] - 
		m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] + 
		m[0]  * m[7] * m[14] + 
		m[4]  * m[2] * m[15] - 
		m[4]  * m[3] * m[14] - 
		m[12] * m[2] * m[7] + 
		m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] - 
		m[0]  * m[7] * m[13] - 
		m[4]  * m[1] * m[15] + 
		m[4]  * m[3] * m[13] + 
		m[12] * m[1] * m[7] - 
		m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] + 
		m[0]  * m[6] * m[13] + 
		m[4]  * m[1] * m[14] - 
		m[4]  * m[2] * m[13] - 
		m[12] * m[1] * m[6] + 
		m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] + 
		m[1] * m[7] * m[10] + 
		m[5] * m[2] * m[11] - 
		m[5] * m[3] * m[10] - 
		m[9] * m[2] * m[7] + 
		m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] - 
		m[0] * m[7] * m[10] - 
		m[4] * m[2] * m[11] + 
		m[4] * m[3] * m[10] + 
		m[8] * m[2] * m[7] - 
		m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] + 
		m[0] * m[7] * m[9] + 
		m[4] * m[1] * m[11] - 
		m[4] * m[3] * m[9] - 
		m[8] * m[1] * m[7] + 
		m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] - 
		m[0] * m[6] * m[9] - 
		m[4] * m[1] * m[10] + 
		m[4] * m[2] * m[9] + 
		m[8] * m[1] * m[6] - 
		m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (pdet)
		*pdet = det;

	if (fabs(det) < 0.0001f)
		return false;

	det = 1.0f / det;

	for (int i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}


void MatrixLookAt(Matrix* dst, const Vector3* eye, const Vector3* at, const Vector3* up)
{
	Vector3 z = (*at - *eye);
	Vec3Normalize(&z, &z);

	Vector3 x;
	Vec3Cross(&x, up, &z);
	Vec3Normalize(&x, &x);

	Vector3 y;
	Vec3Cross(&y, &z, &x);

	*dst = Matrix(
		x.x, y.x, z.x, 0.0f,
		x.y, y.y, z.y, 0.0f,
		x.z, y.z, z.z, 0.0f,
		-Vec3Dot(&x, eye), -Vec3Dot(&y, eye), -Vec3Dot(&z, eye), 1.0f);
}


void MatrixPerspective(Matrix* dst, float fov, float aspect, float n, float f)
{
	float y = 1.0f / tanf(ToRadians(fov) / 2.0f);
	float x = y / aspect;

	*dst = Matrix(
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, f / (f - n), 1,
		0, 0, -n*f / (f - n), 0);
}


void MatrixOrtho(Matrix* dst, float w, float h, float n, float f)
{
	*dst = Matrix(
		2/w,  0,    0,        0,
		0,    2/h,  0,        0,
		0,    0,    1/(f-n),  0,
		0,    0,    n/(n-f),  1);
}


void MatrixOrthoOffCenter(Matrix* dst, float l, float r, float b, float t, float n, float f)
{
	*dst = Matrix(
		2 / (r - l),        0,                  0,            0,
		0,                  2 / (t - b),        0,            0,
		0,                  0,                  1 / (f - n),  0,
		(l + r) / (l - r),  (t + b) / (b - t),  n / (n - f),  1);

}


void MatrixTransformCoord(Vector3* dst, const Vector3* v, const Matrix* m)
{
    Vector3 r;
    r.x = v->x * (*m)[0][0] + v->y * (*m)[1][0] + v->z * (*m)[2][0] + (*m)[3][0];
    r.y = v->x * (*m)[0][1] + v->y * (*m)[1][1] + v->z * (*m)[2][1] + (*m)[3][1];
    r.z = v->x * (*m)[0][2] + v->y * (*m)[1][2] + v->z * (*m)[2][2] + (*m)[3][2];
    *dst = r;
}


void MatrixTransformCoord(Vector3* dst, const Vector4* v, const Matrix* m)
{
    Vector3 r;
    r.x = v->x * (*m)[0][0] + v->y * (*m)[1][0] + v->z * (*m)[2][0] + v->w * (*m)[3][0];
    r.y = v->x * (*m)[0][1] + v->y * (*m)[1][1] + v->z * (*m)[2][1] + v->w * (*m)[3][1];
    r.z = v->x * (*m)[0][2] + v->y * (*m)[1][2] + v->z * (*m)[2][2] + v->w * (*m)[3][2];
    *dst = r;
}


void MatrixTransformNormal(Vector3* dst, const Vector3* v, const Matrix* m)
{
    Vector3 r;
    r.x = v->x * (*m)[0][0] + v->y * (*m)[1][0] + v->z * (*m)[2][0];
    r.y = v->x * (*m)[0][1] + v->y * (*m)[1][1] + v->z * (*m)[2][1];
    r.z = v->x * (*m)[0][2] + v->y * (*m)[1][2] + v->z * (*m)[2][2];
    *dst = r;
}


void MatrixRotationX(Matrix* m, float angle)
{
    float s = std::sin(angle);
    float c = std::cos(angle);
    *m = Matrix(
        1, 0, 0, 0,
        0, c,-s, 0,
        0, s, c, 0,
        0, 0, 0, 1);
}


void MatrixRotationY(Matrix* m, float angle)
{
    float s = std::sin(angle);
    float c = std::cos(angle);
    *m = Matrix(
        c, 0, s, 0,
        0, 1, 0, 0,
       -s, 0, c, 0,
        0, 0, 0, 1);
}


void MatrixRotationZ(Matrix* m, float angle)
{
    float s = std::sin(angle);
    float c = std::cos(angle);
    *m = Matrix(
        c,-s, 0, 0,
        s, c, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1);
}


SideType PointOnPlaneSide(const Vector3& point, const Plane& plane)
{
    auto d = PointToPlaneDistance(point, plane);
    if (d > MATH_SMALL_NUMBER)
        return SIDE_FRONT;
    if (d < -MATH_SMALL_NUMBER)
        return SIDE_BACK;
    return SIDE_CLIP;
}


float PointToPlaneDistance(const Vector3& point, const Plane& plane)
{
    return point.x * plane.x + point.y * plane.y + point.z * plane.z + plane.w;
}


Ray::Ray(const Vector3& origin, const Vector3& dir)
{
    this->origin = origin;
    this->direction = dir;
}


void AffineIdentity(AffineTransform* tm)
{
    tm->FromMatrix(Matrix::IDENTITY);
}


AffineTransform::AffineTransform(const Matrix& m)
{
    FromMatrix(m);
}


void AffineTransform::FromMatrix(const Matrix& m)
{
    translation = Vector3(m[3][0], m[3][1], m[3][2]);
    axis[0] = Vector3(m[0][0], m[0][1], m[0][2]);
    axis[1] = Vector3(m[1][0], m[1][1], m[1][2]);
    axis[2] = Vector3(m[2][0], m[2][1], m[2][2]);
    scale.x = Vec3Length(&axis[0]);
    scale.y = Vec3Length(&axis[0]);
    scale.z = Vec3Length(&axis[0]);
}


Matrix AffineTransform::ToMatrix() const
{
    Matrix m = Matrix::IDENTITY;
    m[0][0] = scale.x * axis[0].x;
    m[0][1] = scale.x * axis[0].y;
    m[0][2] = scale.x * axis[0].z;
    m[1][0] = scale.y * axis[1].x;
    m[1][1] = scale.y * axis[1].y;
    m[1][2] = scale.y * axis[1].z;
    m[2][0] = scale.z * axis[2].x;
    m[2][1] = scale.z * axis[2].y;
    m[2][2] = scale.z * axis[2].z;
    m[3][0] = translation.x;
    m[3][1] = translation.y;
    m[3][2] = translation.z;
    return m;
}


void AffineTransform::Rotate(const Matrix& m)
{
    MatrixTransformNormal(&axis[0], &axis[0], &m);
    MatrixTransformNormal(&axis[1], &axis[1], &m);
    MatrixTransformNormal(&axis[2], &axis[2], &m);
    // re-normalize axis
    Vec3Normalize(&axis[0], &axis[0]);
    Vec3Normalize(&axis[1], &axis[1]);
    Vec3Normalize(&axis[2], &axis[2]);
}