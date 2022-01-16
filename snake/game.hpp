#pragma once
#include <SDL2/SDL.h>

struct Vector2f
{
	int x, y;
};

struct Body
{
	float x, y;
	float w, h;
	Vector2f velocity;
	Body *next_body;
};

struct Snake
{
	Body *head, *tail;
};

struct Food {
	int x,y;
	int w,h;
	bool can_eat;
};

class Game
{
	int screen_width, screen_height;
	int last_frame_time;
	int player_speed;
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool is_running;
	Snake player;
	Food food;

public:
	Game();
	~Game();
	void run();

private:
	void process_input();
	void update();
	void render();
	void quit();
};