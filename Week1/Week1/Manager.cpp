#include <fstream>
#include <sstream>
#include <SDL_image.h>
#include <stdio.h>

#include "Manager.h"

namespace
{
	std::vector <std::string> split(std::string metadata)
	{
		std::istringstream in(metadata);
		std::vector <std::string> result;
		std::string token;

		while (in >> token)
		{
			result.push_back(token);
		}

		return result;
	}
}

SDL_Window* 	Manager::window = nullptr;
SDL_Renderer*	Manager::renderer = nullptr;
vector <int> Manager::inputHistory;


Manager::Manager(void): keys(KEYS::NONE), mouse_pos(0,0), player(nullptr), background(nullptr), scrennwidth(SCREEN_WIDTH), scrennheight(SCREEN_HEIGHT)
{
	t = 0;
}

Manager::~Manager(void)
{
}

int Manager::Init()
{
	//start sdl
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 )
		return MANAGERERROR::SDL_INITFAIL;

	//start img
	int flags = IMG_INIT_PNG | IMG_INIT_JPG;
	if ( IMG_Init( flags ) != flags )
		return MANAGERERROR::IMG_INITFAIL;

	window = SDL_CreateWindow("2D Graphics",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	if ( !window )
		return MANAGERERROR::WINDOW_INITFAIL;

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

	LoadLevel("level1.txt");

	prev = SDL_GetTicks();

	return 0;
}

bool Manager::LoadLevel(std::string levelfile)
{
	std::ifstream in;
	in.open(levelfile);

	if ( in.fail() )
		exit(99999);

	std::string type, file, name;
	int x,y,w,h;

	while ( !in.eof() )
	{
		in >> type;
		if (in.fail())
			continue;

		if (type == "bg")
		{
			in >> file;
			background = LoadTexture(file);
			//objs.insert("bg", new GameObject);
			//objs["bg"]->Init(file);
		}
		else if (type == "Trap" || type == "Bullet" || type == "Player" || type == "Object")
		{
			in >> name >> file >> x >> y >> w >> h;
			objs[name] = new GameObject;
			//objs.insert( name, new GameObject );
			objs[name]->Init( file );
			objs[name]->position.x = (float)x;
			objs[name]->position.y = (float)y;
			objs[name]->pos.w = (float)w;
			objs[name]->pos.h = (float)h;
			objs[name]->name = name;

			if (type == "Player")
			{
				player = objs[name];
			}
			else if (type == "Trap")
			{
				trap = objs[name];
			}
			else if (type == "Bullet")
			{
				bullet = objs[name];
			}

			std::string metadata;
			getline(in, metadata);

			ApplyMetadata(objs[name], metadata);

		}
		else if (type == "Nextlevel")
		{
			in >> nextlevel;
		}
	}

	in.close();
	return true;
}

bool Manager::Update()
{
	if ( !KeyUpdate() )
		return false;
	
	if (keys & KEYS::UP) {}
	if (keys & KEYS::DOWN) {}
	if (keys & KEYS::LEFT) {}
	if (keys & KEYS::RIGHT) {}

	// TODO add stuff
	Vector2 p0(0,0);
	Vector2 p1(SCREEN_WIDTH, SCREEN_HEIGHT);
	float dist = p1.Magnitude() - p0.Magnitude();

	Uint32 now = SDL_GetTicks();
	float dt = (now - prev) / 1000.0f;
	prev = now;

	t += 100 * dt / dist;
	t = t > 1 ? 1 : t;
	player->position = slerp(p0, p1, t, M_PI/4);

	SDL_RenderClear( renderer );
	SDL_RenderCopy( renderer, background, nullptr, nullptr);
	
	for (ObjectByName::value_type& objByName : objs)
	{
		GameObject* obj = objByName.second;
		obj->Move(keys);// i == 0 ? keys : 0);
		obj->Update();


		
	}
	
	for (ObjectByName::value_type& objByName : objs)
	{
		GameObject* obj = objByName.second;
		obj->Render();
	}

	//Enter movement logic here
	if (keys & KEYS::UP)
	{
		trap->movement -= 5.5;
	}

	if (keys & KEYS::DOWN)
	{
		trap->movement += 5.5;
	}

	if (keys & KEYS::LEFT)
	{
		bullet->movement += 5;
	}

	if (keys & KEYS::RIGHT)
	{
		bullet->movement -= 5;
	}


	
	SDL_RenderPresent( renderer );

	return true;
}

void Manager::Close()
{
	if ( renderer != nullptr )
	{
		SDL_DestroyRenderer( renderer );
		renderer = nullptr;
	}
	SDL_DestroyWindow( window );

	IMG_Quit();
	SDL_Quit();
}


SDL_Texture* Manager::LoadTexture(std::string filename, SDL_Rect* offset)
{
	if ( renderer == nullptr )
		exit( 403 );

	SDL_Surface* surface = nullptr;
	SDL_Texture* texture = nullptr;

	surface = IMG_Load( filename.c_str() );
	if ( surface == nullptr )
		exit( 404 );
	
	if ( offset != nullptr )
	{
		offset->w = surface->w;
		offset->h = surface->h;
	}
	
	texture = SDL_CreateTextureFromSurface( renderer, surface );

	SDL_FreeSurface( surface );
	return texture;
}

void Manager::RenderTextureEx(SDL_Texture * texture, SDL_Rect * clip, SDL_Rect * offset, float angle, SDL_Point* rotationPoint)
{
	SDL_RenderCopyEx(renderer, texture, clip, offset, angle, rotationPoint, SDL_FLIP_NONE);
}

void Manager::RenderTexture(SDL_Texture* texture, SDL_Rect* clip, SDL_Rect* offset)
{
	SDL_RenderCopy( renderer, texture, clip, offset );
}

bool Manager::match(const Keys & input, const Keys & move, int threshold)
{
		// not enough keys to do move
		int input_size = input.size();
		int move_size = move.size();

		if (input_size < move_size)
			return false;

		int mismatch = 0;

		int j = input_size - 1;
		int i = move_size - 1;

		// Very latest input must match
		if (input[j] != move[i])
			return false;

		for (; i >= 0 && j >= 0; --j)
		{
			if (input[j] != move[i]) {
				++mismatch;
			}
			else
			{
				mismatch = 0;
				--i;
			}

			if (mismatch >= threshold)
				return false;
		}
		return j >= 0;
}

void Manager::ApplyMetadata(GameObject* obj, std::string metadata)
{
	std::vector<std::string> data = split(metadata);

	for (int i = 0; i < data.size(); ++i)
	{
		if (data[i] == "Follow")
		{
			std::string target = data[i + 1];
			std::string distance = data[i + 2];
			std::string angle = data[i + 3];
			obj->target = objs[target];
			obj->targetPoint = new SDL_Point();
			
			obj->offset.r = stof(distance);
			obj->offset.th = stof(angle);

			i += 3;
		}
	}
}

bool Manager::KeyUpdate()
{
	int x,y;
	SDL_GetMouseState(&x, &y);
	mouse_pos.x = (float)x;
	mouse_pos.y = (float)y;

	SDL_Event e;
	while ( SDL_PollEvent( &e ) )
	{
		switch( e.type )
		{
		case SDL_QUIT:
			return false;

		case SDL_KEYDOWN:
			if ( !KeyDown( e.key.keysym.sym ) )
				return false;
			break;

		case SDL_KEYUP:
			if ( !KeyUp( e.key.keysym.sym ) )
				return false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if ( e.button.button == SDL_BUTTON_LEFT )
				keys |= KEYS::MOUSE0;
			if ( e.button.button == SDL_BUTTON_RIGHT )
				keys |= KEYS::MOUSE1;
			break;

		case SDL_MOUSEBUTTONUP:
			if ( e.button.button == SDL_BUTTON_LEFT )
				keys &= (~KEYS::MOUSE0);
			if ( e.button.button == SDL_BUTTON_RIGHT )
				keys &= (~KEYS::MOUSE1);
			break;
		}
	}
	return true;
}

bool Manager::KeyDown(SDL_Keycode key)
{
	switch( key )
	{
	case SDLK_ESCAPE:
		return false;
		break;

	case SDLK_UP:
	case 'w':
		keys |= KEYS::UP;
		break;
	
	case SDLK_DOWN:
	case 's':
		keys |= KEYS::DOWN;
		break;

	case SDLK_LEFT:
	case 'a':
		keys |= KEYS::LEFT;
		break;

	case SDLK_RIGHT:
	case 'd':
		keys |= KEYS::RIGHT;
		break;

	case SDLK_SPACE:
		keys |= KEYS::SPACE;
		break;

	case SDLK_z:
		keys |= KEYS::Z;
		break;

	case SDLK_x:
		keys |= KEYS::X;
		break;

	}

	return true;
}

bool Manager::KeyUp(SDL_Keycode key)
{
	switch( key )
	{
	case SDLK_ESCAPE:
		return false;
		break;

	case SDLK_UP:
	case 'w':
		keys &= ~KEYS::UP;
		break;
	
	case SDLK_DOWN:
	case 's':
		keys &= ~KEYS::DOWN;
		break;

	case SDLK_LEFT:
	case 'a':
		keys &= ~KEYS::LEFT;
		break;

	case SDLK_RIGHT:
	case 'd':
		keys &= ~KEYS::RIGHT;
		break;

	case SDLK_SPACE:
		keys &= ~KEYS::SPACE;
		break;
		
	case SDLK_z:
		keys &= ~KEYS::Z;
		break;

	case SDLK_x:
		keys &= ~KEYS::X;
		break;

	}

	return true;
}