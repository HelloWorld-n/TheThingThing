#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <thread>

const int screenWidth = 760;
const int screenHeight = 360;
const SDL_Color bg = {0, 0, 0, 255};
const SDL_Color fg = {196, 196, 196, 255};
const SDL_Color sdl_blue = {64, 64, 255, 255};
const SDL_Color sdl_yellow = {228, 228, 32, 255};
const int fontSize_main = 24;
const int fontSize_side = 12;

void SDL_DrawText(
	SDL_Renderer *renderer, int x, int y, char *text,
	TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect,
	SDL_Color bg = ::bg, SDL_Color fg = ::fg
) {
	int text_width;
	int text_height;
	SDL_Surface *surface;

	surface = TTF_RenderText_Solid(font, text, fg);
	*texture = SDL_CreateTextureFromSurface(renderer, surface);
	text_width = surface->w;
	text_height = surface->h;

	SDL_FreeSurface(surface);
	rect->x = x;
	rect->y = y;
	rect->w = text_width;
	rect->h = text_height;
}

void handleEvents(SDL_Window *window, bool mainWindow = false){
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0){
		if (event.type == SDL_QUIT){
			SDL_DestroyWindow(window);
			SDL_Quit();
			if (mainWindow){
				std::exit(0);
			}
		}
	}
}

std::string getGlobalTime(std::time_t the_time_t){
    char buf[sizeof "????-??-??T??:??:??Z"];
    strftime(buf, sizeof buf, "%FT%TZ", std::gmtime(&the_time_t));
	return ( std::string ) buf;
}

std::string getLocalTime(std::time_t the_time_t){
    char buf[sizeof "????-??-??T??:??:??±??:??"];
    strftime(buf, sizeof buf, "%FT%T%z", std::localtime(&the_time_t));
	return ( std::string ) buf;
}

int enlimit(int num, int limit){
	if (num % (limit * 2) < limit){
		return num % limit;
	} else {
		return limit - (num % limit);
	}
}

int main(int argc, char* args[]) {
	const char *fontPath = "fonts/CascadiaMono.ttf";
	SDL_Window *window = NULL;
	SDL_Surface *screenSurface = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;

	TTF_Init();
	TTF_Font *font_main = TTF_OpenFont(fontPath, fontSize_main);
	TTF_Font *font_side = TTF_OpenFont(fontPath, fontSize_side);
	SDL_Surface *surfaceMessage = TTF_RenderText_Solid(font_main, getLocalTime(time(0)).c_str(), fg); 
	SDL_Rect rect;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "Error: \e48;2;0;0;0m\e[38;2;255;0;0m" << SDL_GetError() << "\e[0m;\n";
		return -418;
	}
	
	SDL_CreateWindowAndRenderer(screenWidth, screenHeight, 0, &window, &renderer);
	SDL_SetWindowTitle(window, "TheThingThing");

	if (window == NULL) {
		std::cerr << "Error: \e48;2;0;0;0m\e[38;2;255;0;0m" << SDL_GetError() << "\e[0m;\n";
		return -404;
	}
	screenSurface = SDL_GetWindowSurface(window);
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, bg.r, bg.g, bg.b));
	SDL_UpdateWindowSurface(window);
	
	while (true){
		time_t now;
		time(&now);
		std::string the_now_local = getLocalTime(time(0));
		std::string the_now_global = getGlobalTime(time(0));
		
		the_now_global.pop_back();
		the_now_global.append("(UTC)");

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
		SDL_DrawText(
			renderer, 10, (screenHeight / 2) - fontSize_main, ( char * ) the_now_local.c_str(), 
			font_main, &texture, &rect,
			bg, sdl_blue
		);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
		SDL_DrawText(
			renderer, 10, screenHeight / 2, ( char * ) the_now_global.c_str(), 
			font_main, &texture, &rect,
			bg, sdl_yellow
		);
		SDL_RenderCopy(renderer, texture, NULL, &rect);
        SDL_RenderPresent(renderer);
	
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		SDL_UpdateWindowSurface(window);
		handleEvents(window, true);
	}
	return 0;
}
