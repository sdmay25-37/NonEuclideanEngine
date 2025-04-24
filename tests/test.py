import json
import math
from collections import deque


class Tile:
    def __init__(self, id, x, y):
        self.id = id
        self.x = x
        self.y = y
        self.neighbor_ids = [-1] * 5  # 5 neighbors in a {4,5} tiling
        self.sprite = "floor2.jpg"

    def to_dict(self):
        # Map the 5 directions into left/right/up/down arbitrarily
        dir_map = {
            "leftTileId": self.neighbor_ids[3],
            "rightTileId": self.neighbor_ids[1],
            "upTileId": self.neighbor_ids[0],
            "downTileId": self.neighbor_ids[2]
        }
        return {
            "tileId": self.id,
            "worldPosition": [self.x, self.y],
            "sprite": self.sprite,
            **dir_map
        }


def polar_to_cartesian(r, angle):
    return r * math.cos(angle), r * math.sin(angle)


def create_order_5_square_tiling(max_depth=3):
    angle_offset = 2 * math.pi / 5  # 72 degrees between neighbors
    tiles = {}
    tile_id_counter = 0

    # Place the first tile at origin
    origin = Tile(tile_id_counter, 0.0, 0.0)
    tiles[(0.0, 0.0)] = origin
    queue = deque()
    queue.append((origin, 0))
    tile_id_counter += 1

    while queue:
        current_tile, depth = queue.popleft()
        if depth >= max_depth:
            continue

        for i in range(5):
            angle = i * angle_offset
            r = 1.0 / (depth + 1)  # decreasing radius outward
            dx, dy = polar_to_cartesian(r, angle)
            nx = round(current_tile.x + dx, 5)
            ny = round(current_tile.y + dy, 5)
            key = (nx, ny)

            if key not in tiles:
                new_tile = Tile(tile_id_counter, nx, ny)
                tiles[key] = new_tile
                queue.append((new_tile, depth + 1))
                tile_id_counter += 1
            else:
                new_tile = tiles[key]

            # Link both directions
            if current_tile.neighbor_ids[i] == -1:
                current_tile.neighbor_ids[i] = new_tile.id
            opposite_index = (i + 2) % 5  # 0↔2, 1↔3, 2↔4, 3↔0, 4↔1
            if new_tile.neighbor_ids[opposite_index] == -1:
                new_tile.neighbor_ids[opposite_index] = current_tile.id

    return [tile.to_dict() for tile in tiles.values()]


tiles = create_order_5_square_tiling(max_depth=3)

with open("order_5_square_tiling.json", "w") as f:
    json.dump(tiles, f, indent=2)

print(f"Generated {len(tiles)} tiles.")
