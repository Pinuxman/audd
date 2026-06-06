#include "ui/ui_manager.h"
#include "audio/audio.h"
#include "effects/sine_effect.h"
#include "effects/log_scale.h"
#include <memory>
#include <vector>
#include <cstdio>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    if (init_mic() != 0) {
        fprintf(stderr, "Microphone init failed\n");
        return 1;
    }

    UIManager ui;
    if (!ui.init("Audio Visualizer", 800, 600)) {
        fprintf(stderr, "UI init failed\n");
        return 1;
    }

    // Пул эффектов
    std::vector<std::unique_ptr<IEffect>> effects;
    effects.push_back(std::make_unique<SineEffect>());
    effects.push_back(std::make_unique<LogarithmicScaleEffect>());

    size_t active_idx = 0;
    IEffect* current_effect = effects[active_idx].get();
    bool effect_enabled = true;

    bool clear_screen = true;

    while (ui.is_running()) {
        ui.poll_events();
        const bool* keys = ui.get_keyboard_state();

        // Управление
        if (keys[SDL_SCANCODE_1]) { active_idx = 0; current_effect = effects[active_idx].get(); effect_enabled = true; }
        if (keys[SDL_SCANCODE_2]) { active_idx = 1; current_effect = effects[active_idx].get(); effect_enabled = true; }
        if (keys[SDL_SCANCODE_SPACE]) effect_enabled = !effect_enabled;
        if (keys[SDL_SCANCODE_UP])    clear_screen = true;
        if (keys[SDL_SCANCODE_DOWN])  clear_screen = false;

        capture_audio(audio_buf);

        if (clear_screen) {
            ui.clear_screen();
        }

        SDL_SetRenderDrawColor(ui.renderer(), 0, 220, 120, 255);
        draw_audio(audio_buf, 100.0f, 3.0f, ui.height(), ui.width(), ui.renderer());

        if (effect_enabled && current_effect) {
            current_effect->handle_input(keys);
            current_effect->update(0.016);
            current_effect->render(ui.renderer(), ui.width(), ui.height());
        }

        ui.present();
    }

    return 0;
}
