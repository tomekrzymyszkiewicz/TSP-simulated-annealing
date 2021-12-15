#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
from matplotlib import pyplot as pllt
from scipy.special import gamma

def main():
    if len(sys.argv)>1:
        file_name = str(sys.argv[1])
    else:
        file_name = 'wyniki.csv'
    data = pd.read_csv(file_name,usecols=['graph_name','time','alpha'])
    data = np.array(data)
    X = [data[i][2] for i in range(len(data))]
    Y = [data[i][1] for i in range(len(data))]
    pllt.scatter(X,Y)
    for i in range(len(data)):
        pllt.annotate(''+str(data[i][2]),(X[i],Y[i]), rotation=45)
    pllt.ylabel('Czas wykonania algorytmu [s]')
    pllt.xlabel('Współczynnik alfa')
    pllt.show()


if __name__ == "__main__":
    main()
