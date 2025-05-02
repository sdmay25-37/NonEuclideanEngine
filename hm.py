import numpy as np
import json


def mobius_apply(z, a, b, c, d):
    return (a * z + b) / (c * z + d)


def mobius_compose(m1, m2):
    a1, b1, c1, d1 = m1
    a2, b2, c2, d2 = m2
    return (
        a1*a2 + b1*c2,
        a1*b2 + b1*d2,
        c1*a2 + d1*c2,
        c1*b2 + d1*d2
    )


def rotation(theta):
    """Möbius rotation: z -> e^{iθ} * z"""
    e = np.exp(1j * theta)
    return (e, 0, 0, 1)


def translation(direction, radius=0.5):
    """Approximate hyperbolic step in a given angle"""
    angle = direction * (np.pi / 2)  # 90° steps for square tiles
    z = radius * np.exp(1j * angle)
    return (1, z, 0, 1)


def edge_transform(direction):
    """Rotate then translate in that direction"""
    return mobius_compose(translation(direction), rotation(direction * (np.pi / 2)))


class Tile:
    def __init__(self, word, transform, parent, from_direction=None):
        self.id = word
        self.transform = transform
        self.position = mobius_apply(0, *transform)
        self.parent = parent
        # New field for the direction from the parent
        self.from_direction = from_direction
        self.depth = len(word)
        self.neighbors = {}


class HyperbolicTiling45:
    def __init__(self, max_depth=3):
        self.tiles = {}
        self.max_depth = max_depth
        self.directions = [0, 1, 2, 3]  # square edges
        self.opposite = {"0": "2", "1": "3", "2": "0", "3": "1"}

    def generate(self):
        root = Tile("", (1, 0, 0, 1), None, None)
        self.tiles[""] = root
        queue = [root]

        while queue:
            current = queue.pop(0)
            if current.depth >= self.max_depth:
                continue
            for d in self.directions:
                d_str = str(d)
                new_id = current.id + d_str

                # Create child tile if it doesn't exist
                if new_id not in self.tiles:
                    tform = mobius_compose(
                        current.transform, edge_transform(d))
                    # Set the fromDirection
                    tile = Tile(new_id, tform, current.id, d_str)
                    self.tiles[new_id] = tile
                    queue.append(tile)

                # Link both directions
                current.neighbors[d_str] = new_id
                opposite_dir = self.opposite[d_str]
                self.tiles[new_id].neighbors[opposite_dir] = current.id

    def to_json(self):
        data = []
        for tile in self.tiles.values():
            data.append({
                "id": tile.id,
                "position": {"x": tile.position.real, "y": tile.position.imag},
                "parent": tile.parent,
                "fromDirection": tile.from_direction,  # Include the fromDirection
                "depth": tile.depth,
                "neighbors": tile.neighbors
            })
        return json.dumps(data, indent=2)

# ===== Run and Export =====


if __name__ == "__main__":
    tiling = HyperbolicTiling45(max_depth=10)
    tiling.generate()
    json_data = tiling.to_json()

    with open("tiling_4_5_with_direction.json", "w") as f:
        f.write(json_data)

    print("✅ Tiling generated and saved to tiling_4_5_with_direction.json")
