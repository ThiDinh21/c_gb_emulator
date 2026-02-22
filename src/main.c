#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "mmu.h"
#include "cpu.h"
#include "ppu.h"
#include "timer.h"
#include "logging.h"

#define SCALE       3
#define SCREEN_W    (160 * SCALE)
#define SCREEN_H    (144 * SCALE)

// Classic Game Boy green palette (shade 0=lightest, 3=darkest)
static const uint8_t GB_PALETTE[4][3] = {
    {0x9B, 0xBC, 0x0F}, // 0: white
    {0x8B, 0xAC, 0x0F}, // 1: light gray
    {0x30, 0x62, 0x30}, // 2: dark gray
    {0x0F, 0x38, 0x0F}, // 3: black
};

int main(void)
{
    CPU *cpu = init_cpu();

    // SDL init
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "GB Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN
    );
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    SDL_RenderSetLogicalSize(renderer, 160, 144);

    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        160, 144
    );
    if (!texture)
    {
        fprintf(stderr, "SDL_CreateTexture error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    uint8_t rgb[160 * 144 * 3]; // scratch buffer for texture upload
    int running = 1;

    while (running)
    {
        // Poll events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        uint8_t cycles = cpu_step(cpu);
        ppu_step(cpu, cycles);
        timer_handling(cpu, cycles);

        // Present frame on VBlank
        if (cpu->ppu->frame_ready)
        {
            cpu->ppu->frame_ready = 0;

            // Convert shade 0-3 framebuffer to RGB
            for (int i = 0; i < 160 * 144; i++)
            {
                uint8_t shade = cpu->ppu->framebuffer[i];
                rgb[i * 3 + 0] = GB_PALETTE[shade][0];
                rgb[i * 3 + 1] = GB_PALETTE[shade][1];
                rgb[i * 3 + 2] = GB_PALETTE[shade][2];
            }

            SDL_UpdateTexture(texture, NULL, rgb, 160 * 3);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    clean_up_cpu(cpu);
    return 0;
}
