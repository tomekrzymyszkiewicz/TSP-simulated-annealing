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
## Graphs

In the *graphs* folder there are files with example graphs that can be used to test the program.

| graph file name | number of vertices | weight of minimum hamilton cycle |
|-----------------|--------------------|----------------------------------|
| tsp\_6\_1.txt   | 6                  | 132                              | 
| tsp\_10.txt     | 10                 | 212                              |
| tsp\_12.txt     | 12                 | 264                              | 
| tsp\_13.txt     | 13                 | 269                              |
| burma14.txt     | 14                 | 3323                             |  
| tsp\_15.txt     | 15                 | 291                              | 
| tsp\_17.txt     | 17                 | 39                               |   
| gr21.txt        | 21                 | 2707                             |   
| gr24.txt        | 24                 | 1272                             |  
| bays29.txt      | 29                 | 2020                             |    
| att48.txt       | 48                 | 10628                            |    
| eil51.txt       | 51                 | 426                              |   
| berlin52.txt    | 52                 | 7542                             |   
| st70.txt        | 70                 | 675                              |   
| gr96.txt        | 96                 | 55209                            |   
| kroA100.txt     | 100                | 21282                            |   
| gr120.txt       | 120                | 6942                             | 
| kroB150.txt     | 150                | 26130                            |   
| pr152.txt       | 152                | 73682                            |   
| ftv170.txt      | 170                | 2755                             |  
| kroB200.txt     | 200                | 29437                            |   
| rbg323.txt      | 323                | 1326                             |  