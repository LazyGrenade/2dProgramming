#include <fstream>
#include <iostream>
#include <SDL_image.h>

#include "GameObject.h"
#include "Manager.h"

GameObject::GameObject(void)
	: active(true)
	, target(nullptr)
	, surf(nullptr)
	, img(nullptr)
	, angle(0)
	, rotation(150)
	, targetPoint(NULL)
	, pos({0,0,0,0})
	, position(0,0)
	, velocity(0, 0)
	, accel(0, 50)
	, clip({ 0,0,0,0 })
	, frame(0)
	, prev(0)
	, timer(0)
{
}

GameObject::~GameObject(void)
{
}

void GameObject::Init(string txtfile, SDL_Rect* dest)
{
	img = Manager::LoadTexture(txtfile, &pos);
	prev = SDL_GetTicks();
	timer = 0;
}

//true if eats up a key, false otherwise
bool GameObject::Update()
{
	const Keys r_dragonpunch = { KEYS::RIGHT, KEYS::DOWN, KEYS::RIGHT, KEYS::Z};
	const Keys r_fireball = { KEYS::DOWN, KEYS::RIGHT, KEYS::Z };
	const Keys r_hurricanekick = { KEYS::DOWN, KEYS::LEFT, KEYS::X };

	const Moveset moveset = {
		MoveToAction(r_dragonpunch, "SHOYUUUUKEN!!") ,
		MoveToAction(r_fireball, "HADOOOOOKEN!!") ,
		MoveToAction(r_hurricanekick, "I'D LIKE SOME POUND CAKE!!")
	};


	Uint32 now = SDL_GetTicks();
	dt = (now - prev) / 1000.0;
	timer += dt;
	prev = now;

	if (!active)
		return false;

	angle += rotation * dt;
	
	FollowTarget();
	UpdateAnimation();
	
	return false;
}

void GameObject::Render()
{
	pos.x = (int)position.x;
	pos.y = (int)position.y;
	if (active)
	{
		Manager::RenderTextureEx(img, nullptr, &pos, angle, targetPoint);
	}
}

void GameObject::Move(int direction)
{
	
	if (direction & KEYS::Z)
	{
		Manager::inputHistory.push_back(direction);
	}

	if (direction & KEYS::X)
	{
		Manager::inputHistory.push_back(direction);
	}

	
}

void GameObject::UpdateAnimation()
{
	// Input animation logic (clip animation) here
	if (timer > 1.0 / FPS) // 66ms ~ 1/15 sec
	{
		frame++;
		timer = 0;
	}
}

void GameObject::FollowTarget()
{
	if (target != NULL)
	{
		

		float rotate_angle = (offset.th + movement) *M_PI / 180;

		Vector2 center; // = position + Vector2(clip.w, clip.h) / 2;
		Vector2 target_center = target->position + Vector2(target->pos.w, target->pos.h) / 2;

		center.x = offset.r *cos(rotate_angle) + target->position.x;
		center.y = offset.r *sin(rotate_angle) + target->position.y;
		
		position = center - Vector2(pos.w, pos.h) / 2;

		/*
		//orbiting 
		targetPoint->x = target->position.x + (target->pos.w / 2.0f);
		targetPoint->y = target->position.y + (target->pos.h / 2.0f);

		targetPoint->x -= position.x;
		targetPoint->y -= position.y;

		//rotate around itself
		//targetPoint->x = (target->pos.w / 2.0f);
		//targetPoint->y = (target->pos.h / 2.0f);
		*/

		//SDL_SetRenderDrawColor(Manager::renderer, 0, 255, 0, 255);
		//SDL_RenderDrawLine(Manager::renderer, center.x, center.y, target_center.x, target_center.y);
		/*
		float followSpeed = 1.0f;
		if (name == "trap")
		{
			followSpeed = 50.0f;
		}
		else
		{
			followSpeed = 30.0f;
		}

		// check the distance between the object and the target and if it is greater than something move object to target
		Vector2 directionToTarget = target->position - position;
		float distToTarget = directionToTarget.Magnitude();
		//if (distToTarget > 2 * followSpeed)
		{
			//move object to target
			Vector2 normalizedDirection = directionToTarget.Normalize();
			position += normalizedDirection * followSpeed * dt;
		}
		*/

		/*
		r = magnitude(offfset);
		a = angle*pi/180;
		pos.x = r*cos(angle);
		pos.y = r*sin(angle);
		*/

	}


}
