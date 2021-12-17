import sys
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt


def main():
    if len(sys.argv) > 1:
        file_name = str(sys.argv[1])
    else:
        file_name = 'wyniki.csv'
    data = pd.read_csv(file_name, usecols=['graph_name', 'calculated_path',
                       'calculated_path_weight', 'defined_path_weight', 'time', 'cooling_method'])
    data = np.array(data)
    x = np.array([data[i][1].count(' ') for i in range(0, len(data), 2)])
    y_quality_geo = [100*(data[i][2]-data[i][3])/data[i][2]
                     for i in range(len(data)) if data[i][5] == 'geo']
    y_quality_boltzmann = [100*(data[i][2]-data[i][3])/data[i][2]
                           for i in range(len(data)) if data[i][5] == 'boltzmann']
    y_time_geo = [data[i][4] for i in range(len(data)) if data[i][5] == 'geo']
    y_time_boltzmann = [data[i][4]
                        for i in range(len(data)) if data[i][5] == 'boltzmann']
    col1 = 'steelblue'
    col2 = 'red'
    _, ax = plt.subplots()
    ax.plot(x, y_quality_geo, color=col1, marker='o',
            linestyle='solid', linewidth=3, label='geo(błąd)')
    ax.plot(x, y_quality_boltzmann, color=col1, marker='o',
            linestyle='dashed', linewidth=3, label='log(błąd)')
    ax.set_ylabel('Stosunek błędu do wartości optymalnej [%]', color=col1)
    ax.set_xlabel('Liczba wierzchołków w grafie')
    ax2 = ax.twinx()
    ax2.plot(x, y_time_geo, color=col2, marker='o',
             linestyle='solid', linewidth=3, label='geo(czas)')
    ax2.plot(x, y_time_boltzmann, color=col2, marker='o',
             linestyle='dashed', linewidth=3, label='log(czas)')
    ax2.set_ylabel('Czas wykonania algorytmu [s]', color=col2)
    ax.legend(loc=0)
    ax2.legend(loc=1)
    plt.show()


if __name__ == "__main__":
    main()
