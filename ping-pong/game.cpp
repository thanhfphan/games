#include <SDL2/SDL.h>
#include "game.hpp"
#include <iostream>

void TRY(int flag)
{
	if (flag != 0)
	{
		std::cout << "Got err= " << SDL_GetError() << std::endl;
	}
}

void Game::setup()
{
	is_running = true;
	screen_height = 900;
	screen_width = 1200;
	last_frame_time = 0;
	delta_time = 0;

	ball.x = 100;
	ball.y = 100;
	ball.w = 10;
	ball.h = 10;
	ball.velocity = {400, 400};

	bar.x = 100;
	bar.y = screen_height - 20;
	bar.w = 100;
	bar.h = 10;
	bar.velocity = {0, 0};

	TRY(SDL_Init(SDL_INIT_EVERYTHING));
	window = SDL_CreateWindow("Ping pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (!window)
	{
		std::cout << "CreateWindow - Got err= " << SDL_GetError() << std::endl;
		exit(1);
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		std::cout << "CreateRenderer - Got err= " << SDL_GetError() << std::endl;
		exit(1);
	}

}

void Game::draw()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect rectBall = {(int)ball.x, (int)ball.y, (int)ball.w, (int)ball.h};
	SDL_RenderFillRect(renderer, &rectBall);

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_Rect rectBar = {(int)bar.x, (int)bar.y, (int)bar.w, (int)bar.h};
	SDL_RenderFillRect(renderer, &rectBar);

	SDL_RenderPresent(renderer);
}

void Game::update()
{
	delta_time = (SDL_GetTicks() - last_frame_time) * 0.001;
	last_frame_time = SDL_GetTicks();

	//ball movement
	ball.x += ball.velocity.x * delta_time;
	ball.y += ball.velocity.y * delta_time;
	// bar movement
	bar.x += bar.velocity.x * delta_time;

	//check border
	if (ball.x < 0 || ball.x > screen_width)
	{
		ball.velocity.x *= -1;
	}
	if (ball.y < 0)
	{
		ball.velocity.y *= -1;
	}
	if (bar.x < 0)
	{
		bar.x = 0;
	}
	if (bar.x + bar.w > screen_width)
	{
		bar.x = screen_width - bar.w;
	}

	// check ball vs bar collision
	if (ball.x >= bar.x && ball.x <= (bar.x + bar.w) && ball.y >= bar.y && ball.y <= (bar.y + bar.h))
	{
		ball.velocity.y *= -1;
	}

	// check endgame
	if (ball.y > screen_height)
	{
		std::cout << "You lose" << std::endl;
		is_running = false;
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
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT)
			{
				bar.velocity.x = 800;
			}
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_LEFT)
			{
				bar.velocity.x = -800;
			}
			break;
		}
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDL_KeyCode::SDLK_RIGHT || event.key.keysym.sym == SDL_KeyCode::SDLK_LEFT)
			{
				bar.velocity.x = 0;
			}
		default:
			break;
		}
	}
}

void Game::quit()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}