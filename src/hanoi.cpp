#include <SDL2/SDL.h>
#include <cstdio>
#include <fmt/format.h>

int main(int /*argc*/, char ** /*argv*/) {
  if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
    fmt::println(stderr, "failed to initialize SDL video: {}", SDL_GetError());
    return -1;
  }

  SDL_Window *window = SDL_CreateWindow("Hanoi", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
  if (window == nullptr) {
    fmt::println(stderr, "failed to create SDL window: {}", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
  if (renderer == nullptr) {
    fmt::println(stderr, "failed to create SDL renderer: {}", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return -1;
  }

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
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
