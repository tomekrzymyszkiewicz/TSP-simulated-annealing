import numpy as np
import pandas as pd
import matplotlib as plt
from matplotlib import pyplot as plt
import math
from scipy.special import gamma


def main():
    data = pd.read_csv('wyniki.csv')
    data = np.array(data)
    x = [data[i][0] for i in range(len(data))]
    y = [data[i][1]for i in range(len(data))]
    plt.plot(x, y)
    plt.xlabel('Liczba wierzchołków w grafie')
    plt.ylabel('Maksymalne zużycie pamięci[MB]')
    plt.show()


if __name__ == "__main__":
    main()
