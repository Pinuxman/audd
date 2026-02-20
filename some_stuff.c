#include "some_stuff.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include <stdio.h>
#include "miniaudio.h"


#define CAPTURE_SAMPLES 4096

static ma_device device;
static ma_mutex  audio_mutex;

static float     audio_buffer[CAPTURE_SAMPLES];
static ma_uint32 write_cursor = 0;

static void data_callback(ma_device* device,
                          void* output,
                          const void* input,
                          ma_uint32 frameCount)
{
    (void)device;
    (void)output;

    if (input == NULL) return;

    const float* in = (const float*)input;

    ma_mutex_lock(&audio_mutex);

    for (ma_uint32 i = 0; i < frameCount; i++) {
        audio_buffer[write_cursor++] = in[i];

        if (write_cursor >= CAPTURE_SAMPLES)
            write_cursor = 0;
    }

    ma_mutex_unlock(&audio_mutex);
}

void calc_sin(double* in, uint32_t n){
    in[0] = -PI;
    for (int i = 1; i < n; i++){
        in[i] = in[i - 1] + 2 * (PI / n);
    }
}

void convert(uint32_t* x, uint32_t* y, double fx, double fy, uint32_t w, uint32_t h){
    *x = ((fx + PI ) / (2 * PI)) * w;// [-pi, pi] -> [0, w]
    *y = (1 - (fy + 1) / 2) * h; // [-1, 1] -> [1, 0] -> [h, 0]
}

static SDL_AudioDeviceID mic = 0;
static SDL_AudioStream *mic_stream = NULL;

int init_mic(void)
{
    ma_device_config config;

    config = ma_device_config_init(ma_device_type_capture);
    config.capture.format   = ma_format_f32;
    config.capture.channels = 1;
    config.sampleRate       = 48000;
    config.dataCallback     = data_callback;

    if (ma_device_init(NULL, &config, &device) != MA_SUCCESS)
        return -1;

    if (ma_mutex_init(&audio_mutex) != MA_SUCCESS)
        return -2;

    if (ma_device_start(&device) != MA_SUCCESS)
        return -3;

    return 0;
}


int capture_audio(float out[CAPTURE_SAMPLES])
{
    ma_mutex_lock(&audio_mutex);

    ma_uint32 cursor = write_cursor;

    for (ma_uint32 i = 0; i < CAPTURE_SAMPLES; i++) {
        ma_uint32 index = (cursor + i) % CAPTURE_SAMPLES;
        out[i] = audio_buffer[index];
    }

    ma_mutex_unlock(&audio_mutex);

    return 0;
}



void convert_mic_data(Cords* cords, uint32_t w, uint32_t h){
    cords->x = cords->fx * w; // [0 , 1] -> [0, w]
    cords->y = (1 - (cords->fy + 1) / 2) * h;
}

void draw_audio(float *audio, float scale, float thick, uint32_t h, uint32_t w, SDL_Renderer *renderer){
    const int n = 4096;
    SDL_FPoint *points = SDL_malloc(sizeof(SDL_FPoint) * n);
        if (!points) return;

        float xstep = (float)w / (n - 1);   // full width usage
        float mid    = h * 0.5f;

        for (int i = 0; i < n; i++){
            float x = i * xstep;
            float y = mid + audio[i] * scale;

            points[i].x = x;
            points[i].y = y;
        }

        SDL_RenderLines(renderer, points, n);

        SDL_free(points);
}
