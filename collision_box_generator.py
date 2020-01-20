from PIL import Image
import sys
from collections import defaultdict
import os


class Rect:
    def __init__(self, x, y, w, h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def __repr__(self):
        return '{' + f"{self.x}, {self.y}, {self.w}, {self.h}" + '}'

    def get_bytes(self):
        return self.x.to_bytes(4, byteorder='little') + self.y.to_bytes(4, byteorder='little') + self.w.to_bytes(4, byteorder='little') + self.h.to_bytes(4, byteorder='little')

class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __repr__(self):
        return '{' + f"{self.x}, {self.y}" + '}'

    def get_bytes(self):
        return self.x.to_bytes(4, byteorder='little') + self.y.to_bytes(4, byteorder='little')

visited = set()

dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]

#sections = defaultdict(list)

total_data = defaultdict(lambda: defaultdict(list))


def neighbors(pixel_data, current_point, bounds):
    valid = []
    for dir in dirs:
        new_point = (current_point[0] + dir[0], current_point[1] + dir[1])
        #print(bounds.x + bounds.w, bounds.y + bounds.h)
        if bounds.x <= new_point[0] < bounds.x + bounds.w and bounds.y <= new_point[1] < bounds.y + bounds.h and pixel_data[current_point] == pixel_data[new_point]:
            valid.append(new_point)
    return valid


def dfs(pixel_data, current_point, bounds, id, def_dict):
    my_stack = [current_point]
    while my_stack:
        point = my_stack.pop()
        if point not in visited:
            visited.add(point)
            def_dict[id].append(point)
            for neighbor in neighbors(pixel_data, point, bounds):
                my_stack.append(neighbor)
id = 0


def solve_image(image_file_name):
    global visited
    global id
    visited = set()
    im = Image.open(image_file_name)

    pixel_data = im.load()
    w, h = im.size

    bounds = Rect(0, 0, w, h)

    for x in range(w):
        for y in range(h):
            if (x, y) not in visited:
                dfs(pixel_data, (x, y), bounds, id,
                    total_data[pixel_data[(x, y)]])
                id += 1


definitions = {
    'ignore': [(0, 0, 0, 0), (255, 255, 255, 255)],
    'spawnpoints': [(0, 0, 255, 255)],
    'start_fin': [(0, 50, 255, 255)],
    'point1': [(0, 100, 255, 255)],
    'point2': [(0, 150, 255, 255)]
}


def sections_to_rect_list(sections):
    temp = []
    for i in sections:
        point_list = sections[i]
        # print(point_list[0])
        min_x, min_y = 999999999999, 999999999999
        max_x, max_y = -999999999999, -999999999999

        for point in point_list:
            min_x = min(min_x, point[0])
            min_y = min(min_y, point[1])
            max_x = max(max_x, point[0])
            max_y = max(max_y, point[1])

        temp.append(Rect(min_x, min_y, max_x - min_x, max_y - min_y))
    return temp


def main():
    if len(sys.argv) < 6:
        print(
            f"usage: python3.8 {__file__} <folder/image> [image folder/file] [picfile] [musicfile] [levelfile]")
        return

    if 'f' in sys.argv[1].lower():
        for subdir, dirs, files in os.walk(sys.argv[2]):
            for file in files:
                solve_image(os.path.join(subdir, file))
    else:
        solve_image(sys.argv[2])

    print(f"Found {id} bounds")

    boxes = []
    spawn_points = []
    start_fin = []
    point1 = []
    point2 = []

    for key in total_data:
        if key in definitions['ignore']:
            pass
        elif key in definitions['spawnpoints']:
            spawn_points.extend(sections_to_rect_list(total_data[key]))
        elif key in definitions['start_fin']:
            start_fin.extend(sections_to_rect_list(total_data[key]))
        elif key in definitions['point1']:
            point1.extend(sections_to_rect_list(total_data[key]))
        elif key in definitions['point2']:
            point2.extend(sections_to_rect_list(total_data[key]))
        else:
            boxes.extend(sections_to_rect_list(total_data[key]))

    spawn_points = [Point(r.x, r.y) for r in spawn_points]

    #pic file resources/images/test4.bmp
    #level file resources/levels/testlevel.lvl

    with open(sys.argv[5], 'wb') as f:
        f.write(str.encode(sys.argv[3]))
        f.write(bytes([0]))

        f.write(str.encode(sys.argv[4]))
        f.write(bytes([0]))

        f.write(len(boxes).to_bytes(4, byteorder='little'))
        #f.write(len(spawn_points).to_bytes(4, byteorder='little'))
        f.write(len(start_fin).to_bytes(4, byteorder='little'))
        f.write(len(point1).to_bytes(4, byteorder='little'))
        f.write(len(point2).to_bytes(4, byteorder='little'))

        for box in boxes:
            f.write(box.get_bytes())
        for box in spawn_points:
            f.write(box.get_bytes())
        for box in start_fin:
            f.write(box.get_bytes())
        for box in point1:
            f.write(box.get_bytes())
        for box in point2:
            f.write(box.get_bytes())

        f.write(bytes([2]))
        
        print(len(boxes))
        print(boxes)


main()
