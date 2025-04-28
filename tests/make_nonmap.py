import json

# Read the original JSON
with open('ordered_neighbors_tiling2.json', 'r') as f:
    data = json.load(f)

# Transform the data
transformed = []
for tile in data:
    neighbors = tile["neighbors"]

    # Check if any neighbor is -1
    if -1 in neighbors:
        up = left = down = right = -1
        sprite = "wall.jpg"
    else:
        up = neighbors[0]
        left = neighbors[1]
        down = neighbors[2]
        right = neighbors[3]
        sprite = "floor2.jpg"

    new_tile = {
        "tileId": tile["id"],
        "upTileId": up,
        "leftTileId": left,
        "downTileId": down,
        "rightTileId": right,
        "sprite": sprite
    }
    transformed.append(new_tile)

# Write the new JSON
with open('output.json', 'w') as f:
    json.dump(transformed, f, indent=2)

print("Transformation complete!")
