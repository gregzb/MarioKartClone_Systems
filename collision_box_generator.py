from PIL import Image
import sys
from collections import defaultdict

visited = set()

dirs = [(-1, 0), (1, 0), (0, -1), (0, 1)]

sections = defaultdict(list)

def neighbors(current_point, bounds):
  valid = []
  for dir in dirs:
    new_point = (current_point[0] + dir[0], current_point[1] + dir[1])
    if bounds.x <= new_point[0] <= bounds.x + bounds.w and bounds.y <= new_point[1] <= bounds.y + bounds.h:
      valid.append(new_point)
  return valid

def dfs(current_point, bounds, id):
  if current_point not in visited:
    visited.add(current_point)
    sections[id].append(current_point)
    for neighbor in neighbors(current_point, bounds):
      dfs(neighbor, bounds)

def main():
  if len(sys.argv) < 2:
    print(f"usage: python3.8 {__file__} [image file]")
    return

  image_file_name = sys.argv[1]
  im = Image.open(image_file_name)

  pixel_data = im.load()
  w, h = im.size()

  bounds = Rect(0, 0, w, h)

  id = 0

  for row in range(h):
    for col in range(w):
      if (row, col) not in visited:
        visited.add((row, col))
        new_rect = dfs((row, col), bounds, id)
        id += 1

main()

class Rect:
  def __init__(self, x, y, w, h):
    self.x = 0
    self.y = 0
    self.w = 0
    self.h = 0
