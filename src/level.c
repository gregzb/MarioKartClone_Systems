#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "sdl_utils.h"

struct level level_init(SDL_Renderer* renderer, char* level_name) {
  struct level temp;

  int fd = open(level_name, O_RDONLY);
  char data[4096];
  int data_size = read(fd, &data, 4096);

  temp.level_image = load_image(renderer, data);

  int file_name_length = strlen(data);

  int* coll_dat = (int*) strchr(data, 0) + 1;
  int coll_bytes = data_size - (file_name_length + 1);
  int num_rects = coll_bytes / 16;

  printf("rects: %d\n", num_rects);

  temp.collision_boxes = calloc(num_rects, sizeof(SDL_Rect));

  for (int i = 0; i < num_rects; i++) {
    SDL_Rect coll_box;
    coll_box.x = coll_dat[i*4 + 0];
    coll_box.y = coll_dat[i*4 + 1];
    coll_box.w = coll_dat[i*4 + 2];
    coll_box.h = coll_dat[i*4 + 3];
    temp.collision_boxes[i] = coll_box;
  }

  return temp;
}
