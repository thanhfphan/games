#include "game.hpp"
#include "helper.hpp"
#include <iostream>
#include <SDL2/SDL.h>

Game::Game()
{
	is_running = true;
	screen_height = 900;
	screen_width = 1200;
	last_frame_time = 0;
	player_speed = 200;
	Body *first_part = new Body{200, 300, 10, 10, {player_speed, 0}, NULL};
	player = {first_part, first_part};
	food = {500, 500, 10, 10, true};

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Init error " << SDL_GetError() << std::endl;
		exit(1);
	}
	window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
}

Game::~Game()
{
	quit();
}

void Game::run()
{
	while (is_running)
	{
		process_input();
		update();
		render();
	}
}

void Game::process_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_UP)
			{
				player.head->velocity.x = 0;
				player.head->velocity.y = -player_speed;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_DOWN)
			{
				player.head->velocity.x = 0;
				player.head->velocity.y = player_speed;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_LEFT)
			{
				player.head->velocity.x = -player_speed;
				player.head->velocity.y = 0;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT)
			{
				player.head->velocity.x = player_speed;
				player.head->velocity.y = 0;
			}
			break;
		}
		default:
			break;
		}
	}
}

void Game::update()
{
	float delta_time = (SDL_GetTicks() - last_frame_time) * 0.001;
	last_frame_time = SDL_GetTicks();
	Body *tmp = player.head;
	while (tmp)
	{
		tmp->x += tmp->velocity.x * delta_time;
		tmp->y += tmp->velocity.y * delta_time;
		tmp = tmp->next_body;
	}

	// check collision food vs snake head
	if (food.can_eat && rectangle_intersect({food.x, food.y, food.w, food.h}, {(int)player.head->x, (int)player.head->y, (int)player.head->w, (int)player.head->h}))
	{
		float new_x = 0;
		float new_y = 0;
		if (player.tail->velocity.x > 0)
		{
			new_x = player.tail->x - 10;
			new_y = player.tail->y;
		}
		else if (player.tail->velocity.x < 0)
		{
			new_x = player.tail->x + 10;
			new_y = player.tail->y;
		}
		else if (player.tail->velocity.y > 0)
		{
			new_x = player.tail->x;
			new_y = player.tail->y - 10;
		}
		else if (player.tail->velocity.y < 0)
		{
			new_x = player.tail->x;
			new_y = player.tail->y + 10;
		}
		Body *new_part = new Body{new_x, new_y, player.tail->w, player.tail->h, {player.tail->velocity.x, player.tail->velocity.y}};
		player.tail->next_body = new_part;
		player.tail = new_part;

		food.can_eat = false;
	}

	if (!food.can_eat)
	{
		food.x = rand() % screen_width;
		food.y = rand() % screen_height;
		food.can_eat = true;
	}

	// reset position when head out of the screen
	player.head->x = fmod(player.head->x, screen_width);
	player.head->y = fmod(player.head->y, screen_height);
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);

	//render player
	Body *tmp = player.head;
	while (tmp)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_Rect rect_snake = {(int)tmp->x, (int)tmp->y, (int)tmp->w, (int)tmp->h};
		SDL_RenderFillRect(renderer, &rect_snake);
		tmp = tmp->next_body;
	}

	//render food
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect rect_food = {food.x, food.y, food.h, food.w};
	SDL_RenderFillRect(renderer, &rect_food);

	SDL_RenderPresent(renderer);
}

void Game::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}