from collections import defaultdict
from PIL import Image
import json
import math


def color_distance(c1, c2):
    return math.sqrt(sum((a - b) ** 2 for a, b in zip(c1, c2)))


def is_close_to_wall_color(pixel, wall_colors, threshold=30):
    return any(color_distance(pixel, wall_color) < threshold for wall_color in wall_colors)


DIRECTIONS = {
    "up": (0, 1),
    "down": (0, -1),
    "left": (-1, 0),
    "right": (1, 0),
}

OPPOSITE = {
    "up": "down",
    "down": "up",
    "left": "right",
    "right": "left",
}


def generate_tile(x, y, tile_id_map, data, img, wall_colors, tile_id):
    pos = (x, y)
    if pos in tile_id_map:
        return tile_id_map[pos]

    pixel = img.getpixel(pos)
    sprite = "wall.jpg" if is_close_to_wall_color(
        pixel, wall_colors) else "floor2.jpg"
    tile_data = {
        "tileId": tile_id,
        "worldPosition": [x, y],
        "sprite": sprite,
        "leftTileId": -1,
        "rightTileId": -1,
        "upTileId": -1,
        "downTileId": -1
    }

    tile_id_map[pos] = tile_id
    data.append(tile_data)
    return tile_id


# === Load reference wall colors ===
ref_img = Image.open('res/textures/wall_mask.jpg').convert('RGB')
wall_colors = set(ref_img.getdata())

# === Load map image ===
img = Image.open('res/textures/Test_Forest_2_test.jpg').convert('RGB')
width, height = img.size
tile_id_map = {}
data = []
tile_id = 0

# Traverse every pixel in the image
for y in range(height):
    print(y)
    for x in range(width):
        if (x, y) in tile_id_map:
            continue  # already generated

        current_id = generate_tile(
            x, y, tile_id_map, data, img, wall_colors, tile_id)
        tile_id += 1

        for dir, (dx, dy) in DIRECTIONS.items():
            nx, ny = x + dx, y + dy
            if not (0 <= nx < width and 0 <= ny < height):
                continue

            neighbor_pos = (nx, ny)
            if neighbor_pos in tile_id_map:
                neighbor_id = tile_id_map[neighbor_pos]
            else:
                neighbor_id = generate_tile(
                    nx, ny, tile_id_map, data, img, wall_colors, tile_id)
                tile_id += 1

            # Link current <-> neighbor
            for tile in data:
                if tile["tileId"] == current_id:
                    tile[f"{dir}TileId"] = neighbor_id
                if tile["tileId"] == neighbor_id:
                    tile[f"{OPPOSITE[dir]}TileId"] = current_id

            # === Handle vertical duplication if coming from left/right ===
            if dir in ["left", "right"]:
                for ddy in [-1, 1]:  # bottom, top
                    sx, sy = x, y + ddy
                    tx, ty = nx, ny + ddy
                    if not (0 <= sx < width and 0 <= sy < height and 0 <= tx < width and 0 <= ty < height):
                        continue

                    if (sx, sy) in tile_id_map:
                        if any(tile["worldPosition"] == [tx, ty] for tile in data):
                            continue
                        source_tile_id = tile_id_map[(sx, sy)]
                        source_tile = next(
                            t for t in data if t["tileId"] == source_tile_id)

                        # Duplicate with new tileId
                        new_tile_id = tile_id
                        tile_id += 1

                        new_tile = {
                            "tileId": new_tile_id,
                            "worldPosition": [tx, ty],
                            "sprite": source_tile["sprite"],
                            "leftTileId": -1,
                            "rightTileId": -1,
                            "upTileId": -1,
                            "downTileId": -1
                        }

                        data.append(new_tile)

                        # Link new duplicate to its neighbor
                        neighbor_id = tile_id_map[(nx, ny)]
                        if ny < sy:
                            for tile in data:
                                if tile["tileId"] == neighbor_id:
                                    tile["upTileId"] = new_tile_id
                            new_tile["downTileId"] = neighbor_id
                        else:
                            for tile in data:
                                if tile["tileId"] == neighbor_id:
                                    tile["downTileId"] = new_tile_id
                            new_tile["upTileId"] = neighbor_id
# for tile in data:
#    print(tile)


# Group tiles by worldPosition
grouped_tiles = defaultdict(list)

# Group the tiles by their worldPosition
for tile in data:
    # Convert worldPosition to a tuple
    world_pos_tuple = tuple(tile["worldPosition"])
    grouped_tiles[world_pos_tuple].append(tile)

# Iterate over the grouped tiles to set up/down links
for world_pos, tiles in grouped_tiles.items():
    if len(tiles) > 1:  # Only process if there are multiple tiles at the same position

        for i, tile in enumerate(tiles):
            for j, other_tile in enumerate(tiles):
                if i < j:  # Don't compare the tile with itself
                    # Set upTileId and downTileId between the two tiles
                    if tile["rightTileId"] == -1 and other_tile["upTileId"] == -1 and other_tile["rightTileId"] < other_tile["tileId"] and tile["upTileId"] < tile["tileId"] and other_tile["rightTileId"] != -1:

                        tile["rightTileId"] = other_tile["tileId"]
                        other_tile["upTileId"] = tile["tileId"]
                    elif tile["leftTileId"] == -1 and other_tile["upTileId"] == -1 and other_tile["leftTileId"] < other_tile["tileId"] and tile["upTileId"] < tile["tileId"] and other_tile["leftTileId"] != -1:

                        tile["leftTileId"] = other_tile["tileId"]
                        other_tile["upTileId"] = tile["tileId"]
                    elif tile["rightTileId"] == -1 and other_tile["downTileId"] == -1 and other_tile["rightTileId"] < other_tile["tileId"] and tile["downTileId"] < tile["tileId"] and other_tile["rightTileId"] and other_tile["rightTileId"] != -1:
                        tile["rightTileId"] = other_tile["tileId"]
                        other_tile["downTileId"] = tile["tileId"]
                    elif tile["leftTileId"] == -1 and other_tile["downTileId"] == -1 and other_tile["leftTileId"] < other_tile["tileId"] and tile["downTileId"] < tile["tileId"] and other_tile["leftTileId"] != -1:
                        tile["leftTileId"] = other_tile["tileId"]
                        other_tile["downTileId"] = tile["tileId"]


# Optional: print to verify the results
# for tile in data:
    # print(tile)
"""
print("AAAAAAAAAAAA")
for tile in data:
    if (tile["leftTileId"] == -1 and tile["rightTileId"] == -1 and tile["downTileId"] == -1 and tile["upTileId"] == -1):
        print("A")
    else:
        print(tile)
"""
# === Save the map ===
with open("tilemap.json", "w") as f:
    json.dump(data, f, indent=2)

# Optional print
