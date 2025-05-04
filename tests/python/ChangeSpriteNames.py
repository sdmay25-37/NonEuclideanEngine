import json
import random

# Load your tile data
with open('tests/json/forest_test_small2.json', 'r') as file:
    tiles = json.load(file)

# Replacement options
grass_textures = ['grass1.png', 'grass2.png', 'grass3.png', 'grass4.png']
wall_textures = ['rock1.png', 'rock2.png', 'stump.png']

# Select 200 unique random indices for tree placement
tree_indices = set(random.sample(range(len(tiles)), 200))
tree_indices2 = set(random.sample(range(len(tiles)), 200))
tree_indices3 = set(random.sample(range(len(tiles)), 200))

# Modify tiles
for idx, tile in enumerate(tiles):
    if idx in tree_indices:
        tile['sprite'] = 'tree.png'
    if idx in tree_indices2:
        tile['sprite'] = 'tree2.png'
    if idx in tree_indices3:
        tile['sprite'] = 'tree3.png'
    else:
        sprite = tile.get('sprite')
        if sprite == 'floor2.jpg':
            tile['sprite'] = random.choice(grass_textures)
        elif sprite == 'wall.jpg':
            tile['sprite'] = random.choice(wall_textures)

# Save updated tile data
with open('tests/json/forest_test_small3.json', 'w') as file:
    json.dump(tiles, file, indent=4)
