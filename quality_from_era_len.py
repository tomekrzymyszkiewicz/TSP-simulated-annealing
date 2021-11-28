#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
from matplotlib import pyplot as pllt
from scipy.special import gamma
# graph_name,calculated_path,calculated_path_weight,defined_path,defined_path_weight,time,number_of_repeats,alpha,b,era_length,cooling_method,neighborhood_method

def main():
    if len(sys.argv)>1:
        data = pd.read_csv(str(sys.argv[1]),usecols=['graph_name','calculated_path_weight','defined_path_weight','era_length'])
    else:
        data = pd.read_csv('wyniki.csv',usecols=['graph_name','calculated_path_weight','defined_path_weight','era_length'])
    data = np.array(data)
    X = [data[i][3] for i in range(len(data))]
    Y = [(data[i][1]-data[i][2])/data[i][1]for i in range(len(data))]
    pllt.scatter(X,Y)
    for i in range(len(data)):
        pllt.annotate('epoka='+str(data[i][3]),(X[i],Y[i]))
    pllt.ylabel('Stosunek błędu do wartości optymalnej')
    pllt.xlabel('Długość epoki')
    pllt.show()


if __name__ == "__main__":
    main()
