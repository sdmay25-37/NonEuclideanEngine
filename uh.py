import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

# Helper functions for Möbius transformations


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

# Generate tile positions in Poincaré disk


def generate_tile_positions(depth=3):
    positions = {}  # Store positions of tiles
    directions = [0, 1, 2, 3]  # 4 directions for 4 edges of the tile
    center = np.exp(0j)  # Center of the Poincaré disk

    def recursive_generate(tile_id, position, parent_id=None, from_direction=None, depth=0):
        if depth >= 4:
            return
        positions[tile_id] = position
        for direction in directions:
            new_tile_id = f"{tile_id}{direction}"
            if new_tile_id not in positions:
                # Apply Möbius transformation
                new_position = mobius_apply(position, 1, direction, 0, 1)
                recursive_generate(new_tile_id, new_position,
                                   tile_id, direction, depth + 1)

    # Start with the root tile at the center
    recursive_generate("", center)

    return positions

# Plot the Poincaré disk with tiles


def plot_poincare_disk(positions, title="Poincaré Disk"):
    fig, ax = plt.subplots(figsize=(8, 8))

    # Create the Poincaré disk
    circle = plt.Circle((0, 0), 1, edgecolor='black',
                        facecolor='none', linewidth=2)
    ax.add_artist(circle)

    # Plot tiles as squares
    for tile_id, position in positions.items():
        # Place square around each position
        rect = patches.Rectangle((position.real - 0.1, position.imag - 0.1),
                                 0.2, 0.2, linewidth=1, edgecolor='blue', facecolor='cyan', alpha=0.5)
        ax.add_patch(rect)

        # Annotate with tile ID
        ax.text(position.real, position.imag, tile_id,
                color="red", ha="center", va="center", fontsize=10)

    # Set axis limits and aspect ratio
    ax.set_xlim(-1.1, 1.1)
    ax.set_ylim(-1.1, 1.1)
    ax.set_aspect('equal', 'box')
    ax.set_title(title)

    # Hide axes
    ax.axis('off')

    plt.show()


# Generate positions and plot
tile_positions = generate_tile_positions(depth=3)
plot_poincare_disk(tile_positions)
