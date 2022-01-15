#pragma once
#include <SDL2/SDL.h>

struct Vector2f
{
	float x, y;
};

struct Object
{
	float x, y;
	float w, h;
	Vector2f velocity;
};

class Game
{
public:
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool is_running;
	int screen_width, screen_height;
	Object ball, bar;
	float delta_time;
	int last_frame_time;

public:
	void setup();
	void process_input();
	void update();
	void draw();
	void quit();
};