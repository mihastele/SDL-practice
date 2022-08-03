#include <stdio.h>
#include <SDL2/SDL.h>
#include "./constants.h"

int game_is_running = FALSE;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
struct ball {
	float x;
	float y;
	float width;
	float height;
} ball;

int last_frame_time = 0;

int initialize_window(void){
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr, "Error Init SDL\n");
		return FALSE;
	}

	window = SDL_CreateWindow(
			NULL,
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_BORDERLESS
			);

	if(!window) {
		fprintf(stderr, "GG\n");
		return FALSE;
	}
	renderer = SDL_CreateRenderer(window, -1, 0); // -1 = default driver, 0 = no special way of rendering
	if(!renderer) {
		fprintf(stderr, "Error SDL render");
		return FALSE;
	}
	return TRUE;
}


void setup() {
	ball.x = 20;
	ball.y = 20;
	ball.width = 15;
	ball.height = 15;
}

void process_input(){
	SDL_Event event; // struct ref
	SDL_PollEvent(&event);

	switch(event.type) {
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				game_is_running = FALSE;
			}
			break;
	}
}

void update(){
	// Frame0 >--- delta time --> Frame1
	// while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
	//
	

	int delta = SDL_GetTicks() - last_frame_time;

	// Sleep the execution until we reack the target frame time in milliseconds
	int time_to_wait = FRAME_TARGET_TIME - delta;

	// Only call delay if we are too fast to process this frame
	if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
		SDL_Delay(time_to_wait);
	}

	// Get a delta time factor converted to seconds to be used to update my objects
	float delta_time = delta / 1000.0f;

	printf("delta time %5.5f\n", delta_time);

	last_frame_time = SDL_GetTicks();	

	ball.x += delta;
	ball.y += delta;
}

void render(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	// TODO start drawing game objects 
	
	SDL_Rect ball_rect = {
	       (int)ball.x,
	       (int)ball.y,
	       (int)ball.width,
	       (int)ball.height 
	};

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &ball_rect);

	SDL_RenderPresent(renderer); // buffer swap



}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main() {
	game_is_running = initialize_window();

	setup();
	while(game_is_running) {
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}
