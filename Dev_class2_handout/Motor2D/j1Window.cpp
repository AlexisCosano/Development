#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"

#include "SDL/include/SDL.h"


j1Window::j1Window() : j1Module()
{
	window = NULL;
	screen_surface = NULL;
	name.create("window");
}

// Destructor
j1Window::~j1Window()
{
}

// Called before render is available
bool j1Window::Awake(JSON_Object* config)
{
	LOG("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		Uint32 flags = SDL_WINDOW_SHOWN;
		window_object = json_object_dotget_object(App->configuration_object, "window");

		title = json_object_dotget_string(window_object, "wtitle");

		width = json_object_dotget_number(window_object, "wwidth");
		height = json_object_dotget_number(window_object, "wheight");
		scale = json_object_dotget_number(window_object, "wscale");
		
		fullscreen = json_object_dotget_boolean(window_object, "fullscreen");
		borderless = json_object_dotget_boolean(window_object, "borderless");
		resizable = json_object_dotget_boolean(window_object, "resizable");
		fullscreen_window = json_object_dotget_boolean(window_object, "fullscreenborderless");

		if(fullscreen)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(borderless)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(resizable)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(fullscreen_window)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(title.GetString(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool j1Window::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

// Set new window title
void j1Window::SetTitle(const char* new_title)
{
	//title.create(new_title);
	SDL_SetWindowTitle(window, new_title);
}

void j1Window::GetWindowSize(uint& width, uint& height) const
{
	width = this->width;
	height = this->height;
}

uint j1Window::GetScale() const
{
	return scale;
}

// Save & Load -------------------------- 
bool j1Window::Save(JSON_Object* config)
{
	bool ret = true;

	return(ret);
}

bool j1Window::Load(JSON_Object* config)
{
	bool ret = true;

	return(ret);
}