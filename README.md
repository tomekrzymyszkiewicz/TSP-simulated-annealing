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

## Results file

The resulting file is a CSV file with the following headers:
- graph_name - name of file with adjacency matrix of graph
- calculated_path - path determined by the operation of the algorithm
- calculated_path_weight - weight of the path determined by the operation of the algorithm
- defined_path - the path specified in the configuration file
- defined_path_weight - the weight of the path specified in the configuration file
- time - execution time of the algorithm (a single execution, if there were many executions)
- number_of_repeats - number of repeats of algorithm
- alpha - cooling parameter
- b - cooling parameter (for Boltzmann method)
- era_length - era length
- cooling_method - selected method *geo* or *boltzmann*
- neighborhood_method - selected method *swap* or *invert*

## Python average script

The *average.py* script, when run with a parameter that is the name of the output file with the result of the *main.cpp* program, creates a new file in which all repeatedly executed tasks are averaged to a single result.
The averaged files are suitable for creating graphs using the other Python scripts.
```
python average.py ./<results file name>
```

## Python plots script

The remaining Python scripts are used to generate the plots described in the script names. To create a plot, call the appropriate script with the parameter being the averaged result file.
```
python <name of plot script> ./<averaged results file name>

```
