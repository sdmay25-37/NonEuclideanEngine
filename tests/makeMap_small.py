from PIL import Image
import json

img = Image.open('res/textures/Maze_Idea.png').convert('RGB')
width, height = img.size

tile_size = 3
tiles_x = width // tile_size
tiles_y = height // tile_size

data = []
tile_id_map = {}  # (tile_x, tile_y) -> tileId
tile_id = 0

print(tiles_x)
print(tiles_y)

# First pass — assign tileId and store positions
for tile_y in range(tiles_y):
    for tile_x in range(tiles_x):
        has_black = False
        for dy in range(tile_size):
            for dx in range(tile_size):
                px = tile_x * tile_size + dx
                py = tile_y * tile_size + dy
                pixel = img.getpixel((px, height - py - 1)
                                     )  # bottom-left origin
                if pixel == (0, 0, 0):
                    has_black = True
                    break
            if has_black:
                break

        sprite = "wall.jpg" if has_black else "floor2.jpg"
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
with open('maze_output.json', 'w') as f:
    json.dump(data, f, indent=4)
