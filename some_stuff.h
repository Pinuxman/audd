#ifndef  SOME_STUFF_H
#define SOME_STUFF_H
#define PI 3.1415926
#include <stdint.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_audio.h>
#include "miniaudio.h"

#define MINIAUDIO_IMPLEMENTATION

typedef struct cords{
    uint32_t x;
    uint32_t y;
    float fx;
    float fy;
    int cnt;
}Cords;

void calc_sin(double* in, uint32_t n);
void convert(uint32_t* x, uint32_t* y, double fx, double fy, uint32_t w, uint32_t h);
int init_mic(void);
int capture_audio(float out[4096]);
static void data_callback(ma_device* device,
                          void* output,
                          const void* input,
                          ma_uint32 frameCount);
void convert_mic_data(Cords* cords, uint32_t w, uint32_t h);
void draw_audio(float *audio, float scale, float thick, uint32_t h, uint32_t w, SDL_Renderer* renderer);

#endif // zalupa
