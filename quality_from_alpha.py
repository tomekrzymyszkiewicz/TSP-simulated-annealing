#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
import matplotlib as plt
from matplotlib import pyplot as pllt
import math
from scipy.special import gamma
# graph_name,calculated_path,calculated_path_weight,defined_path,defined_path_weight,time,number_of_repeats,alpha,b,era_length,cooling_method,neighborhood_method

def main():
    if len(sys.argv)>1:
        data = pd.read_csv(str(sys.argv[1]),usecols=['graph_name','calculated_path_weight','defined_path_weight','alpha'])
    else:
        data = pd.read_csv('wyniki.csv',usecols=['graph_name','calculated_path_weight','defined_path_weight','alpha'])
    data = np.array(data)
    # calculated_path_weights = [data[i][2] for i in range(len(data))]
    # defined_path_weights = [data[i][4] for i in range(len(data))]
    # parameters_alpha = [data[i][7] for i in range(len(data))]
    # era_length = [data[i][9] for i in range(len(data))]
    # num_of_unique_values_of_era = len(set(data[]))
    # num_of_unique_values_of_alpha = len(set(parameters_alpha))
    # plots = [[]*num_of_unique_values_of_alpha]*num_of_unique_values_of_era
    # counter = 0;
    # for i in range(num_of_unique_values_of_era):
    #     for j in range(num_of_unique_values_of_alpha):
    #         plots[i][j].append((calculated_path_weights[i*num_of_unique_values_of_era+num_of_unique_values_of_alpha]-defined_path_weights[i*num_of_unique_values_of_era+num_of_unique_values_of_alpha])/defined_path_weights[i*num_of_unique_values_of_era+num_of_unique_values_of_alpha])
    #         plots[i][j].append(parameters_alpha[i*num_of_unique_values_of_era+num_of_unique_values_of_alpha])
    #         plots[i][j].append(era_length[i*num_of_unique_values_of_era+num_of_unique_values_of_alpha])
    # for i in range(num_of_unique_values_of_era):
    X = [data[i][3] for i in range(len(data))]
    Y = [(data[i][1]-data[i][2])/data[i][1]for i in range(len(data))]
    pllt.scatter(X,Y)
    for i in range(len(data)):
        pllt.annotate('alfa='+str(data[i][3]),(X[i],Y[i]))
    pllt.xlabel('Stosunek błędu do wartości optymalnej')
    pllt.ylabel('Współczynnik alfa')
    pllt.show()


if __name__ == "__main__":
    main()
