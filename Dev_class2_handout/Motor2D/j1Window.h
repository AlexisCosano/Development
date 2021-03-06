#ifndef __j1WINDOW_H__
#define __j1WINDOW_H__

#include "j1Module.h"

struct SDL_Window;
struct SDL_Surface;

class j1Window : public j1Module
{
public:

	j1Window();

	// Destructor
	virtual ~j1Window();

	// Called before render is available
	bool Awake(JSON_Object* config);

	// Called before quitting
	bool CleanUp();

	// Changae title
	void SetTitle(const char* new_title);

	// Retrive window size
	void GetWindowSize(uint& width, uint& height) const;

	// Retrieve window scale
	uint GetScale() const;

	// Save & Load
	bool Save(JSON_Object* config);
	bool Load(JSON_Object* config);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	p2SString	  title;
	uint		  width;
	uint		  height;
	uint		  scale;
	JSON_Object * window_object;
	bool          fullscreen;
	bool          borderless;
	bool          resizable;
	bool          fullscreen_window;
};

#endif // __j1WINDOW_H__