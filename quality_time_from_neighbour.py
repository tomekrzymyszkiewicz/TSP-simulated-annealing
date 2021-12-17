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
    data = pd.read_csv(file_name,usecols=['graph_name','calculated_path','calculated_path_weight','defined_path_weight','time','neighborhood_method'])
    data = np.array(data)
    X =  np.array([data[i][1].count(' ') for i in range(0,len(data),2)])
    Y_quality_swap = [100*(data[i][2]-data[i][3])/data[i][2] for i in range(len(data)) if data[i][5] == 'swap']
    Y_quality_invert = [100*(data[i][2]-data[i][3])/data[i][2] for i in range(len(data)) if data[i][5] == 'invert']
    Y_time_swap = [data[i][4] for i in range(len(data)) if data[i][5] == 'swap']
    Y_time_invert = [data[i][4] for i in range(len(data)) if data[i][5] == 'invert']
    col1 = 'steelblue'
    col2 = 'red'
    fig,ax = plt.subplots()
    ax.plot(X,Y_quality_swap,color=col1,marker='o',linestyle='solid',linewidth=3,label='swap(błąd)')
    ax.plot(X,Y_quality_invert,color=col1,marker='o',linestyle='dashed',linewidth=3,label='invert(błąd)')
    ax.set_ylabel('Stosunek błędu do wartości optymalnej [%]',color=col1)
    ax.set_xlabel('Liczba wierzchołków w grafie')
    ax.legend()
    ax2 = ax.twinx()
    ax2.plot(X,Y_time_swap,color=col2,marker='o',linestyle='solid',linewidth=3,label='swap(czas)')
    ax2.plot(X,Y_time_invert,color=col2,marker='o',linestyle='dashed',linewidth=3,label='invert(czas)')
    ax2.set_ylabel('Czas wykonania algorytmu [s]',color=col2)
    ax2.legend()
    plt.show()
    

if __name__ == "__main__":
    main()
