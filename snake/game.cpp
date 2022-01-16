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
	player_speed = 200.f;
	Body *init_body = new Body{200, 300, 10, 10, {player_speed, 0}, NULL};
	player = {init_body, init_body};
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
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_UP && player.head->velocity.y <= 0)
			{
				player.head->velocity.x = 0;
				player.head->velocity.y = -player_speed;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_DOWN && player.head->velocity.y >= 0)
			{
				player.head->velocity.x = 0;
				player.head->velocity.y = player_speed;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_LEFT && player.head->velocity.x <= 0)
			{
				player.head->velocity.x = -player_speed;
				player.head->velocity.y = 0;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT && player.head->velocity.x >= 0)
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

	player.head->x += player.head->velocity.x * delta_time;
	player.head->y += player.head->velocity.y * delta_time;
	Body *tmp = player.tail;
	while (tmp && tmp->next_body)
	{
		std::cout << tmp->x << std::endl;
		tmp->x = tmp->next_body->x;
		tmp->y = tmp->next_body->y;
		tmp = tmp->next_body;
	}
	// check collision food vs snake head
	if (food.can_eat && rectangle_intersect({food.x, food.y, food.w, food.h}, {(int)player.head->x, (int)player.head->y, (int)player.head->w, (int)player.head->h}))
	{
		float new_x = 0;
		float new_y = 0;
		if (player.head->velocity.x > 0)
		{
			new_x = player.head->x + 10;
			new_y = player.head->y;
		}
		else if (player.head->velocity.x < 0)
		{
			new_x = player.head->x - 10;
			new_y = player.head->y;
		}
		else if (player.head->velocity.y > 0)
		{
			new_x = player.head->x;
			new_y = player.head->y + 10;
		}
		else if (player.head->velocity.y < 0)
		{
			new_x = player.head->x;
			new_y = player.head->y - 10;
		}
		Body *new_part = new Body{new_x, new_y, player.head->w, player.head->h, {player.head->velocity.x, player.head->velocity.y}};
		player.head->next_body = new_part;
		player.head = player.head->next_body;

		food.can_eat = false;
	}

	if (!food.can_eat)
	{
		food.x = (rand() % screen_width) / 10 * 10;
		food.y = (rand() % screen_height) / 10 * 10;
		food.can_eat = true;
	}

	// reset position when move out of the screen
	if (player.head->x < 0)
	{
		player.head->x = screen_width;
	}
	if (player.head->x > screen_width)
	{
		player.head->x = 0;
	}
	if (player.head->y < 0)
	{
		player.head->y = screen_height;
	}
	if (player.head->y > screen_height)
	{
		player.head->y = 0;
	}
}

void Game::render()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);

	//render player
	Body *tmp = player.tail;
	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	while (tmp)
	{
		SDL_Rect rect_snake = {(int)tmp->x, (int)tmp->y, (int)tmp->w, (int)tmp->h};
		SDL_RenderFillRect(renderer, &rect_snake);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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