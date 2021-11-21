#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
// #include <queue>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <sstream>
// #include <climits>
// #include <iterator>
#include <algorithm>
// #include <cctype>
// #include <locale>
// #include <cmath>
// #include<sys/resource.h>
#include <unistd.h>
#include "structures.h"
using namespace std;
using namespace std::chrono;

vector<vector<string>> tasks;
vector<string> results;
string results_file_name = "";
int number_of_current_graph_vertices = 0;
adjacency_matrix current_graph_adjacency_matrix = adjacency_matrix();
int *path_array;
int ***solved_subproblems;

void process_mem_usage(double &vm_usage, double &resident_set)
{
    using std::ifstream;
    using std::ios_base;
    using std::string;

    vm_usage = 0.0;
    resident_set = 0.0;

    ifstream stat_stream("/proc/self/stat", ios_base::in);

    string pid, comm, state, ppid, pgrp, session, tty_nr;
    string tpgid, flags, minflt, cminflt, majflt, cmajflt;
    string utime, stime, cutime, cstime, priority, nice;
    string O, itrealvalue, starttime;

    unsigned long vsize;
    long rss;

    stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >> utime >> stime >> cutime >> cstime >> priority >> nice >> O >> itrealvalue >> starttime >> vsize >> rss;

    stat_stream.close();

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024;
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

struct Result
{
    string graph_name;
    string path;
    int path_weight;
    double time;
    int number_of_repeats;
    Result(string graph_name, string path, int path_weight, double time, int number_of_repeats)
    {
        this->graph_name = graph_name;
        this->path = path;
        this->path_weight = path_weight;
        this->time = time;
        this->number_of_repeats = number_of_repeats;
    }
    string toString()
    {
        return (graph_name + "," + path + "," + to_string(path_weight) + "," + to_string(time) + "," + to_string(number_of_repeats));
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "graph_name,path,path_weight,time,number_of_repeats" << endl;
    for (long unsigned int i = 0; i < results.size(); i++)
    {
        fout << results[i] << endl;
    }
    fout.close();
    std::cout << "Correctly saved " << results.size() << " results" << endl;
}

template <typename Out>
void split(const std::string &s, char delim, Out result)
{
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        *result++ = item;
    }
}
std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

bool load_data(string file_name)
{
    std::cout << "Loading data from " << file_name << " file" << endl;
    ifstream fin;
    fin.open(file_name, ios::in);
    if (!fin.good())
    {
        std::cout << "Data file " << file_name << " not exist" << endl;
        fin.close();
        return false;
    }
    string loaded_source, loaded_destination, loaded_weight;
    string loaded_number_of_vertices;
    getline(fin, loaded_number_of_vertices);
    number_of_current_graph_vertices = stoi(loaded_number_of_vertices);
    current_graph_adjacency_matrix = adjacency_matrix(number_of_current_graph_vertices);
    for (int i = 0; i < number_of_current_graph_vertices; i++)
    {
        string loaded_line_of_matrix = "";
        getline(fin, loaded_line_of_matrix);
        ltrim(loaded_line_of_matrix);
        rtrim(loaded_line_of_matrix);
        vector<string> single_line = split(loaded_line_of_matrix, ' ');
        std::vector<std::string>::iterator it = single_line.begin();
        while (it != single_line.end())
        {
            if (it->length() == 0)
            {
                it = single_line.erase(it);
            }
            else
            {
                ++it;
            }
        }
        for (long unsigned int j = 0; j < single_line.size(); j++)
        {
            ltrim(single_line[j]);
            rtrim(single_line[j]);
            current_graph_adjacency_matrix.add_edge_dir(i, j, stoi(single_line[j]));
        }
    }
    std::cout << "Loaded correctly graph with " << number_of_current_graph_vertices << " vertices" << endl
              << "Graph:" << endl;
    current_graph_adjacency_matrix.print();
    fin.close();
    return true;
}

void load_config()
{
    std::cout << "Loading config.ini" << endl;
    ifstream fin;
    fin.open("config.ini", ios::in);
    if (!fin.good())
    {
        std::cout << "Config.ini not found" << endl;
        fin.close();
        return;
    }
    string loaded_line_of_task = "";
    getline(fin, results_file_name);
    while (getline(fin, loaded_line_of_task))
    {
        vector<string> single_line = split(loaded_line_of_task, ' ');
        string graph_file_name, number_of_repeats, shortest_path_weight, shortest_path;
        if (single_line.size() >= 4)
        {
            graph_file_name = single_line[0];
            number_of_repeats = single_line[1];
            shortest_path_weight = single_line[2];
            for (long unsigned int i = 3; i < single_line.size(); i++)
            {
                shortest_path += single_line[i];
                shortest_path += " ";
            }
        }
        if (graph_file_name.size() == 0 || number_of_repeats.size() == 0 || shortest_path_weight.size() == 0 || shortest_path.size() == 0)
        {
            std::cout << "Cannot load this task: " << graph_file_name << " " << number_of_repeats << " " << shortest_path_weight << " " << shortest_path;
            break;
        }
        vector<string> task;
        task.push_back(graph_file_name);
        task.push_back(number_of_repeats);
        task.push_back(shortest_path_weight);
        task.push_back(shortest_path);
        tasks.push_back(task);
    }
    fin.close();
    std::cout << "Config loaded correctly" << endl;
    return;
}

int cost_of_permutation(vector<int> permutation)
{
    int cost = current_graph_adjacency_matrix.matrix[0][permutation[0]];
    for (int i = 0; i < number_of_current_graph_vertices - 2; i++)
    {
        cost += current_graph_adjacency_matrix.matrix[permutation[i]][permutation[i + 1]];
    }
    cost += current_graph_adjacency_matrix.matrix[permutation[permutation.size() - 1]][0];
    return cost;
}

float initial_temperature(int cost, float alpha)
{
    return cost * alpha;
}

vector<int> initial_permutation(int permutation_quantity){
    vector<int> permutation;
    for (int i = 1; i < permutation_quantity; i++)
        permutation.push_back(i);
    random_device rd;
    default_random_engine rng(rd());
    shuffle(permutation.begin(), permutation.end(), rng);
    return permutation;
}

vector<int> swap_permutation(vector<int> permutation, int first_index, int second_index)
{
    std::iter_swap(permutation.begin() + first_index, permutation.begin() + second_index);
    return permutation;
}

vector<int> neighborhood_permutation(vector<int> permutation)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    int first_index = (int)(dis(gen) * 10000) % permutation.size();
    int second_index = (int)(dis(gen) * 10000) % permutation.size();

    if (first_index != second_index)
        permutation = swap_permutation(permutation, first_index, second_index);
    return permutation;
}

float new_temp_geo(float temp, float alpha)
{
    return temp * alpha;
}

string print_vector(vector<int> printed_vector)
{
    string answer;
    for (long unsigned int i = 0; i < printed_vector.size(); i++)
    {
        answer += to_string(printed_vector[i]);
        answer += " ";
    }
    return answer;
}

/**
 * TSP solving with simulated annealing method
 * 
 * @param alpha cooling parameter
 * @param era_length number of iterations to find the best solution for a given temperature
 * @param cooling_method 0 - geometrical cooling | 1 - Boltzmann
 * @param neighborhood_method 0 - swap | 1 - invert
 * @return pair of calculated path and weight
 */
pair<vector<int>, int> TSP_solve(float alpha = 0.9, int era_length = 1, bool cooling_method = false, bool neighborhood_method = false)
{
    vector<int> permutation = initial_permutation(number_of_current_graph_vertices);
    int cost = cost_of_permutation(permutation);
    float min_temp = 0.000001; //minimal temperature - stop condition
    float current_temp = initial_temperature(cost, alpha);
    while (current_temp > min_temp)
    {
        // printf("Current temp: %6lf| Cost: %6d\n",current_temp,cost);
        for (int i = 0; i < era_length; i++)
        {
            vector<int> new_permutation = neighborhood_permutation(permutation);
            int new_cost = cost_of_permutation(new_permutation);
            int delta = new_cost - cost;
            if (delta < 0)
            {
                permutation = new_permutation;
                cost = new_cost;
            }
            else
            {
                random_device rd;
                mt19937 gen(rd());
                uniform_real_distribution<> dis(0, 1);

                float s = dis(gen);
                if (s < exp(-delta / current_temp))
                {
                    permutation = new_permutation;
                    cost = new_cost;
                }
            }
        }
        current_temp = new_temp_geo(current_temp, alpha);
    }


    permutation.insert(permutation.begin(), 0);
    permutation.push_back(0);
    return make_pair(permutation, cost);
}

int main()
{
    load_config();
    if (tasks.size() == 0)
    {
        std::cout << "No tasks found to be performed." << endl;
    }
    else
    {
        for (long unsigned int i = 0; i < tasks.size(); i++)
        {
            std::cout << endl
                      << "##################################################" << endl
                      << endl;
            string graph_file_name = tasks[i][0];
            int number_of_repeats = stoi(tasks[i][1]);
            string shortest_path_weight = tasks[i][2];
            string shortest_path = tasks[i][3];
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << graph_file_name << " file." << endl;
            }
            std::cout << "Computing TSP in " << graph_file_name << " graph repeated " << number_of_repeats << " times" << endl;
            if (number_of_current_graph_vertices < 1)
            {
                std::cout << "Cannot execute task. The array must to have at least 1 element.";
            }
            else if (number_of_repeats < 1)
            {
                std::cout << "Cannot execute task. The minimum number of repetitions is 1.";
            }
            else
            {
                pair<vector<int>, int> answer;
                high_resolution_clock::time_point t_start = high_resolution_clock::now();
                for (int j = 0; j < number_of_repeats; j++)
                {
                    answer = TSP_solve();
                }
                high_resolution_clock::time_point t_end = high_resolution_clock::now();
                duration<double> time_span = duration_cast<duration<double>>(t_end - t_start);
                int weight = answer.second;
                string path = "";
                std::vector<int>::iterator it = answer.first.begin();
                while (it != answer.first.end())
                {
                    path += to_string(*it);
                    path += " ";
                    it++;
                }
                ltrim(path);
                rtrim(path);
                cout << "Calculated shortest path: " << path << endl
                     << "Defined shortest path:    " << shortest_path << endl
                     << "Calculated weight: " << weight << endl
                     << "Defined weight:    " << shortest_path_weight << endl
                     << "Time: " << ((double)time_span.count() / (double)number_of_repeats) << " s" << endl;
                Result result = Result(graph_file_name, path, weight, time_span.count(), number_of_repeats);
                results.push_back(result.toString());
            }
        }
    }
    std::cout << endl;
    save_results(results_file_name);
    std::cout << "Press any key to continue...";
    getchar();
    return 0;
}