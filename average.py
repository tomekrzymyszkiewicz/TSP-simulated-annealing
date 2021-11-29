#!/usr/bin/python
import sys
import numpy as np
import pandas as pd
import csv
import re
# graph_name,calculated_path,calculated_path_weight,defined_path,defined_path_weight,time,number_of_repeats,alpha,b,era_length,cooling_method,neighborhood_method

def main():
    if len(sys.argv)>1:
        data = pd.read_csv(str(sys.argv[1]))
        file_name = re.sub(r"\.\\",'',str(sys.argv[1]))
        # file_name = re.search("[a-zA-Z0-9_]+\.[a-zA-Z]*",str(sys.argv[1])).string
    else:
        data = pd.read_csv('wyniki.csv')
        file_name = 'wyniki.csv'
    data = np.array(data)
    unique_records = [] #graph_name,number_of_repeats,alpha,b,era_length,cooling_method,neighborhood_method
    for record in data:
        val = [record[0],record[6],record[7],record[8],record[9],record[10],record[11]]
        if val not in unique_records:
            unique_records.append(val)
    print("Number of unique records: "+str(len(unique_records)))
    output = []
    for unique_record in unique_records:
        single_line = []
        single_line.append(unique_record[0])    #graph_name
        single_line.append(None)                #calculated_path
        single_line.append(None)                #calculated_path_weight
        single_line.append(None)                #defined_path
        single_line.append(None)                #defined_path_weight
        single_line.append(None)                #time
        single_line.append(unique_record[1])    #number_of_repeats
        single_line.append(unique_record[2])    #alpha
        single_line.append(unique_record[3])    #b
        single_line.append(unique_record[4])    #era_length
        single_line.append(unique_record[5])    #cooling_method
        single_line.append(unique_record[6])    #neighborhood_method
        calculated_path = 0
        calculated_path_weight = 0
        defined_path = ""
        defined_path_weight = 0
        time = 0.0
        for record in data:
            val = [record[0],record[6],record[7],record[8],record[9],record[10],record[11]]
            if val == unique_record:
                defined_path = record[3]
                defined_path_weight = record[4]
                calculated_path = "0 0"
                calculated_path_weight += record[2]
                time += record[5]
        time /= unique_record[1]
        calculated_path_weight /= unique_record[1]
        single_line[1] = calculated_path
        single_line[2] = calculated_path_weight
        single_line[3] = defined_path
        single_line[4] = defined_path_weight
        single_line[5] = time
        output.append(single_line)
    fields = ['graph_name','calculated_path','calculated_path_weight','defined_path','defined_path_weight','time','number_of_repeats','alpha','b','era_length','cooling_method','neighborhood_method']
    with open('average_of_'+file_name,'w') as file:
        write = csv.writer(file)
        write.writerow(fields)
        write.writerows(output)
              

            


if __name__ == "__main__":
    main()
