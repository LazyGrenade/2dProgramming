#include <fstream>
#include <iostream>
#include <SDL_image.h>

#include "GameObject.h"
#include "Manager.h"

GameObject::GameObject(void)
	: target(nullptr)
	, active(true)
	, surf(nullptr)
	, img(nullptr)
	, angle(0)
	, targetPoint(nullptr)
	, roatationSpeed(100)
	, pos({ 0,0,0,0 })
	, position(0, 0)
	, velocity(0, 0)
	, accel(0, 50)
	, clip({ 0,0,0,0 })
	, frame(0)
	, prev(0)
	, timer(0)
{
	comboType = NONE;
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
	Uint32 now = SDL_GetTicks();
	dt = (now - prev) / 1000.0;
	timer += dt;
	prev = now;

	const Keys r_fireball = { KEYS::DOWN, KEYS::RIGHT, KEYS::P};
	const Keys r_dragonpunch = { KEYS::RIGHT, KEYS::DOWN, KEYS::RIGHT, KEYS::P};
	const Keys r_hurricanekick = { KEYS::DOWN, KEYS::RIGHT, KEYS::K };

	const Combos comboList = {
		Combo(r_fireball, "FireBall"),
		Combo(r_dragonpunch, "DragonPunch"),
		Combo(r_hurricanekick, "HurricaneKick") };

	if (!active)
		return false;

	FollowTarget();

	//angle += roatationSpeed * dt;
	
	
	for (const Combo& moveToAction : comboList) 
	{
		const Keys& move = moveToAction.first;
		const std::string& action = moveToAction.second;
		
		if (Manager::inputHistory.size() >= move.size())
		{
			if (Manager::match(Manager::inputHistory, move, 10))
			{
				std::cout << action << std::endl;
				if (action == "FireBall")
					comboType = FIREBALL;

				break;
			}
		}
	}

	switch (comboType)
	{
	case FIREBALL:
		cout << "Fire Ball" << endl;
		Manager::inputHistory.clear();
		system("pause");
		break;

	default:
		cout << "nothing" << endl;
	}

	UpdateAnimation();

	return false;
}
void GameObject::FollowTarget()
{
	if (target == nullptr)
		return;

	target_behavior = TargetBehavior::NONE;

	float angle = 180 * SDL_GetTicks() / 1000.0f;
	float rotate_angle = (offset.th + angle) * M_PI / 180;


	Vector2 target_pivot = target->position + Vector2(target->pos.w, target->pos.h) / 2;



	Vector2 center;
	center.x = offset.r * cos(rotate_angle) + target_pivot.x;
	center.y = offset.r * sin(rotate_angle) + target_pivot.y;

	position = center - Vector2(pos.w, pos.h) / 2;

}

void GameObject::Render()
{
	pos.x = (int)position.x;
	pos.y = (int)position.y;
	if (active)
	{
		Manager::RenderTextureEx(img, nullptr, &pos, angle, nullptr);
	}

}

void GameObject::Move(int direction)
{
	//Enter movement logic here
	if (direction & KEYS::UP) 
	{
		Manager::inputHistory.push_back(direction);
		position = Vector2(0.0f, -1.0f) * 80.0f * dt + position;
	}
	if (direction & KEYS::DOWN) 
	{
		Manager::inputHistory.push_back(direction);
		position = Vector2(0.0f, 1.0f) * 80.0f * dt + position;
	}
	if (direction & KEYS::LEFT) 
	{
		Manager::inputHistory.push_back(direction);
		position = Vector2(-1.0f, 0.0f) * 80.0f * dt + position;
	}
	if (direction & KEYS::RIGHT) 
	{
		Manager::inputHistory.push_back(direction);
		position = Vector2(1.0f, 0.0f) * 80.0f * dt + position;
	}
	if (direction & KEYS::P)
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