#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <random>
#include <string>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <iomanip>
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
    string calculated_path;
    string defined_path;
    int calculated_path_weight;
    int defined_path_weight;
    double time;
    int number_of_repeats;
    float alpha;
    float b;
    int era_length;
    string cooling_method;
    string neighborhood_method;
    Result(string graph_name, string calculated_path, int calculated_path_weight, string defined_path, int defined_path_weight, double time, int number_of_repeats, float alpha, float b, int era_length, string cooling_method, string neighborhood_method)
    {
        this->graph_name = graph_name;
        this->calculated_path = calculated_path;
        this->calculated_path_weight = calculated_path_weight;
        this->defined_path = defined_path;
        this->defined_path_weight = defined_path_weight;
        this->time = time;
        this->number_of_repeats = number_of_repeats;
        this->alpha = alpha;
        this->b = b;
        this->era_length = era_length;
        this->cooling_method = cooling_method;
        this->neighborhood_method = neighborhood_method;
    }
    string toString()
    {
        return (graph_name + "," + calculated_path + "," + to_string(calculated_path_weight) + "," + defined_path + "," + to_string(defined_path_weight) + "," + to_string(time) + "," + to_string(number_of_repeats) + "," + to_string(alpha) + "," + to_string(b) + "," + to_string(era_length) + "," + cooling_method + "," + neighborhood_method);
    }
};

void save_results(string results_file_name)
{
    std::cout << "Saving results" << endl;
    fstream fout;
    fout.open(results_file_name, ios::out);
    fout << "graph_name,calculated_path,calculated_path_weight,defined_path,defined_path_weight,time,number_of_repeats,alpha,b,era_length,cooling_method,neighborhood_method" << endl;
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
    // current_graph_adjacency_matrix.print();
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
    ltrim(results_file_name);
    rtrim(results_file_name);
    cout << "Loaded result file name: " << results_file_name << endl;
    while (getline(fin, loaded_line_of_task))
    {
        vector<string> single_line = split(loaded_line_of_task, ' ');
        std::vector<std::string>::iterator it = single_line.begin();
        while (it != single_line.end())
        {
            if (it->length() == 0)
            {
                it = single_line.erase(it);
            }
            else
            {
                ltrim(*it);
                rtrim(*it);
                ++it;
            }
        }
        string graph_file_name, number_of_repeats, alpha, b, era_length, cooling_method, neighborhood_method, shortest_path_weight, shortest_path;
        if (single_line.size() >= 8)
        {
            graph_file_name = single_line[0];
            number_of_repeats = single_line[1];
            alpha = single_line[2];
            b = single_line[3];
            era_length = single_line[4];
            cooling_method = single_line[5];
            neighborhood_method = single_line[6];
            shortest_path_weight = single_line[7];
            for (long unsigned int i = 8; i < single_line.size(); i++)
            {
                shortest_path += single_line[i];
                shortest_path += " ";
            }
        }
        if (graph_file_name.size() == 0 || number_of_repeats.size() == 0 || alpha.size() == 0 || b.size() == 0 || era_length.size() == 0 || cooling_method.size() == 0 || neighborhood_method.size() == 0 || shortest_path_weight.size() == 0)
        {
            std::cout << "Cannot load this task: " << loaded_line_of_task << endl;
        }
        else
        {
            vector<string> task;
            task.push_back(graph_file_name);
            task.push_back(number_of_repeats);
            task.push_back(alpha);
            task.push_back(b);
            task.push_back(era_length);
            task.push_back(cooling_method);
            task.push_back(neighborhood_method);
            task.push_back(shortest_path_weight);
            task.push_back(shortest_path);
            tasks.push_back(task);
            cout << "Correclty loaded task: ";
            for (long unsigned int i = 0; i < task.size(); i++)
                cout << task[i] << " ";
            cout << endl;
        }
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

float initial_temperature(int divider_of_quantity = 5)
{
    int sum_of_costs = 0;
    random_device rd;
    default_random_engine rng(rd());
    vector<int> prev_permutation;
    for (int i = 1; i < number_of_current_graph_vertices; i++)
        prev_permutation.push_back(i);
    shuffle(prev_permutation.begin(), prev_permutation.end(), rng);
    for (int i = 0; i < number_of_current_graph_vertices / divider_of_quantity; i++)
    {
        vector<int> permutation;
        for (int j = 1; j < number_of_current_graph_vertices; j++)
            permutation.push_back(j);
        shuffle(permutation.begin(), permutation.end(), rng);
        sum_of_costs += abs(cost_of_permutation(prev_permutation) - cost_of_permutation(permutation));
        prev_permutation = permutation;
    }
    return sum_of_costs / (number_of_current_graph_vertices / divider_of_quantity);
}

vector<int> initial_permutation()
{
    vector<int> permutation;
    for (int i = 1; i < number_of_current_graph_vertices; i++)
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

vector<int> neighborhood_permutation_swap(vector<int> permutation)
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

vector<int> neighborhood_permutation_invert(vector<int> permutation)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0, 1);

    int first_index = (int)(dis(gen) * 10000) % permutation.size();
    int second_index = (int)(dis(gen) * 10000) % permutation.size();

    if (first_index == second_index)
        return permutation;
    if (first_index > second_index)
    {
        int tmp = first_index;
        first_index = second_index;
        second_index = tmp;
    }
    while (first_index < second_index)
    {
        permutation = swap_permutation(permutation, first_index, second_index);
        first_index++;
        second_index--;
    }
    return permutation;
}

float new_temp_geo(float temp, float alpha)
{
    return temp * alpha;
}

float new_temp_Boltzmann(float temp, float alpha, float b, int k)
{
    return temp / (alpha + b * log(k));
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
 * @param b cooling parameter (for Boltzmann method)
 * @param era_length number of iterations to find the best solution for a given temperature
 * @param cooling_method 0 - geometrical cooling | 1 - Boltzmann
 * @param neighborhood_method 0 - swap | 1 - invert
 * @return pair of calculated path and weight
 */
pair<vector<int>, int> TSP_solve(float alpha = 0.999, float b = 1, int era_length = 100, bool cooling_method = false, bool neighborhood_method = true)
{
    vector<int> permutation = initial_permutation();
    int cost = cost_of_permutation(permutation);
    int prev_cost = INT32_MAX;
    // float min_temp = 0.000001; //minimal temperature - stop condition
    float current_temp = initial_temperature();
    int same_cost_counter = 0;
    int iteration_counter = 0;
    while (same_cost_counter < 50)
    // while (current_temp > min_temp && same_cost_counter < 50)
    {
        iteration_counter++;
        if (prev_cost == cost)
            same_cost_counter++;
        else
            same_cost_counter = 0;
        prev_cost = cost;
        cout << "Current temp:" << std::right << std::setw(12) << current_temp << "| Cost: " << std::right << std::setw(8) << cost << "\t\r" << std::flush;
        for (int i = 0; i < era_length; i++)
        {
            vector<int> new_permutation;
            if (!cooling_method)
                new_permutation = neighborhood_permutation_swap(permutation);
            else
                new_permutation = neighborhood_permutation_invert(permutation);
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
        if (!cooling_method)
            current_temp = new_temp_geo(current_temp, alpha);
        else
            current_temp = new_temp_Boltzmann(current_temp, alpha, b, iteration_counter);
    }
    permutation.insert(permutation.begin(), 0);
    permutation.push_back(0);
    return make_pair(permutation, cost);
}

int main()
{
    load_config();
    std::cout << "Tasks loaded.\nPress any key to continue...";
    getchar();
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
            float alpha = stof(tasks[i][2]);
            float b = stof(tasks[i][3]);
            int era_length = stoi(tasks[i][4]);
            bool cooling_method = true;
            if (tasks[i][5] == "geo")
                cooling_method = false;
            else if (tasks[i][5] == "boltzmann")
                cooling_method = true;
            bool neighborhood_method = true;
            if (tasks[i][6] == "swap")
                neighborhood_method = false;
            else if (tasks[i][6] == "invert")
                neighborhood_method = true;
            string shortest_path_weight = tasks[i][7];
            string shortest_path = tasks[i][8];
            ltrim(shortest_path);
            rtrim(shortest_path);
            if (!load_data(graph_file_name))
            {
                std::cout << "Cannot load graph from " << graph_file_name << " file." << endl;
            }
            std::cout << "Computing TSP in " << graph_file_name << " graph repeated " << number_of_repeats << " times" << endl
                      << "alpha: " << alpha << endl
                      << "era_length: " << era_length << endl
                      << "cooling method: " << tasks[i][5] << endl;
            if (cooling_method)
                cout << "b: " << b << endl;
            cout << "neighborhood method: " << tasks[i][6] << endl
                 << endl;
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
                for (int j = 0; j < number_of_repeats; j++)
                {
                    high_resolution_clock::time_point t_start = high_resolution_clock::now();

                    answer = TSP_solve(alpha, b, era_length, cooling_method, neighborhood_method);
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
                         << "Time: " << ((double)time_span.count() / (double)number_of_repeats) << " s" << endl
                         << "Task " << i+1 << " from " << tasks.size() << " | Repeat " << j+1 << " from " << number_of_repeats << endl<<endl;
                    Result result = Result(graph_file_name, path, weight, shortest_path, stoi(shortest_path_weight), time_span.count(), number_of_repeats, alpha, b, era_length, tasks[i][5], tasks[i][6]);
                    results.push_back(result.toString());
                }
            }
        }
    }
    save_results(results_file_name);
    std::cout << "Press any key to continue...";
    getchar();
    return 0;
}