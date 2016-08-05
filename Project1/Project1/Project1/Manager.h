#include <SDL.h>
#include <map>

#include "helper.h"
#include "GameObject.h"

#pragma once

namespace MANAGERERROR
{
	enum { SDL_INITFAIL = -10, IMG_INITFAIL, TTF_INITFAIL, WINDOW_INITFAIL };
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

	static SDL_Texture* LoadTexture(std::string filename, SDL_Rect* offset = nullptr);
	static void			RenderTexture(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* offset);
	static void			RenderTextureEx(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* offset, float angle, SDL_Point* rotationPoint);
	static SDL_Window*		window;
	static SDL_Renderer*	renderer;

private:

	void ApplyMetadata(GameObject* obj, std::string metadata);


	bool KeyUpdate();
	bool KeyDown(SDL_Keycode key);
	bool KeyUp(SDL_Keycode key);

	PhysicsManager* phys_man;

	int		 keys;
	Vector2 mouse_pos;

	std::string nextlevel;
	typedef std::map<std::string, GameObject*> ObjectByName;
	ObjectByName objs;

	GameObject* player;

	SDL_Texture* background;

	const Uint32 screenwidth;
	const Uint32 screenheight;
};

