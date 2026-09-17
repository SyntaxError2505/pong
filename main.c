#include <stdio.h>
#include <assert.h>
#include <SDL3/SDL.h>

#define WIDTH 800
#define HEIGHT 600

#define PEDAL_WIDTH 10
#define PEDAL_HEIGHT 100

#define PEDAL_SPEED 0.00001

SDL_Window* window;
SDL_Renderer* renderer;

SDL_FRect left_pedal = {
    .x = 0,
    .y = HEIGHT/2 - PEDAL_HEIGHT/2,
    .w = PEDAL_WIDTH,
    .h = PEDAL_HEIGHT,
};

SDL_FRect right_pedal = {
    .x = WIDTH - PEDAL_WIDTH,
    .y = HEIGHT/2 - PEDAL_HEIGHT/2,
    .w = PEDAL_WIDTH,
    .h = PEDAL_HEIGHT,
};


int main(void){
    assert(SDL_Init(SDL_INIT_VIDEO));
    assert(SDL_CreateWindowAndRenderer("Pong", WIDTH, HEIGHT, SDL_WINDOW_VULKAN, &window, &renderer));

    Uint64 old_time = SDL_GetTicksNS();

    for(;;){
        // meassure delta time
        Uint64 new_time = SDL_GetTicksNS();
        Uint64 delta_time = new_time - old_time;
        old_time = new_time;

        // print debug info
        printf("Delta Time: %li\nFramerate: %f\n", delta_time, 1000/((double)delta_time / 1000000.0));

        // poll events
        SDL_Event e;
        if(SDL_PollEvent(&e)){
            // quiting the app
            if(e.type == SDL_EVENT_QUIT) break;
            // movement
            if(e.type == SDL_EVENT_KEY_DOWN){
                if(e.key.key == SDLK_UP) { right_pedal.y -= PEDAL_SPEED * delta_time; }
                if(e.key.key == SDLK_DOWN) { right_pedal.y += PEDAL_SPEED * delta_time; }
                if(e.key.key == SDLK_W) { left_pedal.y -= PEDAL_SPEED * delta_time; }
                if(e.key.key == SDLK_S) { left_pedal.y += PEDAL_SPEED * delta_time; }
            }
        }

        // Background color
        SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
        SDL_RenderClear(renderer);

        // Pedals
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &right_pedal);
        SDL_RenderFillRect(renderer, &left_pedal);

        // switch buffers
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}
