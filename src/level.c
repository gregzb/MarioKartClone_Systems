#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "sdl_utils.h"

char *level_names[] = {"resources/levels/testlevel.lvl", "resources/levels/testlevel2.lvl", NULL};

struct level level_init(SDL_Renderer* renderer, char* level_name) {
  struct level temp;

  int fd = open(level_name, O_RDONLY);
  if(fd < 0)
  {
    printf("Error: could not read file %s; %s\n", level_name, strerror(errno));
  }
  char data[4096];
  int data_size = read(fd, &data, 4096);
  close(fd);

  if (renderer)
  {
    temp.level_image = load_image(renderer, data);
    SDL_QueryTexture(temp.level_image, NULL, NULL, &temp.size.x, &temp.size.y);
  }
  else temp.level_image = NULL;

  int file_name_length = strlen(data);

  int* coll_dat = strchr(data, 0) + 1;
  int coll_bytes = data_size - (file_name_length + 1);
  temp.num_boxes = coll_bytes / 16;

  //printf("rects: %d\n", temp.num_boxes);

  temp.collision_boxes = calloc(temp.num_boxes, sizeof(SDL_Rect));

  for (int i = 0; i < temp.num_boxes; i++) {
    SDL_Rect coll_box;
    coll_box.x = coll_dat[i*4 + 0];
    coll_box.y = coll_dat[i*4 + 1];
    coll_box.w = coll_dat[i*4 + 2];
    coll_box.h = coll_dat[i*4 + 3];
    temp.collision_boxes[i] = coll_box;

    //printf("%d, %d, %d, %d\n", coll_box.x, coll_box.y, coll_box.w, coll_box.h);
  }

  temp.scale_factor = 3;

  return temp;
}
