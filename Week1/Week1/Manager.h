#include <SDL.h>
#include <unordered_map>

#include "helper.h"
#include "GameObject.h"

#pragma once

namespace MANAGERERROR
{
	enum { SDL_INITFAIL=-10, IMG_INITFAIL, TTF_INITFAIL, WINDOW_INITFAIL };
}

class Manager
{
public:
	Manager(void);
	~Manager(void);

	int Init();

	bool LoadLevel(std::string levelfile);

	bool Update();
	void Close();

	static SDL_Texture* LoadTexture(std::string filename, SDL_Rect* offset=nullptr);
	static void			RenderTextureEx(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* offset, float angle, SDL_Point* rotationPoint);
	static void			RenderTexture(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* offset);
	
	static SDL_Window*		window;
	static SDL_Renderer*	renderer;
	static vector <int> inputHistory;
	static bool match(const Keys& input, const Keys& move, int threshold = 1);

	
private:

	void ApplyMetadata(GameObject* obj, std::string metadata);
	bool KeyUpdate();
	bool KeyDown(SDL_Keycode key);
	bool KeyUp(SDL_Keycode key);

	PhysicsManager* phys_man;

	Uint32 prev;
	float t;

	int		 keys;
	Vector2 mouse_pos;
	
	std::string nextlevel;
	typedef std::unordered_map<std::string, GameObject*> ObjectByName;
	ObjectByName objs;

	GameObject* player;
	GameObject* trap;
	GameObject* bullet;

	SDL_Texture* background;

	const Uint32 scrennwidth;
	const Uint32 scrennheight;
};

