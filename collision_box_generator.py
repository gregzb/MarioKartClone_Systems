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

visited = set()

dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]

sections = defaultdict(list)

def neighbors(pixel_data, current_point, bounds):
  valid = []
  for dir in dirs:
    new_point = (current_point[0] + dir[0], current_point[1] + dir[1])
    #print(bounds.x + bounds.w, bounds.y + bounds.h)
    if bounds.x <= new_point[0] < bounds.x + bounds.w and bounds.y <= new_point[1] < bounds.y + bounds.h and pixel_data[current_point] == pixel_data[new_point]:
      valid.append(new_point)
  return valid

def dfs(pixel_data, current_point, bounds, id):
  my_stack = [current_point]
  while my_stack:
    point = my_stack.pop()
    if point not in visited:
      visited.add(point)
      sections[id].append(point)
      for neighbor in neighbors(pixel_data, point, bounds):
        #print(point, id)
        my_stack.append(neighbor)

        #print(point)
        #dfs(neighbor, bounds, id)
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
      if (x, y) not in visited and pixel_data[(x,y)] != (255, 255, 255):
        #print(row, col)
        dfs(pixel_data, (x, y), bounds, id)
        id += 1

def main():
  if len(sys.argv) < 3:
    print(f"usage: python3.8 {__file__} <folder/image> [image folder/file]")
    return

  if 'f' in sys.argv[1].lower():
    for subdir, dirs, files in os.walk(sys.argv[2]):
      for file in files:
        solve_image(os.path.join(subdir, file))
  else:
    solve_image(sys.argv[2])

  # image_file_name = sys.argv[1]
  # im = Image.open(image_file_name)
  #
  # pixel_data = im.load()
  # w, h = im.size
  #
  # bounds = Rect(0, 0, w, h)
  #
  # id = 0
  #
  # for x in range(w):
  #   for y in range(h):
  #     if (x, y) not in visited:
  #       #print(row, col)
  #       dfs(pixel_data, (x, y), bounds, id)
  #       id += 1

  print(f"Found {id} bounds")

  boxes = []

  for i in sections:
    point_list = sections[i]
    min_x, min_y = 999999999999, 999999999999
    max_x, max_y = -999999999999, -999999999999

    for point in point_list:
      min_x = min(min_x, point[0])
      min_y = min(min_y, point[1])
      max_x = max(max_x, point[0])
      max_y = max(max_y, point[1])

    boxes.append(Rect(min_x, min_y, max_x - min_x, max_y - min_y))

  print(boxes)

main()
