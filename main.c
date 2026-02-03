#include "SDL3/SDL_events.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_oldnames.h"
#include "SDL3/SDL_pixels.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_scancode.h"
#include <SDL3/SDL.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include "SDL3/SDL_video.h"
#include "some_stuff.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    const int width = 800;
    const int height = 600;
    SDL_Window *window = SDL_CreateWindow(
        "Draw Sin",
        width, height,
        0
    );

    init_mic();
    float out[4096];
    Cords cords;
    cords.fx = 0;
    cords.cnt = 0;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    int run_flag = 1;
    double fase = 0;
    double shift = 0;
    int clear = 0;
    uint32_t  color = 0;
    int cnt = 1;
    float max = 0;

    while(run_flag){
        SDL_SetRenderDrawColor(renderer, (color * cnt) % 255,
                              (color + cnt) % 255, (color - cnt) % 255, 255);
        SDL_Event event;
        capture_audio(out);
        draw_audio(out, height*0.4f, 2.0f, height, width, renderer);
        convert_mic_data(&cords, width, height);

        while(SDL_PollEvent(&event)){
            if (event.type == SDL_EVENT_QUIT) run_flag = 0;
            if (event.type == SDL_EVENT_KEY_DOWN){
                if (event.key.scancode == SDL_SCANCODE_LEFT){ fase += 0.001; cnt *= 3;}
                if (event.key.scancode == SDL_SCANCODE_RIGHT){ fase -= 0.001; cnt *= 3;}
                if (event.key.scancode == SDL_SCANCODE_UP) clear = 1;
                if (event.key.scancode == SDL_SCANCODE_DOWN) clear = 0;
        }
        }
        shift += fase;
        double in[200];
        calc_sin(in,200);
        uint32_t x, y;
        color++;
        for (int i = 0; i < 200; i++){
            convert(&x, &y, in[i], sin(cnt % 10 * in[i] + shift), width ,height);
            SDL_RenderLine(renderer, x, y, x, y);
        }
        SDL_RenderPresent(renderer);
        if(clear){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
