#include "audio.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

static ma_device device;
static ma_mutex  audio_mutex;
static float     audio_buffer[CAPTURE_SAMPLES];
static ma_uint32 write_cursor = 0;

static void data_callback(ma_device* dev, void* output, const void* input, ma_uint32 frameCount) {
    (void)dev; (void)output;
    if (!input) return;
    const float* in = (const float*)input;
    ma_mutex_lock(&audio_mutex);
    for (ma_uint32 i = 0; i < frameCount; i++) {
        audio_buffer[write_cursor++] = in[i];
        if (write_cursor >= CAPTURE_SAMPLES) write_cursor = 0;
    }
    ma_mutex_unlock(&audio_mutex);
}

int init_mic() {
    ma_device_config config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32;
    config.capture.channels = 1;
    config.sampleRate       = 48000;
    config.dataCallback     = data_callback;
    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS) return -1;
    if (ma_mutex_init(&audio_mutex) != MA_SUCCESS) return -2;
    if (ma_device_start(&device) != MA_SUCCESS) return -3;
    return 0;
}

int capture_audio(float out[CAPTURE_SAMPLES]) {
    ma_mutex_lock(&audio_mutex);
    ma_uint32 cursor = write_cursor;
    for (ma_uint32 i = 0; i < CAPTURE_SAMPLES; i++) {
        out[i] = audio_buffer[(cursor + i) % CAPTURE_SAMPLES];
    }
    ma_mutex_unlock(&audio_mutex);
    return 0;
}

void draw_audio(float* audio, float scale, float thick, uint32_t h, uint32_t w, SDL_Renderer* renderer) {
    (void)thick;
    const int n = CAPTURE_SAMPLES;
    SDL_FPoint* points = (SDL_FPoint*)SDL_malloc(sizeof(SDL_FPoint) * n);
    if (!points) return;
    float xstep = (float)w / (n - 1);
    float mid   = h * 0.5f;
    for (int i = 0; i < n; i++) {
        points[i].x = i * xstep;
        points[i].y = mid + audio[i] * scale;
    }
    SDL_RenderLines(renderer, points, n);
    SDL_free(points);
}
