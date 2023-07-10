#pragma once

#include "Types.h"

#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

class Entity;
class Event;
class Level;
class Pathfinder;
class MovementCommand;

extern boost::uuids::random_generator uuidGenerator;
extern std::string GenerateUUID();
extern void RenderAll();
extern int FireEvent(Event *e);
extern json::object GetJson(std::string filename);
extern double Distance(point start, point end);

extern Level *level;
extern Pathfinder *pathfinder;
extern MovementCommand *FollowPath(Entity *target, std::stack<point> *path);

static const int
MAP_WIDTH = 130,
MAP_HEIGHT = 65,
SCALE = 10,
SPRITE_WIDTH = 12 * SCALE,
SPRITE_HEIGHT = 12 * SCALE,
WINDOW_WIDTH = (MAP_WIDTH) * SPRITE_WIDTH / SCALE,
WINDOW_HEIGHT = (MAP_HEIGHT) * SPRITE_HEIGHT / SCALE;
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
	void Puts(std::string text, int x, int y, char color, char bgColor = 'x'); // Draw a string to the screen
	void PutBorder(int x, int y, int width, int height, char color, char bgColor = 'x', bool thick=false); // Display a border
	void PutTitledBorder(std::string title, int x, int y, int width, int height, char color, char bgColor = 'x', bool thick=false); // Display a border with a centered title

	SDL_Texture *LoadTexture(std::string filePath, int aR, int aG, int aB); // Load an SDL_Texture from a string filepath. aR,aG,aB are the alpha colors
	void Update(); // Refresh what is on the screen
};