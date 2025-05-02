import json
import random

# Load your tile data
with open('tests/json/forest_test_small.json', 'r') as file:
    tiles = json.load(file)

# Replacement options
grass_textures = ['grass1.png', 'grass2.png', 'grass3.png', 'grass4.png']
wall_textures = ['rock1.png', 'rock2.png', 'stump.png']

# Modify tiles
for tile in tiles:
    sprite = tile.get('sprite')
    if sprite == 'floor2.jpg':
        tile['sprite'] = random.choice(grass_textures)
    elif sprite == 'wall.jpg':
        tile['sprite'] = random.choice(wall_textures)

# Save updated tile data
with open('tests/json/forest_test_small2.json', 'w') as file:
    json.dump(tiles, file, indent=4)
