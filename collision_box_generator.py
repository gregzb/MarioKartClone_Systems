from PIL import Image
import sys

visited = set()

dirs = []

def neighbors(current_point, bounds):
  pass

def dfs(current_point):
  if current_point not in visited:
    visited.add(current_point)

def main():
  if len(sys.argv) < 2:
    print(f"usage: python3.8 {__file__} [image file]")
    return

  image_file_name = sys.argv[1]
  im = Image.open(image_file_name)

  pixel_data = im.load()
  w, h = im.size()

  for row in range(h):
    for col in range(w):
      if (row, col) not in visited:
        visited.add((row, col))
        new_rect = dfs((row, col))

main()

class Rect:
  def __init__(self, x1, y1, x2, y2):
    self.x1 = x1
    self.y1 = y1
    self.x2 = x2
    self.y2 = y2
