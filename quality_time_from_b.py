#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt

def main():
    if len(sys.argv)>1:
        file_name = str(sys.argv[1])
    else:
        file_name = 'wyniki.csv'
    data = pd.read_csv(file_name,usecols=['graph_name','calculated_path_weight','defined_path_weight','time','b'])
    data = np.array(data)
    X = [data[i][4] for i in range(len(data))]
    Y_time = [data[i][3] for i in range(len(data))]
    Y_quality = [100*(data[i][1]-data[i][2])/data[i][1] for i in range(len(data))]
    col1 = 'steelblue'
    col2 = 'red'
    fig,ax = plt.subplots()
    ax.plot(X,Y_quality,color=col1,marker='o',linewidth=3)
    ax.set_ylabel('Stosunek błędu do wartości optymalnej [%]',color=col1)
    ax.set_xlabel('Współczynnik b')
    ax2 = ax.twinx()
    ax2.plot(X,Y_time,color=col2,marker='o',linewidth=3)
    ax2.set_ylabel('Czas wykonania algorytmu [s]',color=col2)
    for i in range(len(data)):
        ax.annotate(''+str(data[i][4]),(X[i],Y_quality[i]), rotation=45)
    for i in range(len(data)):
        ax2.annotate(''+str(data[i][4]),(X[i],Y_time[i]), rotation=45)
    plt.show()


if __name__ == "__main__":
    main()
