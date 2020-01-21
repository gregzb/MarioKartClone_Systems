#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>

#include <SDL2/SDL.h>

#include "level.h"
#include "sdl_utils.h"

char *level_names[] = {"resources/levels/level1.lvl", NULL};

struct level level_init(SDL_Renderer *renderer, char *level_name)
{
    struct level temp;

    printf("Loading level: %s\n", level_name);

    int fd = open(level_name, O_RDONLY);
    if (fd < 0)
    {
        printf("Error: could not read file %s; %s\n", level_name, strerror(errno));
    }
    char data[4096];
    for (int i = 0; i < 4096; i++)
    {
        data[i] = 0;
    }
    int data_size = read(fd, &data, 4096);
    close(fd);

    int cursor = 0;

    if (renderer)
    {
        temp.level_image = load_image(renderer, data);
        SDL_QueryTexture(temp.level_image, NULL, NULL, &temp.size.x, &temp.size.y);
    }
    else
        temp.level_image = NULL;

    int file_name_length = strlen(data) + 1;
    cursor += file_name_length;

    int *coll_dat = (int *)((char *)data + cursor);
    temp.num_boxes = coll_dat[0];
    temp.num_start_boxes = coll_dat[1];
    temp.num_cp_1 = coll_dat[2];
    temp.num_cp_2 = coll_dat[3];

    coll_dat += 4;

    //printf("rects: %d\n", temp.num_boxes);

    temp.collision_boxes = calloc(temp.num_boxes, sizeof(SDL_Rect));
    for (int i = 0; i < temp.num_boxes; i++)
    {
        temp.collision_boxes[i].x = coll_dat[i * 4 + 0];
        temp.collision_boxes[i].y = coll_dat[i * 4 + 1];
        temp.collision_boxes[i].w = coll_dat[i * 4 + 2];
        temp.collision_boxes[i].h = coll_dat[i * 4 + 3];

        //printf("%d, %d, %d, %d\n", temp.collision_boxes[i].x, temp.collision_boxes[i].y, temp.collision_boxes[i].w, temp.collision_boxes[i].h);
    }

    coll_dat += temp.num_boxes * 4;

    //temp.collision_boxes = calloc(temp.num_boxes, sizeof(SDL_Rect));
    for (int i = 0; i < 4; i++)
    {
        temp.spawn_points[i].x = coll_dat[i * 2 + 0];
        temp.spawn_points[i].y = coll_dat[i * 2 + 1];

        //printf("%d, %d\n", temp.spawn_points[i].x, temp.spawn_points[i].y);
    }

    coll_dat += 4 * 2;

    temp.start_boxes = calloc(temp.num_start_boxes, sizeof(SDL_Rect));
    for (int i = 0; i < temp.num_start_boxes; i++)
    {
        temp.start_boxes[i].x = coll_dat[i * 4 + 0];
        temp.start_boxes[i].y = coll_dat[i * 4 + 1];
        temp.start_boxes[i].w = coll_dat[i * 4 + 2];
        temp.start_boxes[i].h = coll_dat[i * 4 + 3];

        //printf("%d, %d, %d, %d\n", temp.start_boxes[i].x, temp.start_boxes[i].y, temp.start_boxes[i].w, temp.start_boxes[i].h);
    }

    coll_dat += temp.num_start_boxes * 4;

    temp.cp_1 = calloc(temp.num_cp_1, sizeof(SDL_Rect));
    for (int i = 0; i < temp.num_cp_1; i++)
    {
        temp.cp_1[i].x = coll_dat[i * 4 + 0];
        temp.cp_1[i].y = coll_dat[i * 4 + 1];
        temp.cp_1[i].w = coll_dat[i * 4 + 2];
        temp.cp_1[i].h = coll_dat[i * 4 + 3];

        //printf("%d, %d, %d, %d\n", temp.cp_1[i].x, temp.cp_1[i].y, temp.cp_1[i].w, temp.cp_1[i].h);
    }

    coll_dat += temp.num_cp_1 * 4;

    temp.cp_2 = calloc(temp.num_cp_2, sizeof(SDL_Rect));
    for (int i = 0; i < temp.num_cp_2; i++)
    {
        temp.cp_2[i].x = coll_dat[i * 4 + 0];
        temp.cp_2[i].y = coll_dat[i * 4 + 1];
        temp.cp_2[i].w = coll_dat[i * 4 + 2];
        temp.cp_2[i].h = coll_dat[i * 4 + 3];

        //printf("%d, %d, %d, %d\n", temp.cp_2[i].x, temp.cp_2[i].y, temp.cp_2[i].w, temp.cp_2[i].h);
    }

    coll_dat += temp.num_cp_2 * 4;
    temp.scale_factor = coll_dat[0];

    //temp.scale_factor = 2;

    return temp;
}
