#pragma once


#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Event;

extern boost::uuids::random_generator uuidGenerator;
extern std::string GenerateUUID();
extern void RenderAll();
extern int FireEvent(Event *e);

static const int
MAP_WIDTH = 130,
MAP_HEIGHT = 65,
SPRITE_WIDTH = 12,
SPRITE_HEIGHT = 12,
WINDOW_WIDTH = (MAP_WIDTH) * SPRITE_WIDTH,
WINDOW_HEIGHT = (MAP_HEIGHT) * SPRITE_HEIGHT;
static SDL_Rect backgroundTile = { 219 % 16 * SPRITE_WIDTH, 219 / 16 * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT };

static const int
AC_STANDARD = 100;

typedef struct Point {
	int x, y;
};

extern bool quit;

namespace Render {
	static SDL_Window *window;
	static SDL_Renderer *renderer;
	static SDL_Texture *spriteSheet;

	void Init(); // Startup calls
	void Exit(); // SDL_Exit, etc.

	void Put(int c, int x, int y, int r, int g, int b, int bgRed = 0, int bgGreen = 0, int bgBlue = 0); // Draw a character to the screen
	void Put(int c, int x, int y, char color, char bgColor = 'x'); // Draw a character to the screen
	void Puts(std::string text, int x, int y, int r, int g, int b, int bgRed = 0, int bgGreen = 0, int bgBlue = 0); // Draw a string to the screen

	SDL_Texture *LoadTexture(std::string filePath, int aR, int aG, int aB); // Load an SDL_Texture from a string filepath. aR,aG,aB are the alpha colors
	void Update(); // Refresh what is on the screen
};