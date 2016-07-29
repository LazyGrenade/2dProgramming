#include <SDL.h>
#include <string>
#include <unordered_map>

#include "helper.h"

#pragma once

using namespace std;

class Manager;
class PhysicsObject;
class PhysicsManager;

//GameObject class
class GameObject
{
	friend Manager;
	friend PhysicsObject;
	friend PhysicsManager;
public:
	GameObject(void);
	~GameObject(void);

	virtual void Init(std::string txtfile, SDL_Rect* dest = nullptr);
	virtual bool Update();

	virtual void Render();
	virtual void Move(int action);

	//virtual bool Collide(GameObject* other);
	//virtual void HandleCollision(GameObject* other);

	bool active;

protected:
	virtual void UpdateAnimation();
	virtual void FollowTarget();


	PhysicsObject* phys_obj;

	string name;

	GameObject* target;
	Vector2 offset;

	SDL_Point* targetPoint;

	SDL_Surface* surf;
	SDL_Texture* img;
	SDL_Rect	 pos;

	float rotationSpeed;
	float angle;
	float mass;
	Vector2 position;
	Vector2 velocity;
	Vector2 accel;

	//animation stuff
	SDL_Rect	 clip;
	int			 frame;

	string state;

	//timer
	Uint32		prev;
	double		dt;
	double		timer;
};
