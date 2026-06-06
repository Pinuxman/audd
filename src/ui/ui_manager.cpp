#include "ui_manager.h"

UIManager::UIManager() {}
UIManager::~UIManager() {
    if (m_renderer) SDL_DestroyRenderer(m_renderer);
    if (m_window)   SDL_DestroyWindow(m_window);
}

bool UIManager::init(const char* title, uint32_t w, uint32_t h) {
    m_window = SDL_CreateWindow(title, w, h, SDL_WINDOW_RESIZABLE);
    if (!m_window) return false;
    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) return false;
    m_width = w;
    m_height = h;
    return true;
}

void UIManager::poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_WINDOW_RESIZED) {
            m_width = event.window.data1;
            m_height = event.window.data2;
        }
        if (event.type == SDL_EVENT_QUIT) m_quit = true;
        if (event.type == SDL_EVENT_KEY_DOWN) {
            if (event.key.scancode == SDL_SCANCODE_ESCAPE) m_quit = true;
        }
    }
}

const bool* UIManager::get_keyboard_state() const {
    return SDL_GetKeyboardState(nullptr);
}

void UIManager::clear_screen() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);
}

void UIManager::present() {
    SDL_RenderPresent(m_renderer);
}
