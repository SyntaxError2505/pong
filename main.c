#include <stdio.h>
#include <assert.h>
#include <SDL3/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;

int main(void){
    assert(SDL_Init(SDL_INIT_VIDEO));
    assert(SDL_CreateWindowAndRenderer("Pong", 800, 600, SDL_WINDOW_VULKAN, &window, &renderer));

    for(;;){
        SDL_Event e;
        SDL_PollEvent(&e);
        if(e.type == SDL_EVENT_QUIT) break;

        SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}
