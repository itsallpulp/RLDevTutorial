#include "Globals.h"

void Render::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	spriteSheet = LoadTexture("Markvii12x12.png", 255, 0, 255);
	Update();
}

void Render::Exit()
{
	SDL_Quit();
}

void Render::Put(int c, int x, int y, int r, int g, int b, int bgRed, int bgGreen, int bgBlue)
{
	int cx = c % 16;
	int cy = floor(c / 16);

	SDL_Rect renderQuad = { x * SPRITE_WIDTH, y * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT };
	SDL_Rect clip = { cx * SPRITE_WIDTH, cy * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT };
	SDL_SetTextureColorMod(spriteSheet, bgRed, bgGreen, bgBlue);
	SDL_RenderCopyEx(renderer, spriteSheet, &backgroundTile, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);

	SDL_SetTextureColorMod(spriteSheet, r, g, b);
	SDL_RenderCopyEx(renderer, spriteSheet, &clip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}

SDL_Texture *Render::LoadTexture(std::string filepath, int alphaRed, int alphaGreen, int alphaBlue)
{
	void *pixels;
	int pitch;
	SDL_Texture *newTexture = NULL;
	SDL_Surface *loaded = IMG_Load(filepath.c_str());
	SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loaded, SDL_PIXELFORMAT_RGBA8888, 0);


	newTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, formattedSurface->w, formattedSurface->h);
	SDL_SetTextureBlendMode(newTexture, SDL_BLENDMODE_BLEND);
	SDL_LockTexture(newTexture, &formattedSurface->clip_rect, &pixels, &pitch);
	memcpy(pixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

	Uint32 *tpixels = (Uint32 *)pixels;
	int pixelCount = (pitch / 4) * formattedSurface->h;

	Uint32 colorKey = SDL_MapRGB(formattedSurface->format, alphaRed, alphaGreen, alphaBlue);
	Uint32 transparent = SDL_MapRGBA(formattedSurface->format, 0xFF, 0xFF, 0xFF, 0x00);

	for (int i = 0; i < pixelCount; ++i)
	{
		if (tpixels[i] == colorKey)
		{
			tpixels[i] = transparent;
		}
	}

	SDL_UnlockTexture(newTexture);
	pixels = NULL;
	SDL_FreeSurface(formattedSurface);
	SDL_FreeSurface(loaded);

	return newTexture;
}

void Render::Update()
{
	uint32_t frameStart = SDL_GetTicks();
	SDL_RenderPresent(renderer);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);
	int frame_time = SDL_GetTicks() - frameStart;
}

void Render::Puts(std::string text, int x, int y, int r, int g, int b, int bgRed, int bgGreen, int bgBlue)
{
	int i = 0;
	for (char c : text)
	{
		Put((int)c, x + i, y, r, g, b, bgRed, bgGreen, bgBlue);
		++i;
	}
}
