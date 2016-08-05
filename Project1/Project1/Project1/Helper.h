#pragma once

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//keys
namespace KEYS
{
	enum {
		NONE = 0,
		MOUSE0 = 1 << 1, MOUSE1 = 1 << 2, UP = 1 << 3, DOWN = 1 << 4,
		LEFT = 1 << 5, RIGHT = 1 << 6, SPACE = 1 << 7
	};
	const int NUMKEYS = 7;
}

//pixel coordinates
struct Vector2
{
	Vector2() : r(x), th(y) {}
	Vector2(float xx, float yy) : x(xx), y(yy), r(x), th(x) { }
	Vector2(const Vector2& a) : x(a.x), y(a.y), r(y), th(y) { }
	Vector2 Normalize();
	float Magnitude();
	Vector2 operator=(const Vector2 &b)
	{
		x = b.x; y = b.y;
		return b;
	}
	float x;
	float y;

	float& r;
	float& th;
};


inline float Vector2::Magnitude()
{
	return sqrt((x * x) + (y * y));
}

inline Vector2 Vector2::Normalize()
{
	Vector2 result;
	result.x = x /= Magnitude();
	result.y = y /= Magnitude();
	return result;
}

static Vector2 operator+(const Vector2 &a, const Vector2 &b)
{
	Vector2 result;
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return result;
}

static Vector2 operator-(const Vector2 &a, const Vector2 &b)
{
	Vector2 result;
	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return result;
}

static Vector2 operator*(const Vector2 &a, float c)
{
	Vector2 result = a;
	result.x *= c;
	result.y *= c;
	return result;
}

static Vector2 operator*(float c, const Vector2 &a)
{
	Vector2 result = a;
	result.x *= c;
	result.y *= c;
	return result;
}

static Vector2 operator/(const Vector2 &a, float c)
{
	Vector2 result = a;
	result.x /= c;
	result.y /= c;
	return result;
}


static Vector2 operator+=(Vector2 &LHS, const Vector2 &RHS)
{
	LHS.x += RHS.x;
	LHS.y += RHS.y;
	return LHS;
}

static const int FPS = 15;
