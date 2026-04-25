#pragma once
#include <SDL3/SDL.h>
#include <cstdint>
#include "miniaudio.h"
#define CAPTURE_SAMPLES 4096

int  init_mic();
int  capture_audio(float out[CAPTURE_SAMPLES]);
void draw_audio(float* audio, float scale, float thick, uint32_t h, uint32_t w, SDL_Renderer* renderer);
