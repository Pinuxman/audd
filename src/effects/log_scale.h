#pragma once
#include "SDL3/SDL_render.h"
#include "effect.h"
#include <cmath>
#include <SDL3/SDL.h>
#include "audio/audio.h"

void draw_column(const float* audio_buf, float log_scale, float intensity, uint32_t h, uint32_t w, SDL_Renderer* renderer) {
    const int num_samples = w;
    int height = h;
        for (int i = 0; i < num_samples; ++i) {
            int y = static_cast<int>(h * (1.0f - audio_buf[i] * log_scale * intensity));
            int green = static_cast<int>(255 * (1 - y / h));
            int red = static_cast<int>(255 * y / h);
            SDL_SetRenderDrawColor(renderer, static_cast<Uint8>(red), static_cast<Uint8>(green), 0, 255); // Set color from green to red based on y
            SDL_FRect rect = {0, static_cast<float>(y), 20., static_cast<float>(h - y)}; // Draw column as a rectangle
            SDL_RenderFillRect(renderer, &rect);
        }
    }


class LogarithmicScaleEffect : public IEffect {
public:
    void update(double dt) override {}
    void render(SDL_Renderer* renderer, uint32_t w, uint32_t h) override;
    void handle_input(const bool* key_state) override {}
    const char* name() const override { return "LogarithmicScale"; }

private:
    float calculate_log_scale(float value);
};

void LogarithmicScaleEffect::render(SDL_Renderer* renderer, uint32_t w, uint32_t h) {
    float log_scale = calculate_log_scale(0.5f); // Example value
    draw_column(audio_buf, log_scale, 1.0f, h, w, renderer);
}

float LogarithmicScaleEffect::calculate_log_scale(float value) {
    return std::log(value + 1.0f) / std::log(2.0f);
}
