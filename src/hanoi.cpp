#include <SDL2/SDL.h>
#include <cstdio>
#include <fmt/format.h>

#include "HanoiState.hpp"
#include "defer.hpp"

void drawHanoiDisk(SDL_Renderer *renderer, unsigned int disk, size_t disk_count,
                   unsigned int rod_base_width, SDL_Rect rod_rect) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

    const float width_ratio = static_cast<float>(rod_base_width) / disk_count;
    const int disk_height = 10;

    SDL_Rect disk_rect = {};
    disk_rect.h = disk_height;
    disk_rect.w = width_ratio * disk;
    disk_rect.y = rod_rect.y + rod_rect.h - disk_height * disk;

    // rod center
    disk_rect.x = rod_rect.x + rod_rect.w / 2;

    // the leftmost point
    disk_rect.x -= rod_base_width / 2;

    // offset based on value
    disk_rect.x += (disk_count - disk) * width_ratio / 2;

    SDL_RenderFillRect(renderer, &disk_rect);
}

void drawHanoiRod(SDL_Renderer *renderer, const DiskRod &rod, size_t rod_idx,
                  size_t rod_count, unsigned int area_width,
                  unsigned int area_height) {
    const int rod_base_width = 50;
    const int rod_width = 10;
    const int rod_height = 100;

    const int awidth_divided = area_width / rod_count;
    const int left_base = rod_idx * awidth_divided;
    const int left_padding = awidth_divided / 2;
    const int left = left_base + left_padding - rod_width / 2;
    const int top = area_height - rod_height;

    SDL_Rect rod_rect = {left, top, rod_width, rod_height};

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rod_rect);

    const auto disks = rod.disks();


    for (size_t disk_idx = 0; disk_idx < disks.size(); disk_idx++) {
        const auto disk = disks[disk_idx];
        drawHanoiDisk(renderer, disk, rod.height(), rod_base_width, rod_rect);
    }
}

int main(int /*argc*/, char ** /*argv*/) {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        fmt::println(stderr, "failed to initialize SDL video: {}",
                     SDL_GetError());
        return -1;
    }
    auto defer_sdl_quit = Defer{[]() {
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        SDL_Quit();
    }};

    SDL_Window *window = SDL_CreateWindow("Hanoi", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    if (window == nullptr) {
        fmt::println(stderr, "failed to create SDL window: {}", SDL_GetError());
        return -1;
    }
    auto defer_sdl_window = Defer{[&window]() { SDL_DestroyWindow(window); }};

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        fmt::println(stderr, "failed to create SDL renderer: {}",
                     SDL_GetError());
        return -1;
    }
    auto defer_sdl_renderer =
        Defer{[&renderer]() { SDL_DestroyRenderer(renderer); }};

    HanoiState hanoi_state(3, 3);

    bool should_close = false;
    while (!should_close) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                should_close = true;
                break;
            default:
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        int drawable_width;
        int drawable_height;
        SDL_GetRendererOutputSize(renderer, &drawable_width, &drawable_height);

        for (size_t rod_idx = 0; rod_idx < hanoi_state.rodCount(); rod_idx++) {
            drawHanoiRod(renderer, hanoi_state.rods()[rod_idx], rod_idx,
                         hanoi_state.rodCount(), drawable_width,
                         drawable_height);
        }


        SDL_RenderPresent(renderer);
    }

    return 0;
}
