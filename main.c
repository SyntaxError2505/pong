#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <SDL3/SDL.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 60

#define SPAWN_BUFFER 100

#define PEDAL_WIDTH 10
#define PEDAL_HEIGHT 100

#define SECOND 1000000000.0
#define FRAME_TIME ((Uint64)(SECOND / FPS))

#define MAX_SPAWN_VERTICAL_VELOCITY 500.0
#define PEDAL_SPEED 500.0
#define MAX_INTERFERENCE 50.0

typedef struct {
    float x, y;
    int r;
    float dx, dy;
} Ball;

void draw_ball(SDL_Renderer *renderer, Ball ball)
{
    for (int dy = -ball.r; dy <= ball.r; dy++) {
        float half_width = sqrtf(
            (float)(ball.r * ball.r - dy * dy)
        );

        SDL_RenderLine(
            renderer,
            ball.x - half_width,
            ball.y + dy,
            ball.x + half_width,
            ball.y + dy
        );
    }
}

int randint(int min, int max){
    return min + (rand() % (max - min + 1));
}

double randdouble(double min, double max){
    return min + (max - min) * ((double)rand() / RAND_MAX);
}

SDL_Window* window;
SDL_Renderer* renderer;

Ball ball = {0, 0, 10, 0, 0};

void interfere(void){
    ball.dx += randdouble(-MAX_INTERFERENCE, MAX_INTERFERENCE);
    ball.dy += randdouble(-MAX_INTERFERENCE, MAX_INTERFERENCE);
}

void cap_fps(Uint64 *next_frame){
    *next_frame += FRAME_TIME;
    Uint64 now = SDL_GetTicksNS();
    if(*next_frame > now) SDL_DelayNS(*next_frame - now);
    else *next_frame = now;
}

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

void spawn_ball(void){
    // Randomly spawn the ball inside a buffered zone
    ball.x = randint(SPAWN_BUFFER, WIDTH - SPAWN_BUFFER);
    ball.y = randint(SPAWN_BUFFER, HEIGHT - SPAWN_BUFFER);
    ball.dy = randdouble(-MAX_SPAWN_VERTICAL_VELOCITY, MAX_SPAWN_VERTICAL_VELOCITY);
    ball.dx = randdouble(MAX_SPAWN_VERTICAL_VELOCITY / 2, MAX_SPAWN_VERTICAL_VELOCITY) * ((ball.x > WIDTH/2) ? -1 : 1);

    printf("Spawned Ball:\nx: %f\ny: %f\ndx: %f\ndy: %f\n", ball.x, ball.y, ball.dx, ball.dy);
}

int main(void){
    // SDL Init
    assert(SDL_Init(SDL_INIT_VIDEO));
    assert(SDL_CreateWindowAndRenderer("Pong", WIDTH, HEIGHT, SDL_WINDOW_VULKAN, &window, &renderer));

    // set seed to current nanosecond count
    srand(SDL_GetTicksNS());

    spawn_ball();

    Uint64 old_time = SDL_GetTicksNS();
    Uint64 next_frame = old_time;

    for(;;){
        // meassure delta time
        Uint64 new_time = SDL_GetTicksNS();
        Uint64 delta_time = new_time - old_time;
        old_time = new_time;

        // print debug info
        printf("Delta Time: %lu\nFramerate: %f\nBall x: %f\nBall y: %f\nBall dx: %f\nBall dy: %f\n", delta_time, 1000/((double)delta_time / 1000000.0), ball.x, ball.y, ball.dx, ball.dy);

        // poll events
        SDL_Event e;
        if(SDL_PollEvent(&e)){
            // quiting the app
            if(e.type == SDL_EVENT_QUIT) break;
        }

        // movement
        const bool *keys = SDL_GetKeyboardState(NULL);
        if(keys[SDL_SCANCODE_UP])   { right_pedal.y -= PEDAL_SPEED / SECOND * delta_time; }
        if(keys[SDL_SCANCODE_DOWN]) { right_pedal.y += PEDAL_SPEED / SECOND * delta_time; }
        if(keys[SDL_SCANCODE_W])    { left_pedal.y -= PEDAL_SPEED / SECOND * delta_time; }
        if(keys[SDL_SCANCODE_S])    { left_pedal.y += PEDAL_SPEED / SECOND * delta_time; }

        // ball movement
        ball.x += ball.dx / SECOND * delta_time;
        ball.y += ball.dy / SECOND * delta_time;

        // bounce off the top and bottom screen edges
        if(ball.y < ball.r){
            ball.y = ball.r;
            ball.dy = -ball.dy;
            interfere();
        }
        if(ball.y > HEIGHT - ball.r){
            ball.y = HEIGHT - ball.r;
            ball.dy = -ball.dy;
            interfere();
        }

        // bounce of paddles
        if(ball.x - ball.r < left_pedal.x + left_pedal.w && ball.dx < 0
            && ball.y >= left_pedal.y && ball.y <= left_pedal.y + PEDAL_HEIGHT){
            ball.x = left_pedal.x + left_pedal.w + ball.r;
            ball.dx = -ball.dx;
            interfere();
        }
        if(ball.x + ball.r > right_pedal.x && ball.dx > 0
            && ball.y >= right_pedal.y && ball.y <= right_pedal.y + PEDAL_HEIGHT){
            ball.x = right_pedal.x - ball.r;
            ball.dx = -ball.dx;
            interfere();
        }

        // ball left the screen on either side, respawn it
        if(ball.x < -ball.r || ball.x > WIDTH + ball.r){
            spawn_ball();
        }

        // Background color
        SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
        SDL_RenderClear(renderer);

        // Pedals
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &right_pedal);
        SDL_RenderFillRect(renderer, &left_pedal);

        // Draw ball
        draw_ball(renderer, ball);

        // switch buffers
        SDL_RenderPresent(renderer);

        // FPS Limit
        cap_fps(&next_frame);
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    return 0;
}
