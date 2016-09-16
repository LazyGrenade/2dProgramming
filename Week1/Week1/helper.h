#pragma once

#include <vector>
#include <unordered_map>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

//keys
namespace KEYS
{
	enum { 
		NONE=0,
		MOUSE0=1<<1,	MOUSE1=1<<2,	UP=1<<3,	DOWN=1<<4, 
		LEFT=1<<5,		RIGHT=1<<6,		SPACE=1<<7, Z = 1 << 8, X = 1 << 9,
	};
	const int NUMKEYS = 9;
}

typedef std::vector<int> Keys;
typedef std::pair<Keys, std::string> MoveToAction;
typedef std::vector<MoveToAction> Moveset;

//pixel coordinates
struct Vector2
{
	Vector2(): r(x), th(y) {}
	Vector2(float xx, float yy) : x(xx), y(yy), r(x), th(y) { }
	Vector2(const Vector2& a) : x(a.x), y(a.y), r(x), th(y) { }
	float Magnitude();
	Vector2 Normalize();
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

inline float Vector2::Magnitude()
{
	float result = sqrt((this->x*this->x) + (this->y*this->y));
	return result;
}

inline Vector2 Vector2::Normalize()
{
	float mag = this->Magnitude();
	Vector2 norm = Vector2(this->x / mag, this->y / mag);
	return norm;
}

static const int FPS = 15;

template<class T>
static T lerp(const T& a, const T& b, float t)
{
	return a * (1 - t) + b * t;
}

template<class T>
static T slerp(const T& a, const T& b, float t, float m)
{
	
	return a * sin((1 - t)*m)/sin(m) + b * sin (t*m)/sin(m);
}
