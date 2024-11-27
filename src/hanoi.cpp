#include <SDL2/SDL.h>
#include <cstdio>
#include <fmt/format.h>

#include "HanoiState.hpp"
#include "defer.hpp"

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

        int window_width;
        int window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);

        for (size_t rod_idx = 0; rod_idx < hanoi_state.rodCount(); rod_idx++) {
            const int rod_base_width = 50;
            const int rod_width = 10;
            const int rod_height = 100;

            const int wwidth_divided = (window_width / hanoi_state.rodCount());
            const int left_base = rod_idx * wwidth_divided;
            const int left_padding = wwidth_divided / 2;
            const int left = left_base + left_padding - rod_width / 2;
            const int rod_top = window_height - rod_height;

            SDL_Rect rod_rect = {left, rod_top, rod_width, rod_height};

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &rod_rect);

            const DiskRod &rod = hanoi_state.rods()[rod_idx];
            const auto disks = rod.disks();

            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

            for (size_t disk_idx = 0; disk_idx < disks.size(); disk_idx++) {
                const auto disk = disks[disk_idx];
                const float width_ratio =
                    static_cast<float>(rod_base_width) / rod.height();
                const int disk_height = 10;
                const size_t reverse_idx = rod.height() - disk_idx - 1;

                SDL_Rect disk_rect = {};
                disk_rect.h = disk_height;
                disk_rect.w = width_ratio * disk;
                disk_rect.y =
                    rod_top + rod_height - disk_height * (reverse_idx + 1);
                disk_rect.x = left + (reverse_idx * width_ratio -
                                      rod_base_width + rod_width) /
                                         2.0f;


                SDL_RenderFillRect(renderer, &disk_rect);
            }
        }


        SDL_RenderPresent(renderer);
    }

    return 0;
}
