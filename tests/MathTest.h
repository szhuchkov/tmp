#pragma once


#include "UnitTest.h"


class MathTest : public UnitTest
{
public:
	virtual const char* GetName() override
	{
		return "Math test";
	}

	virtual bool Run() override
	{
		bool res = true;

		res &= Vec2Test();
		res &= Vec3Test();
		res &= Vec4Test();
		res &= MatrixTest();
		res &= ComplexVec3Test();

		return res;
	}

	template <typename _T>
	bool IsEqualVectors(const _T& a, const _T& b, const float eps = 0.001f)
	{
		const int dim = sizeof(_T) / sizeof(float);
		for(int i = 0; i < dim; i++)
		{
			if (fabs(a[i] - b[i]) > eps)
				return false;
		}
		return true;
	}

	bool IsEqualMatrix(const Matrix* a, const Matrix* b, const float eps = 0.001f)
	{
		const float* p1 = reinterpret_cast<const float*>(a);
		const float* p2 = reinterpret_cast<const float*>(b);
		const int dim = sizeof(Matrix) / sizeof(float);
		for(int i = 0; i < dim; i++)
		{
			if (fabs(p1[i] - p2[i]) > eps)
				return false;
		}
		return true;
	}

	bool Vec2Test()
	{
		Vector2 a(1, 0);
		Vector2 b = Vector2(0, 1);
		Vector2 c = a + b;
		Vector2 d(-c);
		Vector2 e = Vector2(-2, -2) - d;
		Vector2 f = e;

		e = e + 2.0f;
		e = e - 2.0f;
		e = e * 2.0f;
		e = e / 2.0f;

		e += 2.0f;
		e -= 2.0f;
		e *= 2.0f;
		e /= 2.0f;

		e += Vector2(2.0f);
		e -= Vector2(2.0f);
		e *= Vector2(2.0f);
		e /= Vector2(2.0f);

		if (!IsEqualVectors(e, f))
			return false;
		
		return true;
	}

	bool Vec3Test()
	{
		Vector3 a(0, 1, 2);
		Vector3 b = Vector3(2, 1, 0);
		Vector3 c = a + b;
		Vector3 d(-c);
		Vector3 e = Vector3(-3, -3, -3) - d;
		Vector3 f = e;

		e = e + 2.0f;
		e = e - 2.0f;
		e = e * 2.0f;
		e = e / 2.0f;

		e += 2.0f;
		e -= 2.0f;
		e *= 2.0f;
		e /= 2.0f;

		e += Vector3(2.0f);
		e -= Vector3(2.0f);
		e *= Vector3(2.0f);
		e /= Vector3(2.0f);

		if (!IsEqualVectors(e, f))
			return false;

		return true;
	}

	bool Vec4Test()
	{
		Vector4 a(0, 1, 2, 3);
		Vector4 b = Vector4(3, 2, 1, 0);
		Vector4 c = a + b;
		Vector4 d(-c);
		Vector4 e = Vector4(-4, -4, -4, -4) - d;
		Vector4 f = e;

		e = e + 2.0f;
		e = e - 2.0f;
		e = e * 2.0f;
		e = e / 2.0f;

		e += 2.0f;
		e -= 2.0f;
		e *= 2.0f;
		e /= 2.0f;

		e += Vector4(2.0f);
		e -= Vector4(2.0f);
		e *= Vector4(2.0f);
		e /= Vector4(2.0f);

		if (!IsEqualVectors(e, f))
			return false;

		return true;
	}

	bool MatrixTest()
	{
		static const Matrix refView(
			-0.948683f, -0.169031f, -0.267261f, +0.000000f,
			+0.000000f, +0.845154f, -0.534522f, +0.000000f,
			+0.316228f, -0.507093f, -0.801784f, +0.000000f,
			-0.000000f, +0.000000f, +3.741657f, +1.000000f);
		static const Matrix refProj(
			+0.794502f, +0.000000f, +0.000000f, +0.000000f,
			+0.000000f, +1.191754f, +0.000000f, +0.000000f,
			+0.000000f, +0.000000f, +1.010101f, +1.000000f,
			+0.000000f, +0.000000f, -1.010101f, +0.000000f);
		static const Matrix refViewProj(
			-0.753731f, -0.201443f, -0.269961f, -0.267261f,
			+0.000000f, +1.007215f, -0.539922f, -0.534522f,
			+0.251244f, -0.604329f, -0.809882f, -0.801784f,
			-0.000000f, +0.000000f, +2.769351f, +3.741657f);
		static const Matrix refViewInv(
			-0.948683f, +0.000000f, +0.316228f, +0.000000f,
			-0.169031f, +0.845154f, -0.507093f, +0.000000f,
			-0.267261f, -0.534523f, -0.801784f, +0.000000f,
			+1.000000f, +2.000000f, +3.000000f, +1.000000f);
		static const Matrix refOrtho(
			+0.666667f, +0.000000f, +0.000000f, +0.000000f,
			+0.000000f, +0.666667f, +0.000000f, +0.000000f,
			+0.000000f, +0.000000f, +0.001000f, +0.000000f,
			+0.333333f, -0.333333f, -0.000000f, +1.000000f);
		
		// construct look at matrix
		Matrix view;
		MatrixLookAt(&view, &Vector3(1, 2, 3), &Vector3(0, 0, 0), &Vector3(0, 1, 0));
		if (!IsEqualMatrix(&view, &refView))
			return false;

		// construct perspectve matrix
		Matrix proj;
		MatrixPerspective(&proj, 80.0f, 1.5f, 1.0f, 100.0f);
		if (!IsEqualMatrix(&proj, &refProj))
			return false;

		// matrix multiplication
		Matrix viewProj;
		MatrixMultiply(&viewProj, &view, &proj);
		if (!IsEqualMatrix(&viewProj, &refViewProj))
			return false;

		// matrix inverse test
		Matrix viewInv;
		MatrixInverse(&viewInv, &view);
		if (!IsEqualMatrix(&viewInv, &refViewInv))
			return false;

		// construct ortho matrix
		Matrix ortho;
		MatrixOrthoOffCenter(&ortho, -2.0f, 1.0f, -1.0f, 2.0f, 0.0f, 1000.0f);
		if (!IsEqualMatrix(&ortho, &refOrtho))
			return false;

		// matrix ops
		Matrix m1 = view;
		Matrix m2(proj);
		Matrix m3 = view * proj;
		m1 *= m2;
		if (!IsEqualMatrix(&m3, &viewProj))
			return false;
		if (!IsEqualMatrix(&m1, &viewProj))
			return false;

		return true;
	}

	void RandomVector3(Vector3* v)
	{
		v->x = (float)rand() / RAND_MAX;
		v->y = (float)rand() / RAND_MAX;
		v->z = (float)rand() / RAND_MAX;
	}

	// complex vector3 math test -- compute normals of a disk
	bool ComplexVec3Test()
	{
		const int SEGMENTS = 360;
		static Vector3 verts[SEGMENTS + 1];
		Vector3 ref(0, 1, 0);

		verts[0] = Vector3(0, 0, 0);
		for(int i = 0; i < SEGMENTS; i++)
		{
			float a = 2.0f * MATH_PI * i / SEGMENTS;
			verts[1 + i] = Vector3(sinf(a), 0, cosf(a));
		}

		for(int i = 0; i < SEGMENTS; i++)
		{
			int i1 = 0;
			int i2 = i + 1;
			int i3 = (i + 1) % SEGMENTS + 1;

			const Vector3& v1 = verts[i1];
			const Vector3& v2 = verts[i2];
			const Vector3& v3 = verts[i3];

			Vector3 e1 = v2 - v1;
			Vector3 e2 = v3 - v1;

			Vector3 n;
			Vec3Cross(&n, &e1, &e2);
			Vec3Normalize(&n, &n);

			if (!IsEqualVectors(n, ref))
				return false;
		}

		return true;
	}
};
