#include "stdafx.h"

#include "Tests.h"
#include "Math/Math.h"

#define T_ASSERT(condition, msg) assert(condition && msg)

namespace Tests {

	static void TestDot() {
		Vector3f v1(2, 7, 1);
		Vector3f v2(8, 2, 8);
		float expected = 38;

		T_ASSERT(Math::Dot(v1, v2) == expected, "Dot product test failed!");
	}

	static void TestNormalize() {
		Vector3f v1(2, 7, 1);
		Vector3f expected(2.0f/sqrt(54), 7.0f / sqrt(54), 1.0f / sqrt(54));

		T_ASSERT(Math::Normalize(v1) == expected, "Normalize test failed!");
	}

	static void TestMatrixMul() {
		Mat4x4 m1;
		Mat4x4 m2;
		Mat4x4 expected;

		m1(0, 0) = 1;
		m1(0, 1) = 2;
		m1(0, 2) = 3;
		m1(0, 3) = 4;
		m1(1, 0) = 5;
		m1(1, 1) = 6;
		m1(1, 2) = 7;
		m1(1, 3) = 8;
		m1(2, 0) = 9;
		m1(2, 1) = 10;
		m1(2, 2) = 11;
		m1(2, 3) = 12;
		m1(3, 0) = 13;
		m1(3, 1) = 14;
		m1(3, 2) = 15;
		m1(3, 3) = 16;

		m2(0, 0) = 16;
		m2(0, 1) = 15;
		m2(0, 2) = 14;
		m2(0, 3) = 13;
		m2(1, 0) = 12;
		m2(1, 1) = 11;
		m2(1, 2) = 10;
		m2(1, 3) = 9;
		m2(2, 0) = 8;
		m2(2, 1) = 7;
		m2(2, 2) = 6;
		m2(2, 3) = 5;
		m2(3, 0) = 4;
		m2(3, 1) = 3;
		m2(3, 2) = 2;
		m2(3, 3) = 1;

		expected(0, 0) = 80;
		expected(0, 1) = 70;
		expected(0, 2) = 60;
		expected(0, 3) = 50;
		expected(1, 0) = 240;
		expected(1, 1) = 214;
		expected(1, 2) = 188;
		expected(1, 3) = 162;
		expected(2, 0) = 400;
		expected(2, 1) = 358;
		expected(2, 2) = 316;
		expected(2, 3) = 274;
		expected(3, 0) = 560;
		expected(3, 1) = 502;
		expected(3, 2) = 444;
		expected(3, 3) = 386;

		T_ASSERT((m1 * m2) == expected, "Matrix multiplication test failed!");
	}

	void TestCross() {
		Vector3f v1(2, -3, 1);
		Vector3f v2(-2, 1, 1);
		Vector3f expected(-4, -4, -4);

		T_ASSERT(Math::Cross(v1, v2) == expected, "Cross product test failed!");
	}

	void RunAllTests() {
		TestDot();
		TestCross();
		TestNormalize();
		TestMatrixMul();
	}

}