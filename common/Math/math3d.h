#pragma once


#if PLATFORM_ANDROID
#define MATH_INLINE		inline
#endif


#if PLATFORM_PC
#define MATH_INLINE		__forceinline
#endif


#define MATH_MATRIX_MEMCPY	1


constexpr float MATH_PI = 3.141592654f;
constexpr float MATH_BIG_NUMBER = 10000.0f;
constexpr float MATH_SMALL_NUMBER = 0.0001f;


enum SideType
{
    SIDE_BACK = 0,
    SIDE_CLIP = 1,
    SIDE_FRONT = 2,
};


enum CullType
{
    CULL_OUT = 0,
    CULL_CLIP = 1,
    CULL_IN = 2,
};


MATH_INLINE float ToRadians(float degree)
{
	return MATH_PI * degree / 180.0f;
}


MATH_INLINE float ToDegree(float radian)
{
	return 180.0f * radian / MATH_PI;
}


template <typename _T>
MATH_INLINE const _T& Clamp(const _T& val, const _T& mins, const _T& maxs)
{
    if (val < mins) return mins;
    if (maxs < val) return maxs;
    return val;
}


class Vector2
{
public:
	float	x, y;

	Vector2();
	Vector2(float s);
	Vector2(const Vector2& v);
	Vector2(float x, float y);

	Vector2& operator = (const Vector2& v);

	bool operator == (const Vector2& v) const;
	bool operator != (const Vector2& v) const;

	const float& operator [] (int index) const;
	float& operator [] (int index);

	Vector2 operator + () const;
	Vector2 operator - () const;

	Vector2 operator + (float s) const;
	Vector2 operator - (float s) const;
	Vector2 operator * (float s) const;
	Vector2 operator / (float s) const;

	Vector2 operator + (const Vector2& v) const;
	Vector2 operator - (const Vector2& v) const;
	Vector2 operator * (const Vector2& v) const;
	Vector2 operator / (const Vector2& v) const;

	Vector2& operator += (float s);
	Vector2& operator -= (float s);
	Vector2& operator *= (float s);
	Vector2& operator /= (float s);

	Vector2& operator += (const Vector2& v);
	Vector2& operator -= (const Vector2& v);
	Vector2& operator *= (const Vector2& v);
	Vector2& operator /= (const Vector2& v);

    friend Vector2 operator * (float s, const Vector2& v);
};


float Vec2Length(const Vector2* a);
float Vec2LengthSq(const Vector2* a);
float Vec2Dot(const Vector2* a, const Vector2* b);
float Vec2Distance(const Vector2* a, const Vector2* b);
float Vec2DistanceSq(const Vector2* a, const Vector2* b);
void Vec2Normalize(Vector2* dst, const Vector2* src);


class Vector3
{
public:
    static const Vector3 ZERO;
    static const Vector3 ONE;
    static const Vector3 UP;
    static const Vector3 DOWN;
    static const Vector3 RIGHT;
    static const Vector3 LEFT;
    static const Vector3 FORWARD;
    static const Vector3 BACK;

	float	x, y, z;

	Vector3();
	Vector3(float s);
	Vector3(const Vector3& v);
	Vector3(float x, float y, float z);

	Vector3& operator = (const Vector3& v);

	bool operator == (const Vector3& v) const;
	bool operator != (const Vector3& v) const;

	const float& operator [] (int index) const;
	float& operator [] (int index);

	Vector3 operator + () const;
	Vector3 operator - () const;

	Vector3 operator + (float s) const;
	Vector3 operator - (float s) const;
	Vector3 operator * (float s) const;
	Vector3 operator / (float s) const;

	Vector3 operator + (const Vector3& v) const;
	Vector3 operator - (const Vector3& v) const;
	Vector3 operator * (const Vector3& v) const;
	Vector3 operator / (const Vector3& v) const;

	Vector3& operator += (float s);
	Vector3& operator -= (float s);
	Vector3& operator *= (float s);
	Vector3& operator /= (float s);

	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *= (const Vector3& v);
	Vector3& operator /= (const Vector3& v);

    friend Vector3 operator * (float s, const Vector3& v);
};


float Vec3Dot(const Vector3* a, const Vector3* b);
float Vec3Length(const Vector3* a);
void Vec3Normalize(Vector3* dst, const Vector3* src);
float Vec3Distance(const Vector3* a, const Vector3* b);
float Vec3LengthSq(const Vector3* a);
float Vec3DistanceSq(const Vector3* a, const Vector3* b);
void Vec3Cross(Vector3* dst, const Vector3* a, const Vector3* b);


class Vector4
{
public:
	float	x, y, z, w;

	Vector4();
	Vector4(float s);
	Vector4(const Vector4& v);
	Vector4(float x, float y, float z, float w);

	Vector4& operator = (const Vector4& v);

	bool operator == (const Vector4& v) const;
	bool operator != (const Vector4& v) const;

	const float& operator [] (int index) const;
	float& operator [] (int index);

	Vector4 operator + () const;
	Vector4 operator - () const;

	Vector4 operator + (float s) const;
	Vector4 operator - (float s) const;
	Vector4 operator * (float s) const;
	Vector4 operator / (float s) const;

	Vector4 operator + (const Vector4& v) const;
	Vector4 operator - (const Vector4& v) const;
	Vector4 operator * (const Vector4& v) const;
	Vector4 operator / (const Vector4& v) const;

	Vector4& operator += (float s);
	Vector4& operator -= (float s);
	Vector4& operator *= (float s);
	Vector4& operator /= (float s);

	Vector4& operator += (const Vector4& v);
	Vector4& operator -= (const Vector4& v);
	Vector4& operator *= (const Vector4& v);
	Vector4& operator /= (const Vector4& v);

    friend Vector4 operator * (float s, const Vector4& v);
};


class Matrix
{
public:
	Vector4	x, y, z, w;

    static const Matrix IDENTITY;

	Matrix();
	Matrix(const Matrix& m);
	Matrix(
		float a00, float a01, float a02, float a03,
		float a10, float a11, float a12, float a13,
		float a20, float a21, float a22, float a23,
		float a30, float a31, float a32, float a33);

	Matrix& operator = (const Matrix& m);

	bool operator == (const Matrix& m) const;
	bool operator != (const Matrix& m) const;

	const Vector4& operator [] (int index) const;
	Vector4& operator [] (int index);

	Matrix operator + (const Matrix& v) const;
	Matrix operator - (const Matrix& v) const;
	Matrix operator * (const Matrix& v) const;

	Matrix& operator += (const Matrix& v);
	Matrix& operator -= (const Matrix& v);
	Matrix& operator *= (const Matrix& v);
};


void MatrixIdentity(Matrix* dst);
void MatrixTranspose(Matrix* dst, const Matrix* src);
void MatrixMultiply(Matrix* dst, const Matrix* m1, const Matrix* m2);
bool MatrixInverse(Matrix* dst, const Matrix* m, float* det = nullptr);
void MatrixLookAt(Matrix* dst, const Vector3* eye, const Vector3* at, const Vector3* up);
void MatrixPerspective(Matrix* dst, float fov, float aspect, float znear, float zfar);
void MatrixOrtho(Matrix* dst, float w, float h, float znear, float zfar);
void MatrixOrthoOffCenter(Matrix* dst, float l, float r, float b, float t, float znear, float zfar);
void MatrixTransformCoord(Vector3* dst, const Vector3* v, const Matrix* m);
void MatrixTransformCoord(Vector3* dst, const Vector4* v, const Matrix* m);
void MatrixTransformNormal(Vector3* dst, const Vector3* v, const Matrix* m);


class Plane
{
public:
    float x, y, z, w;

    Plane() = default;
    Plane(const Plane& other) = default;
    Plane(float x, float y, float z, float w);

    Plane& operator = (const Plane& other) = default;

    bool operator == (const Plane& other) const;
    bool operator != (const Plane& other) const;
};


SideType PointOnPlaneSide(const Vector3& point, const Plane& plane);
float PointToPlaneDistance(const Vector3& point, const Plane& plane);


//-----------------------------------------------------------------------------
//
//	Vector2 implementation
//
//-----------------------------------------------------------------------------


MATH_INLINE Vector2::Vector2()
{
}

MATH_INLINE Vector2::Vector2(float s)
{
	x = y = s;
}

MATH_INLINE Vector2::Vector2(const Vector2& v)
{
	x = v.x;
	y = v.y;
}

MATH_INLINE Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

MATH_INLINE Vector2& Vector2::operator = (const Vector2& v)
{
	x = v.x;
	y = v.y;
	return (*this);
}

MATH_INLINE bool Vector2::operator == (const Vector2& v) const
{
	return x == v.x && y == v.y;
}

MATH_INLINE bool Vector2::operator != (const Vector2& v) const
{
	return x != v.x || y != v.y;
}

MATH_INLINE const float& Vector2::operator [] (int index) const
{
	return (&x)[index];
}

MATH_INLINE float& Vector2::operator [] (int index)
{
	return (&x)[index];
}

MATH_INLINE Vector2 Vector2::operator + () const
{
	return Vector2(x, y);
}

MATH_INLINE Vector2 Vector2::operator - () const
{
	return Vector2(-x, -y);
}

MATH_INLINE Vector2 Vector2::operator + (float s) const
{
	return Vector2(x + s, y + s);
}

MATH_INLINE Vector2 Vector2::operator - (float s) const
{
	return Vector2(x - s, y - s);
}

MATH_INLINE Vector2 Vector2::operator * (float s) const
{
	return Vector2(x * s, y * s);
}

MATH_INLINE Vector2 Vector2::operator / (float s) const
{
	return Vector2(x / s, y / s);
}

MATH_INLINE Vector2 Vector2::operator + (const Vector2& v) const
{
	return Vector2(x + v.x, y + v.y);
}

MATH_INLINE Vector2 Vector2::operator - (const Vector2& v) const
{
	return Vector2(x - v.x, y - v.y);
}

MATH_INLINE Vector2 Vector2::operator * (const Vector2& v) const
{
	return Vector2(x * v.x, y * v.y);
}

MATH_INLINE Vector2 Vector2::operator / (const Vector2& v) const
{
	return Vector2(x / v.x, y / v.y);
}

MATH_INLINE Vector2& Vector2::operator += (float s)
{
	x += s;
	y += s;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator -= (float s)
{
	x -= s;
	y -= s;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator *= (float s)
{
	x *= s;
	y *= s;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator /= (float s)
{
	x /= s;
	y /= s;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator += (const Vector2& v)
{
	x += v.x;
	y += v.y;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator -= (const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator *= (const Vector2& v)
{
	x *= v.x;
	y *= v.y;
	return (*this);
}

MATH_INLINE Vector2& Vector2::operator /= (const Vector2& v)
{
	x /= v.x;
	y /= v.y;
	return (*this);
}

MATH_INLINE Vector2 operator * (float s, const Vector2& v)
{
    return v * s;
}


//-----------------------------------------------------------------------------
//
//	Vector3 implementation
//
//-----------------------------------------------------------------------------


MATH_INLINE Vector3::Vector3()
{
}

MATH_INLINE Vector3::Vector3(float s)
{
	x = y = z = s;
}

MATH_INLINE Vector3::Vector3(const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
}

MATH_INLINE Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

MATH_INLINE Vector3& Vector3::operator = (const Vector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return (*this);
}

MATH_INLINE bool Vector3::operator == (const Vector3& v) const
{
	return x == v.x && y == v.y && z == v.z;
}

MATH_INLINE bool Vector3::operator != (const Vector3& v) const
{
	return x != v.x || y != v.y || z != v.z;
}

MATH_INLINE const float& Vector3::operator [] (int index) const
{
	return (&x)[index];
}

MATH_INLINE float& Vector3::operator [] (int index)
{
	return (&x)[index];
}

MATH_INLINE Vector3 Vector3::operator + () const
{
	return Vector3(x, y, z);
}

MATH_INLINE Vector3 Vector3::operator - () const
{
	return Vector3(-x, -y, -z);
}

MATH_INLINE Vector3 Vector3::operator + (float s) const
{
	return Vector3(x + s, y + s, z + s);
}

MATH_INLINE Vector3 Vector3::operator - (float s) const
{
	return Vector3(x - s, y - s, z - s);
}

MATH_INLINE Vector3 Vector3::operator * (float s) const
{
	return Vector3(x * s, y * s, z * s);
}

MATH_INLINE Vector3 Vector3::operator / (float s) const
{
	return Vector3(x / s, y / s, z / s);
}

MATH_INLINE Vector3 Vector3::operator + (const Vector3& v) const
{
	return Vector3(x + v.x, y + v.y, z + v.z);
}

MATH_INLINE Vector3 Vector3::operator - (const Vector3& v) const
{
	return Vector3(x - v.x, y - v.y, z - v.z);
}

MATH_INLINE Vector3 Vector3::operator * (const Vector3& v) const
{
	return Vector3(x * v.x, y * v.y, z * v.z);
}

MATH_INLINE Vector3 Vector3::operator / (const Vector3& v) const
{
	return Vector3(x / v.x, y / v.y, z / v.z);
}

MATH_INLINE Vector3& Vector3::operator += (float s)
{
	x += s;
	y += s;
	z += s;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator -= (float s)
{
	x -= s;
	y -= s;
	z -= s;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator *= (float s)
{
	x *= s;
	y *= s;
	z *= s;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator /= (float s)
{
	x /= s;
	y /= s;
	z /= s;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator += (const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator -= (const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator *= (const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return (*this);
}

MATH_INLINE Vector3& Vector3::operator /= (const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return (*this);
}

MATH_INLINE Vector3 operator * (float s, const Vector3& v)
{
    return v * s;
}


//-----------------------------------------------------------------------------
//
//	Vector4 implementation
//
//-----------------------------------------------------------------------------


MATH_INLINE Vector4::Vector4()
{
}

MATH_INLINE Vector4::Vector4(float s)
{
	x = y = z = w = s;
}

MATH_INLINE Vector4::Vector4(const Vector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
}

MATH_INLINE Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

MATH_INLINE Vector4& Vector4::operator = (const Vector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return (*this);
}

MATH_INLINE bool Vector4::operator == (const Vector4& v) const
{
	return x == v.x && y == v.y && z == v.z && w == v.w;
}

MATH_INLINE bool Vector4::operator != (const Vector4& v) const
{
	return x != v.x || y != v.y || z != v.z || w != v.w;
}

MATH_INLINE const float& Vector4::operator [] (int index) const
{
	return (&x)[index];
}

MATH_INLINE float& Vector4::operator [] (int index)
{
	return (&x)[index];
}

MATH_INLINE Vector4 Vector4::operator + () const
{
	return Vector4(x, y, z, w);
}

MATH_INLINE Vector4 Vector4::operator - () const
{
	return Vector4(-x, -y, -z, -w);
}

MATH_INLINE Vector4 Vector4::operator + (float s) const
{
	return Vector4(x + s, y + s, z + s, w + s);
}

MATH_INLINE Vector4 Vector4::operator - (float s) const
{
	return Vector4(x - s, y - s, z - s, w - s);
}

MATH_INLINE Vector4 Vector4::operator * (float s) const
{
	return Vector4(x * s, y * s, z * s, w * s);
}

MATH_INLINE Vector4 Vector4::operator / (float s) const
{
	return Vector4(x / s, y / s, z / s, w / s);
}

MATH_INLINE Vector4 Vector4::operator + (const Vector4& v) const
{
	return Vector4(x + v.x, y + v.y, z + v.z, w + v.w);
}

MATH_INLINE Vector4 Vector4::operator - (const Vector4& v) const
{
	return Vector4(x - v.x, y - v.y, z - v.z, w - v.w);
}

MATH_INLINE Vector4 Vector4::operator * (const Vector4& v) const
{
	return Vector4(x * v.x, y * v.y, z * v.z, w * v.w);
}

MATH_INLINE Vector4 Vector4::operator / (const Vector4& v) const
{
	return Vector4(x / v.x, y / v.y, z / v.z, w / v.w);
}

MATH_INLINE Vector4& Vector4::operator += (float s)
{
	x += s;
	y += s;
	z += s;
	w += s;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator -= (float s)
{
	x -= s;
	y -= s;
	z -= s;
	w -= s;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator *= (float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator /= (float s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator += (const Vector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator -= (const Vector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator *= (const Vector4& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return (*this);
}

MATH_INLINE Vector4& Vector4::operator /= (const Vector4& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return (*this);
}

MATH_INLINE Vector4 operator * (float s, const Vector4& v)
{
    return v * s;
}


//-----------------------------------------------------------------------------
//
//	Matrix implementation
//
//-----------------------------------------------------------------------------


MATH_INLINE Matrix::Matrix()
{
}

MATH_INLINE Matrix::Matrix(const Matrix& m)
{
#if MATH_MATRIX_MEMCPY
	memcpy(this, &m, sizeof(Matrix));
#else
	x = m.x;
	y = m.y;
	z = m.z;
	w = m.w;
#endif
}

MATH_INLINE Matrix::Matrix(
	float a00, float a01, float a02, float a03,
	float a10, float a11, float a12, float a13,
	float a20, float a21, float a22, float a23,
	float a30, float a31, float a32, float a33)
{
	x = Vector4(a00, a01, a02, a03);
	y = Vector4(a10, a11, a12, a13);
	z = Vector4(a20, a21, a22, a23);
	w = Vector4(a30, a31, a32, a33);
}

MATH_INLINE Matrix& Matrix::operator = (const Matrix& m)
{
	if (this != &m)
	{
#if MATH_MATRIX_MEMCPY
		memcpy(this, &m, sizeof(Matrix));
#else
		x = m.x;
		y = m.y;
		z = m.z;
		w = m.w;
#endif
	}
	return (*this);
}

MATH_INLINE Matrix Matrix::operator * (const Matrix& m) const
{
	Matrix res;
	MatrixMultiply(&res, this, &m);
	return res;
}

MATH_INLINE Matrix& Matrix::operator *= (const Matrix& m)
{
	Matrix tmp;
	MatrixMultiply(&tmp, this, &m);
	*this = tmp;
	return (*this);
}

MATH_INLINE const Vector4& Matrix::operator [] (int index) const
{
	return (&x)[index];
}

MATH_INLINE Vector4& Matrix::operator [] (int index)
{
	return (&x)[index];
}