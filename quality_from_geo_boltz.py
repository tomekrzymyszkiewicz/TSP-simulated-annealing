#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
from matplotlib import pyplot as pllt
from scipy.special import gamma

def main():
    if len(sys.argv)>1:
        data = pd.read_csv(str(sys.argv[1]),usecols=['graph_name','calculated_path','calculated_path_weight','defined_path_weight','cooling_method'])
    else:
        data = pd.read_csv('wyniki.csv',usecols=['graph_name','calculated_path','calculated_path_weight','defined_path_weight','cooling_method'])
    data = np.array(data)
    num_of_vertices = np.array([data[i][1].count(' ') for i in range(len(data))])
    X = [num_of_vertices[i] for i in range(0,len(data),2)]
    Y_geo = [(data[i][2]-data[i][3])/data[i][2] for i in range(len(data)) if data[i][4] == 'geo']
    Y_boltzmann = [(data[i][2]-data[i][3])/data[i][2] for i in range(len(data)) if data[i][4] == 'boltzmann']
    pllt.plot(X,Y_geo,label='geo')
    pllt.plot(X,Y_boltzmann,label='boltzmann')
    pllt.ylabel('Stosunek błędu do wartości optymalnej')
    pllt.xlabel('Liczba wierzchołków w grafie')
    pllt.show()


if __name__ == "__main__":
    main()
