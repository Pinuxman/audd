#pragma once
#include <SDL3/SDL.h>
#include <cstdint>

class IEffect {
public:
    virtual ~IEffect() = default;
    virtual void update(double dt) = 0;
    virtual void render(SDL_Renderer* renderer, uint32_t w, uint32_t h) = 0;
    virtual void handle_input(const bool* key_state) = 0;
    virtual const char* name() const = 0;
};
