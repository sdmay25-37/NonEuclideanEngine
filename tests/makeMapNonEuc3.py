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

center_x, center_y = width // 2, height // 2
tile_id_map = {}
data = []
tile_id = 0

# Generate center tile
center_id = generate_tile(center_x, center_y, tile_id_map,
                          data, img, wall_colors, tile_id)
tile_id += 1

queue = [(center_x, center_y)]
visited = set(queue)

for _ in range(2):  # Expand more rings if needed
    next_queue = []
    for x, y in queue:
        current_id = tile_id_map[(x, y)]

        for dir, (dx, dy) in DIRECTIONS.items():
            nx, ny = x + dx, y + dy
            npos = (nx, ny)
            if not (0 <= nx < width and 0 <= ny < height):
                continue
            if npos in visited:
                continue
            visited.add(npos)

            # Generate new tile for neighbor
            neighbor_id = generate_tile(
                nx, ny, tile_id_map, data, img, wall_colors, tile_id)
            tile_id += 1

            # Link current <-> neighbor
            for tile in data:
                if tile["tileId"] == current_id:
                    tile[f"{dir}TileId"] = neighbor_id
                if tile["tileId"] == neighbor_id:
                    tile[f"{OPPOSITE[dir]}TileId"] = current_id

            next_queue.append(npos)

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

                        # Do NOT update tile_id_map to avoid overwriting original position
                        # This makes sure the duplicate is only connected locally

                        # Link new duplicate to its neighbors
                        new_tile_id = tile_id - 1
                        neighbor_id = tile_id_map[(nx, ny)]
                        for tile in data:
                            if tile["tileId"] == neighbor_id:
                                print("HERE")
                                print(tile)
                                print(new_tile)
                                print("AAAA")
                                if ny < sy:  # If new tile is on top
                                    tile["upTileId"] = new_tile_id
                                    new_tile["downTileId"] = neighbor_id
                                else:  # If new tile is on bottom
                                    tile["downTileId"] = new_tile_id
                                    new_tile["upTileId"] = neighbor_id

    queue = next_queue

for tile in data:
    print(tile)


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
        print(f"Tiles at position {world_pos}:")
        for tile in tiles:
            print(tile)
        print()
        for i, tile in enumerate(tiles):
            for j, other_tile in enumerate(tiles):
                if i != j:  # Don't compare the tile with itself
                    # Set upTileId and downTileId between the two tiles
                    if tile["rightTileId"] == -1 and other_tile["upTileId"] == -1 and other_tile["rightTileId"] != -1:
                        tile["rightTileId"] = other_tile["tileId"]
                        other_tile["upTileId"] = tile["tileId"]
                    elif tile["leftTileId"] == -1 and other_tile["upTileId"] == -1 and other_tile["leftTileId"] != -1:
                        tile["leftTileId"] = other_tile["tileId"]
                        other_tile["upTileId"] = tile["tileId"]
                    elif tile["rightTileId"] == -1 and other_tile["downTileId"] == -1 and other_tile["rightTileId"] != -1:
                        tile["rightTileId"] = other_tile["tileId"]
                        other_tile["downTileId"] = tile["tileId"]
                    elif tile["leftTileId"] == -1 and other_tile["downTileId"] == -1 and other_tile["leftTileId"] != -1:
                        tile["leftTileId"] = other_tile["tileId"]
                        other_tile["downTileId"] = tile["tileId"]


# Optional: print to verify the results
for tile in data:
    print(tile)
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
