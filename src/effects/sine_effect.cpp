#include "sine_effect.h"
#include <cmath>
#include <SDL3/SDL_scancode.h>

#define PI 3.1415926

SineEffect::SineEffect() {}

void SineEffect::calc_sin(double* in, uint32_t n) {
    in[0] = -PI;
    for (uint32_t i = 1; i < n; i++) {
        in[i] = in[i - 1] + 2.0 * (PI / n);
    }
}

void SineEffect::convert(uint32_t* x, uint32_t* y, double fx, double fy, uint32_t w, uint32_t h) {
    *x = (uint32_t)(((fx + PI) / (2.0 * PI)) * w);
    *y = (uint32_t)((1.0 - (fy + 1.0) / 2.0) * h);
}

void SineEffect::handle_input(const bool* key_state) {
    if (key_state[SDL_SCANCODE_LEFT])  fase += 0.001;
    if (key_state[SDL_SCANCODE_RIGHT]) fase -= 0.001;
    if (key_state[SDL_SCANCODE_UP])    { cnt *= 3; if (cnt > 100) cnt = 1; }
    if (key_state[SDL_SCANCODE_DOWN])  { cnt /= 3; if (cnt < 1) cnt = 1; }
}

void SineEffect::update(double dt) {
    (void)dt;
    shift += fase;
}

void SineEffect::render(SDL_Renderer* renderer, uint32_t w, uint32_t h) {
    color += (cnt > 0 ? cnt : 1);

    // Простые множители гарантируют равномерное переливание по всему спектру
    uint8_t r = (color * 7)  % 255;
    uint8_t g = (color * 13) % 255;
    uint8_t b = (color * 17) % 255;

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    double in[200];
    calc_sin(in, 200);

    for (int i = 0; i < 200; i++) {
        uint32_t x, y;
        convert(&x, &y, in[i], std::sin(cnt % 10 * in[i] + shift), w, h);
        SDL_RenderLine(renderer, x, y, x, y);
    }
}
