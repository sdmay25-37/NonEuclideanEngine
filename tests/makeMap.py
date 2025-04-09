from PIL import Image
import json

img = Image.open('res/textures/Maze_Idea.png').convert('RGB')
width, height = img.size

data = []
tile_id_map = {}  # (x, y) -> tileId
tile_id = 0

# First pass — assign tileId and store positions
for y in range(height):
    for x in range(width):
        pixel = img.getpixel((x, height - y - 1))  # bottom-left origin
        sprite = "wall.jpg" if pixel == (0, 0, 0) else "floor2.jpg"

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
with open('maze_output.json', 'w') as f:
    json.dump(data, f, indent=4)
