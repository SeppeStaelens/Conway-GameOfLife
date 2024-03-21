import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

### SETTINGS ###

data_path = "examples/Gosper_glider_gun/"

for i in range(0, 121):
    data_name = f"step{i}"

    data = pd.read_csv(data_path + "txt/" + data_name + ".txt", sep = " ", header=None)

    N = len(data)

    plt.figure()
    im = plt.imshow(data, cmap="Greys")

    ax = plt.gca()

    # Remove major ticks
    ax.set_xticks([], minor = False)
    ax.set_yticks([], minor = False)

    # Minor ticks
    ax.set_xticks(np.arange(-.5, N, 1), minor=True)
    ax.set_yticks(np.arange(-.5, N, 1), minor=True)

    # Gridlines based on minor ticks
    ax.grid(which='minor', color='grey', linestyle='-', linewidth=2)

    # Remove minor ticks
    ax.tick_params(which='minor', bottom=False, left=False)

    plt.savefig(data_path + "png/" + data_name + ".png")

    plt.close()

