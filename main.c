#include <stdio.h>
#include <stdint.h>
#include <memory.h>
#include <SDL.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize2.h"

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 272
#define IMAGE_FILE "image.jpg"

int main(int argc, char *argv[]) {
  SDL_Window * window = NULL;
  SDL_Renderer * renderer = NULL;
  SDL_Surface * buffer = NULL;
  SDL_Texture * image = NULL;
  char running = 0;
  SDL_Event event;
  uint8_t * image_data = NULL;
  uint8_t * new_image_data = NULL;
  int image_width, image_height, image_pixel_size;
  SDL_PixelFormatEnum format_sdl;
  stbir_pixel_layout format_stbir;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
    printf("Could not initialize SDL2: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow(
    "image",
    SDL_WINDOWPOS_UNDEFINED,
    SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN
  );
  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 2;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (renderer == NULL) {
    printf("Failed to initialize renderer: %s\n", SDL_GetError());
    return 3;
  }

  image_data = stbi_load(IMAGE_FILE, &image_width, &image_height, &image_pixel_size, STBI_default);
  switch (image_pixel_size)
  {
    case STBI_grey:
      printf("Grayscale images are not supported by SDL, reloading..\n");
      stbi_image_free(image_data);
      image_data = stbi_load(IMAGE_FILE, &image_width, &image_height, &image_pixel_size, STBI_rgb);
      format_sdl = SDL_PIXELFORMAT_RGB24;
      format_stbir = STBIR_RGB;
      break;
    case STBI_grey_alpha:
      printf("Grayscale images are not supported by SDL, reloading..\n");
      stbi_image_free(image_data);
      image_data = stbi_load(IMAGE_FILE, &image_width, &image_height, &image_pixel_size, STBI_rgb_alpha);
      format_sdl = SDL_PIXELFORMAT_RGBA32;
      format_stbir = STBIR_RGBA;
      break;
    case STBI_rgb:
      format_sdl = SDL_PIXELFORMAT_RGB24;
      format_stbir = STBIR_RGB;
      break;
    case STBI_rgb_alpha:
      format_sdl = SDL_PIXELFORMAT_RGBA32;
      format_stbir = STBIR_RGBA;
      break;
    default:
      printf("Could not determine format");
      return 4;
      break;
  }
  buffer = SDL_CreateRGBSurfaceWithFormat(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, format_sdl);
  stbir_resize_uint8_linear(image_data, image_width, image_height, 0, buffer->pixels, buffer->w, buffer->h, 0, format_stbir);
  stbi_image_free(image_data);
  image_data = NULL;

  image = SDL_CreateTextureFromSurface(renderer, buffer);
  SDL_FreeSurface(buffer);

  running = 1;
  while (running > 0) {
    SDL_RenderClear(renderer);
    if (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running = 0;
          break;
      }
    }
    SDL_RenderCopy(renderer, image, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  return 0;
}
