#include "lap.h"
#include "sdl_utils.h"

//Oops its disgusting
char lap_logic(struct kart *kart, struct level *current_level)
{
	char temp = 0;
	struct v2_rect kart_rect = {kart->position.x, kart->position.y, kart->size.x, kart->size.y};
	kart_rect.x -= kart_rect.w / 2;
	kart_rect.y -= kart_rect.h / 2;

	struct v2_rect intersection = {0};

	char progress[3];
	memcpy(progress, kart->progress, 3);

	char intersecting = 0;
	for (int i = 0; i < current_level->num_start_boxes; i++)
	{
		struct v2_rect other_rect = v2_rect_from_SDL_Rect(current_level->start_boxes[i]);

		intersecting = v2_rect_intersection(kart_rect, other_rect, &intersection);
		if (intersecting)
		{
			if (progress[0] && progress[1] && progress[2])
			{
				temp = 1;
			}
			kart->progress[0] = 1;
			kart->progress[1] = 0;
			kart->progress[2] = 0;
			break;
		}
	}

	intersecting = 0;

	for (int i = 0; i < current_level->num_cp_1; i++)
	{
		struct v2_rect other_rect = v2_rect_from_SDL_Rect(current_level->cp_1[i]);

		intersecting = v2_rect_intersection(kart_rect, other_rect, &intersection);
		if (intersecting)
		{
			if (progress[0])
			{
				kart->progress[0] = 1;
				kart->progress[1] = 1;
				kart->progress[2] = 0;
			}
			else
			{
				kart->progress[0] = 0;
				kart->progress[1] = 0;
				kart->progress[2] = 0;
			}
			break;
		}
	}

	intersecting = 0;

	for (int i = 0; i < current_level->num_cp_2; i++)
	{
		struct v2_rect other_rect = v2_rect_from_SDL_Rect(current_level->cp_2[i]);

		intersecting = v2_rect_intersection(kart_rect, other_rect, &intersection);
		if (intersecting)
		{
			if (progress[0] && progress[1])
			{
				kart->progress[0] = 1;
				kart->progress[1] = 1;
				kart->progress[2] = 1;
			}
			else
			{
				kart->progress[0] = 0;
				kart->progress[1] = 0;
				kart->progress[2] = 0;
			}
			break;
		}
	}

	return temp;
}