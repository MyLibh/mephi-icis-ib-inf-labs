import os

from PIL import Image
from pathlib import Path

DIR = "../assets/images/"
OUTPUT_DIRECTORY = DIR + "scaled/"
SCALE_FACTOR = 2

Path(OUTPUT_DIRECTORY).mkdir(parents=True, exist_ok=True)
for filename in os.listdir(DIR):
    if filename.endswith(".png"):
        with Image.open(DIR + filename) as img:
            w, h = img.size
            
            new_image = Image.new('RGB', (SCALE_FACTOR * w, SCALE_FACTOR * h))

            origin = img.load()
            scaled = new_image.load()
            for i in range(w):
                for j in range(h):
                    for k in range(SCALE_FACTOR):
                        for u in range(SCALE_FACTOR):
                            scaled[SCALE_FACTOR * i + k, SCALE_FACTOR * j + u] = origin[i, j]

            new_image.save(OUTPUT_DIRECTORY + filename)

