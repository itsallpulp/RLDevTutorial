#include "Globals.h"
#include "Color.h"

void Render::Init()
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	window = SDL_CreateWindow("RoguelikeDev Tutorial Week 3", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetScale(renderer, 0.1, 0.1);
	SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
	SDL_SetWindowResizable(window, SDL_TRUE);
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	spriteSheet = LoadTexture("Markvii12x12big.png", 255, 0, 255);
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

	SDL_Rect renderQuad = { x * SPRITE_WIDTH / SCALE, y * SPRITE_HEIGHT / SCALE, SPRITE_WIDTH / SCALE, SPRITE_HEIGHT / SCALE };
	SDL_Rect clip = { cx * SPRITE_WIDTH, cy * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT };
	SDL_SetTextureColorMod(spriteSheet, bgRed, bgGreen, bgBlue);
	SDL_RenderCopyEx(renderer, spriteSheet, &backgroundTile, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);

	SDL_SetTextureColorMod(spriteSheet, r, g, b);
	SDL_RenderCopyEx(renderer, spriteSheet, &clip, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);
}

void Render::Put(int c, int x, int y, char color, char bgColor)
{
	int cx = c % 16;
	int cy = floor(c / 16);

	SDL_Rect renderQuad = { x * SPRITE_WIDTH / SCALE, y * SPRITE_HEIGHT / SCALE, SPRITE_WIDTH / SCALE, SPRITE_HEIGHT / SCALE};
	SDL_Rect clip = { cx * SPRITE_WIDTH, cy * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT };

	SDL_SetTextureColorMod(spriteSheet, colors[bgColor].r, colors[bgColor].g, colors[bgColor].b);
	SDL_RenderCopyEx(renderer, spriteSheet, &backgroundTile, &renderQuad, 0.0, NULL, SDL_FLIP_NONE);

	SDL_SetTextureColorMod(spriteSheet, colors[color].r, colors[color].g, colors[color].b );
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

void Render::Puts(std::string text, int x, int y, char color, char bgColor)
{
	int i = 0;
	for (char c : text)
	{
		Put((int)c, x + i, y, color, bgColor);
		++i;
	}
}

void Render::PutBorder(int x, int y, int width, int height, char color, char bgColor, bool thick)
{
	int glyphs[6] = { 218, 192, 191, 217, 196, 179 };

	if (thick)
	{
		glyphs[0] = 201; // Top Left
		glyphs[1] = 200; // Bottom Left
		glyphs[2] = 187; // Top Right
		glyphs[3] = 188; // Bottom Right
		glyphs[4] = 205; // Horizontal
		glyphs[5] = 186; // Vertical
	}

	for (int i = 0; i < width; ++i)
	{
		Put(glyphs[4], x + i, y, color, bgColor);
		Put(glyphs[4], x + i, y+height-1, color, bgColor);
	}

	for (int i = 0; i < height; ++i)
	{
		Put(glyphs[5], 0, y + i, color, bgColor);
		Put(glyphs[5], width-1, y + i, color, bgColor);
	}

	Put(glyphs[0], x, y, color, bgColor);
	Put(glyphs[1], x, y+height-1, color, bgColor);
	Put(glyphs[2], x+width-1, y, color, bgColor);
	Put(glyphs[3], x+width-1, y+height-1, color, bgColor);
}

void Render::PutTitledBorder(std::string title, int x, int y, int width, int height, char color, char bgColor, int opts)
{

	PutBorder(x, y, width, height, color, bgColor, opts & BORDER_THICK);

	int offset = 2; 

	if (opts & BORDER_TITLE_CENTER)
	{
		offset = (width / 2) - (title.size() / 2);
	}
	else if (opts & BORDER_TITLE_RIGHT)
	{
		offset = width - 2 - title.size();
	}

	int left = opts & BORDER_THICK ? 181 : 180,
		right = opts & BORDER_THICK ? 198 : 195;

	Put(left, x + offset - 1, y, color, bgColor);
	Put(right, x + title.size() + offset, y, color, bgColor);
	Puts(title, x + offset, y, color, bgColor);

	if (opts & FILL_BACKGROUND)
	{
		for (int i = x + 1; i < x + width + -1; ++i)
		{
			for (int j = y + 1; j < y + height - 1; ++j)
			{
				Render::Put(219, i, j, bgColor, bgColor);
			}
		}
	}

}
