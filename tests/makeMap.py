from PIL import Image
import json
import math


def color_distance(c1, c2):
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(c1, c2)))


def is_close_to_wall_color(pixel, wall_colors, threshold=30):
    for wall_color in wall_colors:
        if color_distance(pixel, wall_color) < threshold:
            return True
    return False


ref_img = Image.open('res/textures/wall_mask.jpg').convert('RGB')
ref_width, ref_height = ref_img.size
wall_colors = set()

for y in range(ref_height):
    for x in range(ref_width):
        color = ref_img.getpixel((x, y))
        wall_colors.add(color)

# Load the map image
img = Image.open('res/textures/Test_Forest_2_test.jpg').convert('RGB')
width, height = img.size

data = []
tile_id_map = {}  # (x, y) -> tileId
tile_id = 0

print(height)
# First pass — assign tileId and store positions
for y in range(height):
    print(y)
    for x in range(width):
        pixel = img.getpixel((x, height - y - 1))

        sprite = "wall.jpg" if is_close_to_wall_color(
            pixel, wall_colors, threshold=30) else "floor2.jpg"

        tile_id_map[(x, y)] = tile_id
        data.append({
            "tileId": tile_id,
            "worldPosition": [x, y],
            "sprite": sprite
        })
        tile_id += 1

# Second pass — set neighbors
for tile in data:
    x, y = tile["worldPosition"]

    # Look in 4 directions
    left = (x - 1, y)
    right = (x + 1, y)
    up = (x, y + 1)
    down = (x, y - 1)

    tile["leftTileId"] = tile_id_map.get(left, -1)
    tile["rightTileId"] = tile_id_map.get(right, -1)
    tile["upTileId"] = tile_id_map.get(up, -1)
    tile["downTileId"] = tile_id_map.get(down, -1)

# Output JSON
with open('forest_output_3.json', 'w') as f:
    json.dump(data, f, indent=4)
