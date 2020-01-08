from PIL import Image
import sys

def main():
  if len(sys.argv) < 2:
    print(f"usage: python3.8 {__file__} [image file]")
    return

  image_file_name = sys.argv[1]
  im = Image.open(image_file_name)

  pixel_data = im.load()

main()
