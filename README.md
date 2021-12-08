# TSP-simulated-annealing

The project aims to solve TSP by simulated annealing method

## config.ini file

The program reads data from the config.ini configuration file in the following syntax

```
<results file name>
<graph file name> <number of repeats> <alpha parameter> <b parameter> <era_length> <cooling_method> <neighborhood method> <correct TSP path weight> <correct TSP path> 
```
- results file name - name of CSV file
- graph file name - name of file with adjacency matrix of graph
- number of repeats - number of repeats of algorithm
- alpha parameter - cooling parameter from range [0.85,1)
- b parameter - cooling parameter (for Boltzmann method)
- era_length - number of iterations to find the best solution for a given temperature
- cooling_method - *geo* or *boltzmann*
- neighborhood method - *swap* or *invert*
- correct TSP path weight - integer number
- correct TSP path - sequence of integers number separated by space