#include "some_stuff.h"
#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include <stdio.h>
float samples[4096];

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
    int count = 0;
    SDL_AudioDeviceID *devs = SDL_GetAudioRecordingDevices(&count);

    for (int i = 0; i < count; i++) {
        const char *name = SDL_GetAudioDeviceName(devs[i]);
        printf("[%d] %s\n", i, name);
    }
    SDL_AudioSpec spec;
    SDL_ResumeAudioStreamDevice(mic_stream);
    spec.format   = SDL_AUDIO_F32; // float samples
    spec.channels = 1;                // mono
    spec.freq     = 44100;

    mic_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_RECORDING,
        &spec,
        NULL,   // no callback: we’ll read manually
        NULL
    );

    if (!mic_stream) {
        SDL_Log("Failed to open mic stream: %s", SDL_GetError());
        return 0;
    }

    // Start capturing
    SDL_ResumeAudioStreamDevice(mic_stream);
    return 1;
}


int capture_audio(float out[4096])
{
    int bytes = sizeof(float) * 4096;

    if (SDL_GetAudioStreamAvailable(mic_stream) < bytes)
        return 0;

    SDL_GetAudioStreamData(mic_stream, out, bytes);
    return 1;
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

        // Set color once (instead of per-vertex)
        //SDL_SetRenderDrawColor(renderer, 220, 240, 255, 255);  // cyan-ish, fully opaque

        // One call → draws 4095 connected line segments
        SDL_RenderLines(renderer, points, n);

        SDL_free(points);
}
