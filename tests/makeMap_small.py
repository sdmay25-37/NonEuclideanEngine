from PIL import Image
import json

# Load the reference image to get wall-indicating colors
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

tile_size = 3
tiles_x = width // tile_size
tiles_y = height // tile_size

data = []
tile_id_map = {}  # (tile_x, tile_y) -> tileId
tile_id = 0

# First pass — assign tileId and store positions
for tile_y in range(tiles_y):
    for tile_x in range(tiles_x):
        has_wall_color = False
        for dy in range(tile_size):
            for dx in range(tile_size):
                px = tile_x * tile_size + dx
                py = tile_y * tile_size + dy
                pixel = img.getpixel((px, height - py - 1)
                                     )  # bottom-left origin

                if pixel in wall_colors:
                    has_wall_color = True
                    break
            if has_wall_color:
                break

        sprite = "wall.jpg" if has_wall_color else "floor2.jpg"
        tile_id_map[(tile_x, tile_y)] = tile_id
        data.append({
            "tileId": tile_id,
            "worldPosition": [tile_x, tile_y],
            "sprite": sprite
        })
        tile_id += 1

# Second pass — set neighbors
for tile in data:
    x, y = tile["worldPosition"]

    tile["leftTileId"] = tile_id_map.get((x - 1, y), -1)
    tile["rightTileId"] = tile_id_map.get((x + 1, y), -1)
    tile["upTileId"] = tile_id_map.get((x, y + 1), -1)
    tile["downTileId"] = tile_id_map.get((x, y - 1), -1)

# Output JSON
with open('forest_output.json', 'w') as f:
    json.dump(data, f, indent=4)
