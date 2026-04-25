#pragma once
#include <SDL3/SDL.h>
#include <cstdint>

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool init(const char* title, uint32_t w, uint32_t h);
    void poll_events();
    bool is_running() const { return !m_quit; }

    // SDL3 вернет const bool*
    const bool* get_keyboard_state() const;

    SDL_Renderer* renderer() const { return m_renderer; }
    uint32_t width() const { return m_width; }
    uint32_t height() const { return m_height; }

    void clear_screen();
    void present();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    uint32_t      m_width    = 0;
    uint32_t      m_height   = 0;
    bool          m_quit     = false;
};
