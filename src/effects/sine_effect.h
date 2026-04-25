#pragma once
#include "effect.h"

class SineEffect : public IEffect {
public:
    SineEffect();
    void update(double dt) override;
    void render(SDL_Renderer* renderer, uint32_t w, uint32_t h) override;
    void handle_input(const bool* key_state) override;
    const char* name() const override { return "Sine Wave"; }

private:
    double fase  = 0.0;
    double shift = 0.0;
    int    cnt   = 1;
    uint32_t color = 0;

    void calc_sin(double* in, uint32_t n);
    void convert(uint32_t* x, uint32_t* y, double fx, double fy, uint32_t w, uint32_t h);
};
