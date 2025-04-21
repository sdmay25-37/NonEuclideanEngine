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
print(width)

centerPixel = [int(width/2), int(height/2)]
print(int(height/2))
print(int(width/2))

pixel = img.getpixel((centerPixel[0], centerPixel[1]))
print(pixel)

centerSprite = "wall.jpg" if is_close_to_wall_color(
    pixel, wall_colors, threshold=30) else "floor2.jpg"

tile_id_map[centerPixel[0], centerPixel[1]] = tile_id
print(tile_id_map)
print([centerPixel[0], centerPixel[1]])

topPixel = centerPixel.copy()
topPixel[1] = topPixel[1] + 1
pixel = img.getpixel((topPixel[0], topPixel[1]))
print(pixel)

topSprite = "wall.jpg" if is_close_to_wall_color(
    pixel, wall_colors, threshold=30) else "floor2.jpg"

tile_id_map[topPixel[0], topPixel[1]] = tile_id + 1
print(tile_id_map)
print([topPixel[0], topPixel[1]])

bottomPixel = centerPixel.copy()

bottomPixel[1] = bottomPixel[1] - 1

pixel = img.getpixel((bottomPixel[0], bottomPixel[1]))
print(pixel)

bottomSprite = "wall.jpg" if is_close_to_wall_color(
    pixel, wall_colors, threshold=30) else "floor2.jpg"

tile_id_map[bottomPixel[0], bottomPixel[1]] = tile_id + 2
print([bottomPixel[0], bottomPixel[1]])

rightPixel = centerPixel.copy()
rightPixel[0] = rightPixel[0] + 1
pixel = img.getpixel((rightPixel[0], rightPixel[1]))
print(pixel)

rightSprite = "wall.jpg" if is_close_to_wall_color(
    pixel, wall_colors, threshold=30) else "floor2.jpg"

tile_id_map[rightPixel[0], rightPixel[1]] = tile_id + 3
print([rightPixel[0], rightPixel[1]])

leftPixel = centerPixel.copy()
leftPixel[0] = leftPixel[0] - 1
pixel = img.getpixel((leftPixel[0], leftPixel[1]))
print(pixel)

leftSprite = "wall.jpg" if is_close_to_wall_color(
    pixel, wall_colors, threshold=30) else "floor2.jpg"

tile_id_map[leftPixel[0], leftPixel[1]] = tile_id + 4


print([leftPixel[0], leftPixel[1]])

data.append({
    "tileId": tile_id,
    "worldPosition": [centerPixel[0], centerPixel[1]],
    "sprite": centerSprite,
    "leftTileId": tile_id+4,
    "rightTileId": tile_id+3,
    "upTileId": tile_id+1,
    "downTileId": tile_id+2,

})
tile_id
data.append({
    "tileId": tile_id + 1,
    "worldPosition": [topPixel[0], topPixel[1]],
    "sprite": topSprite,
    "leftTileId": -1,
    "rightTileId": -1,
    "upTileId": -1,
    "downTileId": tile_id,
})

data.append({
    "tileId": tile_id+2,
    "worldPosition": [bottomPixel[0], bottomPixel[1]],
    "sprite": bottomSprite,
    "leftTileId": -1,
    "rightTileId": -1,
    "upTileId": tile_id,
    "downTileId": -1,
})

data.append({
    "tileId": tile_id+3,
    "worldPosition": [rightPixel[0], rightPixel[1]],
    "sprite": rightSprite,
    "leftTileId": tile_id,
    "rightTileId": -1,
    "upTileId": -1,
    "downTileId": -1,
})

data.append({
    "tileId": tile_id+4,
    "worldPosition": [leftPixel[0], leftPixel[1]],
    "sprite": leftSprite,
    "leftTileId": -1,
    "rightTileId": tile_id,
    "upTileId": -1,
    "downTileId": -1,
})

tile_id += 5
# for tile in data:
#    print(tile)


top_tile_pos = (centerPixel[0], centerPixel[1]+1)
new_tile_positions = {
    "top": (top_tile_pos[0], top_tile_pos[1] + 1),
    "bottom": (top_tile_pos[0], top_tile_pos[1] - 1),
    "right":  (top_tile_pos[0] + 1, top_tile_pos[1]),
    "left": (top_tile_pos[0] - 1, top_tile_pos[1]),

}

direction_offsets = {
    "top": (0, 1),
    "bottom": (0, -1),
    "right": (1, 0),
    "left": (-1, 0),

}

for dir, pos in new_tile_positions.items():
    if pos in tile_id_map:
        continue  # skip if tile already exists

    pixel = img.getpixel(pos)
    sprite = "wall.jpg" if is_close_to_wall_color(
        pixel, wall_colors, threshold=30) else "floor2.jpg"

    new_id = tile_id
    tile_id_map[pos] = new_id

    tile_data = {
        "tileId": new_id,
        "worldPosition": [pos[0], pos[1]],
        "sprite": sprite,
        "leftTileId": -1,
        "rightTileId": -1,
        "upTileId": -1,
        "downTileId": -1
    }

    # Set neighbor pointing to the left_tile
    if dir == "left":
        tile_data["rightTileId"] = tile_id_map[top_tile_pos]
    elif dir == "top":
        tile_data["downTileId"] = tile_id_map[top_tile_pos]
    elif dir == "right":
        tile_data["leftTileId"] = tile_id_map[top_tile_pos]
    elif dir == "bottom":
        tile_data["upTileId"] = tile_id_map[top_tile_pos]

    # Optional: Update left_tile to point to this new tile (if desired)
    for existing_tile in data:
        if existing_tile["worldPosition"] == [top_tile_pos[0], top_tile_pos[1]]:
            if dir == "left":
                existing_tile["leftTileId"] = new_id
            elif dir == "top":
                existing_tile["upTileId"] = new_id
            elif dir == "right":
                existing_tile["rightTileId"] = new_id
            elif dir == "bottom":
                existing_tile["downTileId"] = new_id
            break

    data.append(tile_data)
    tile_id += 1

bottom_tile_pos = (centerPixel[0], centerPixel[1]-1)
new_tile_positions = {
    "top": (bottom_tile_pos[0], bottom_tile_pos[1] + 1),
    "bottom": (bottom_tile_pos[0], bottom_tile_pos[1] - 1),
    "right":  (bottom_tile_pos[0] + 1, bottom_tile_pos[1]),
    "left": (bottom_tile_pos[0] - 1, bottom_tile_pos[1]),

}

direction_offsets = {
    "top": (0, 1),
    "bottom": (0, -1),
    "right": (1, 0),
    "left": (-1, 0),

}

for dir, pos in new_tile_positions.items():
    if pos in tile_id_map:
        continue  # skip if tile already exists

    pixel = img.getpixel(pos)
    sprite = "wall.jpg" if is_close_to_wall_color(
        pixel, wall_colors, threshold=30) else "floor2.jpg"

    new_id = tile_id
    tile_id_map[pos] = new_id

    tile_data = {
        "tileId": new_id,
        "worldPosition": [pos[0], pos[1]],
        "sprite": sprite,
        "leftTileId": -1,
        "rightTileId": -1,
        "upTileId": -1,
        "downTileId": -1
    }

    # Set neighbor pointing to the left_tile
    if dir == "left":
        tile_data["rightTileId"] = tile_id_map[bottom_tile_pos]
    elif dir == "top":
        tile_data["downTileId"] = tile_id_map[bottom_tile_pos]
    elif dir == "right":
        tile_data["leftTileId"] = tile_id_map[bottom_tile_pos]
    elif dir == "bottom":
        tile_data["upTileId"] = tile_id_map[bottom_tile_pos]

    # Optional: Update left_tile to point to this new tile (if desired)
    for existing_tile in data:
        if existing_tile["worldPosition"] == [bottom_tile_pos[0], bottom_tile_pos[1]]:
            if dir == "left":
                existing_tile["leftTileId"] = new_id
            elif dir == "top":
                existing_tile["upTileId"] = new_id
            elif dir == "right":
                existing_tile["rightTileId"] = new_id
            elif dir == "bottom":
                existing_tile["downTileId"] = new_id
            break

    data.append(tile_data)
    tile_id += 1


for tile in data:
    print(tile)
right_tile_pos = (centerPixel[0] + 1, centerPixel[1])
new_tile_positions = {
    "top": (right_tile_pos[0], right_tile_pos[1] + 1),
    "bottom": (right_tile_pos[0], right_tile_pos[1] - 1),
    "right":  (right_tile_pos[0] + 1, right_tile_pos[1]),
    "left": (right_tile_pos[0] - 1, right_tile_pos[1]),

}

direction_offsets = {
    "top": (0, 1),
    "bottom": (0, -1),
    "right": (1, 0),
    "left": (-1, 0),

}

for dir, pos in new_tile_positions.items():
    if pos in tile_id_map:
        print(pos)
        print(dir)
        print("IT EXISTS")
        if dir == "top":
            print("HERE")
            pixel = img.getpixel(pos)
            sprite = "wall.jpg" if is_close_to_wall_color(
                pixel, wall_colors, threshold=30) else "floor2.jpg"
            new_id = tile_id
            tile_id_map[pos] = new_id
            tile_data = {
                "tileId": new_id,
                "worldPosition": [pos[0], pos[1]],
                "sprite": sprite,
                "leftTileId": -1,
                "rightTileId": -1,
                "upTileId": -1,
                "downTileId": -1
            }
            tile_data["downTileId"] = tile_id_map[right_tile_pos]
            print(data)
            for existing_tile in data:
                print(existing_tile)
                if existing_tile["worldPosition"] == [right_tile_pos[0], right_tile_pos[1]]:
                    if dir == "left":
                        existing_tile["leftTileId"] = new_id
                    elif dir == "top":
                        existing_tile["upTileId"] = new_id
                    elif dir == "right":
                        existing_tile["rightTileId"] = new_id
                    elif dir == "bottom":
                        existing_tile["downTileId"] = new_id
                    break
            data.append(tile_data)
            print(tile_data)
            tile_id += 1
        if dir == "bottom":
            print("HERE")
            pixel = img.getpixel(pos)
            sprite = "wall.jpg" if is_close_to_wall_color(
                pixel, wall_colors, threshold=30) else "floor2.jpg"
            new_id = tile_id
            tile_id_map[pos] = new_id
            tile_data = {
                "tileId": new_id,
                "worldPosition": [pos[0], pos[1]],
                "sprite": sprite,
                "leftTileId": -1,
                "rightTileId": -1,
                "upTileId": -1,
                "downTileId": -1
            }
            tile_data["upTileId"] = tile_id_map[right_tile_pos]
            for existing_tile in data:
                if existing_tile["worldPosition"] == [right_tile_pos[0], right_tile_pos[1]]:
                    if dir == "left":
                        existing_tile["leftTileId"] = new_id
                    elif dir == "top":
                        existing_tile["upTileId"] = new_id
                    elif dir == "right":
                        existing_tile["rightTileId"] = new_id
                    elif dir == "bottom":
                        existing_tile["downTileId"] = new_id
                    break
            data.append(tile_data)
            print(tile_data)
            tile_id += 1
        print("added")
        continue  # skip if tile already exists

    pixel = img.getpixel(pos)
    sprite = "wall.jpg" if is_close_to_wall_color(
        pixel, wall_colors, threshold=30) else "floor2.jpg"

    new_id = tile_id
    tile_id_map[pos] = new_id

    tile_data = {
        "tileId": new_id,
        "worldPosition": [pos[0], pos[1]],
        "sprite": sprite,
        "leftTileId": -1,
        "rightTileId": -1,
        "upTileId": -1,
        "downTileId": -1
    }

    # Set neighbor pointing to the left_tile
    if dir == "left":
        tile_data["rightTileId"] = tile_id_map[right_tile_pos]
    elif dir == "top":
        tile_data["downTileId"] = tile_id_map[right_tile_pos]
    elif dir == "right":
        tile_data["leftTileId"] = tile_id_map[right_tile_pos]
    elif dir == "bottom":
        tile_data["upTileId"] = tile_id_map[right_tile_pos]

    # Optional: Update left_tile to point to this new tile (if desired)
    for existing_tile in data:
        if existing_tile["worldPosition"] == [right_tile_pos[0], right_tile_pos[1]]:
            if dir == "left":
                existing_tile["leftTileId"] = new_id
            elif dir == "top":
                existing_tile["upTileId"] = new_id
            elif dir == "right":
                existing_tile["rightTileId"] = new_id
            elif dir == "bottom":
                existing_tile["downTileId"] = new_id
            break

    data.append(tile_data)
    tile_id += 1

left_tile_pos = (centerPixel[0] - 1, centerPixel[1])
new_tile_positions = {
    "top": (left_tile_pos[0], left_tile_pos[1] + 1),
    "bottom": (left_tile_pos[0], left_tile_pos[1] - 1),
    "right":  (left_tile_pos[0] + 1, left_tile_pos[1]),
    "left": (left_tile_pos[0] - 1, left_tile_pos[1]),

}

direction_offsets = {
    "top": (0, 1),
    "bottom": (0, -1),
    "right": (1, 0),
    "left": (-1, 0),

}

for dir, pos in new_tile_positions.items():
    if pos in tile_id_map:
        print(pos)
        print(dir)
        print("IT EXISTS")
        if dir == "top":
            print("HERE")
            pixel = img.getpixel(pos)
            sprite = "wall.jpg" if is_close_to_wall_color(
                pixel, wall_colors, threshold=30) else "floor2.jpg"
            new_id = tile_id
            tile_id_map[pos] = new_id
            tile_data = {
                "tileId": new_id,
                "worldPosition": [pos[0], pos[1]],
                "sprite": sprite,
                "leftTileId": -1,
                "rightTileId": -1,
                "upTileId": -1,
                "downTileId": -1
            }
            tile_data["downTileId"] = tile_id_map[left_tile_pos]
            for existing_tile in data:
                print(existing_tile["tileId"])
                # print(existing_tile["worldPosition"])
                # print([pos[0], pos[1]])
                # print(existing_tile["worldPosition"] == [pos[0], pos[1]])
                if existing_tile["worldPosition"] == [pos[0], pos[1]]:
                    print(existing_tile["tileId"])

                if existing_tile["worldPosition"] == [left_tile_pos[0], left_tile_pos[1]]:
                    if dir == "left":
                        existing_tile["leftTileId"] = new_id
                    elif dir == "top":
                        existing_tile["upTileId"] = new_id
                    elif dir == "right":
                        existing_tile["rightTileId"] = new_id
                    elif dir == "bottom":
                        existing_tile["downTileId"] = new_id
                    break
            data.append(tile_data)
            print(tile_data)
            tile_id += 1
        if dir == "bottom":
            print("HERE")
            pixel = img.getpixel(pos)
            sprite = "wall.jpg" if is_close_to_wall_color(
                pixel, wall_colors, threshold=30) else "floor2.jpg"
            new_id = tile_id
            tile_id_map[pos] = new_id
            tile_data = {
                "tileId": new_id,
                "worldPosition": [pos[0], pos[1]],
                "sprite": sprite,
                "leftTileId": -1,
                "rightTileId": -1,
                "upTileId": -1,
                "downTileId": -1
            }
            tile_data["upTileId"] = tile_id_map[left_tile_pos]
            for existing_tile in data:
                if existing_tile["worldPosition"] == [left_tile_pos[0], left_tile_pos[1]]:
                    if dir == "left":
                        existing_tile["leftTileId"] = new_id
                    elif dir == "top":
                        existing_tile["upTileId"] = new_id
                    elif dir == "right":
                        existing_tile["rightTileId"] = new_id
                    elif dir == "bottom":
                        existing_tile["downTileId"] = new_id
                    break
            data.append(tile_data)
            print(tile_data)
            tile_id += 1
        print("added")
        continue

    pixel = img.getpixel(pos)
    sprite = "wall.jpg" if is_close_to_wall_color(
        pixel, wall_colors, threshold=30) else "floor2.jpg"

    new_id = tile_id
    tile_id_map[pos] = new_id

    tile_data = {
        "tileId": new_id,
        "worldPosition": [pos[0], pos[1]],
        "sprite": sprite,
        "leftTileId": -1,
        "rightTileId": -1,
        "upTileId": -1,
        "downTileId": -1
    }

    # Set neighbor pointing to the left_tile
    if dir == "left":
        tile_data["rightTileId"] = tile_id_map[left_tile_pos]
    elif dir == "top":
        tile_data["downTileId"] = tile_id_map[left_tile_pos]
    elif dir == "right":
        tile_data["leftTileId"] = tile_id_map[left_tile_pos]
    elif dir == "bottom":
        tile_data["upTileId"] = tile_id_map[left_tile_pos]

    # Optional: Update left_tile to point to this new tile (if desired)
    for existing_tile in data:
        if existing_tile["worldPosition"] == [left_tile_pos[0], left_tile_pos[1]]:
            if dir == "left":
                existing_tile["leftTileId"] = new_id
            elif dir == "top":
                existing_tile["upTileId"] = new_id
            elif dir == "right":
                existing_tile["rightTileId"] = new_id
            elif dir == "bottom":
                existing_tile["downTileId"] = new_id
            break

    data.append(tile_data)
    tile_id += 1

for tile in data:
    print(tile)

# First pass — assign tileId and store positions
'''
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
'''
