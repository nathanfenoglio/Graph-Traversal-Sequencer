#pragma once
#include "list.h"

#include <stdlib.h>
#include <stdio.h>

typedef struct struct_graph* graphPT;

//creates a graph with the specified number of vertices
graphPT newGraph(int N, int undirected);

//deallocates the memory allocated to graph g
void destroyGraph(graphPT g);

//returns the number of vertices in g
int numVertices(graphPT g);

//connects vertices v1 and v2 in the graph
//returns 1 if successful, 0 if errors occurred
int addEdge(graphPT g, int v1, int v2);

//disconnects vertices v1 and v2 in the graph
//returns 1 if successful, 0 if errors occurred
int removeEdge(graphPT g, int v1, int v2);

//returns 1 if the specified edge exists, 0 otherwise
int edgeExists(graphPT g, int v1, int v2);

//returns an array with the neighbors of vertex v in graph g
//Sets res_size to the size of the returned array
int* vertexNeighbors(graphPT g, int v, int* res_size);

//prints the graph, namely the vertices, and the neighbors of each vertex
void printGraph(graphPT g);

//prints the neighbors of vertex v
void printNeighbors(graphPT g, int v);

//returns the pointer to the adjacency matrix of g
int** getAdjacencyMatrix(graphPT g);

//return 2D array of all vertexNeighbors of the graph with -1 filled in at end of each row
int** node_neighbors_2D_array(graphPT g);

void print_1D_int_array(int the_array[], int size);

void just_print_N_from_graph(graphPT g);

struct queue;

struct queue* createQueue();

//Check if the queue is empty
int isEmpty(struct queue* q);

void enqueue(struct queue* q, int value);

//removing elements from the queue
int dequeue(struct queue* q);

int remove_tail(struct queue* q);

//print the queue
void printQueue(struct queue* q);

//void printQueueWithMappedVals(struct queue* q, int mapped_vals[], FILE* outgoing);
std::string printQueueWithMappedVals(struct queue* q, std::vector <int> mapped_vals, FILE* outgoing);

//YOU CAN ACTUALLY SPECIFY THE START NODE IN graph_main.c DON'T HAVE TO JUST START ON 0
//I BELIEVE THEY ARE ALL THE SAME THOUGH JUST CYCLIC PERMUTATIONS OR WHATEVER
//modified parameters for IMGUI application
void printAllHamiltonianCyclesFromNodeZero(struct queue* q, std::vector <int> mapped_vals, graphPT g, std::vector <std::string>& all_hamiltonians_strings);

//depth first search implementation
//the inner part called from within DFS
//modified for IMGUI application
void DFS_visit(graphPT the_graph, int vertex, int* time, std::vector <int>& color_of_vertices, std::vector <int>& predecessors, std::vector <int>& start_times, std::vector <int>& finish_times, int* cycle, struct queue* a_path_to_a_dead_end);

//the outer part that calls DFS_visit for each of the vertices (if not visited by another vertex path)
//stores the arrays needed outside of the inner function call
//modified for IMGUI application
void DFS(graphPT the_graph, std::vector <int>& color_of_vertices, std::vector <int>& predecessors, std::vector <int>& start_times, std::vector <int>& finish_times, int* cycle);

//need a new array of the same size as array that holds the data to hold the ordered indices that point to the data records
//modified for IMGUI application
void insertionSortIndirect(std::vector <int>& arr, std::vector <int>& arr_indirect, int n);

//modified for IMGUI application
void print_all_possible_paths_inner(graphPT the_graph, std::vector <int>& predecessor_arr, struct queue* the_q, int current_index, int already_explored_in_current_context, int ok_to_print, std::vector <int>& mapped_vals, std::vector <int>& visited_already, int* num_paths, FILE* outgoing, int hamiltonians_instead, std::vector <std::string>& all_possible_paths_strings);

//parameter for hamiltonians instead of everything
//modified for IMGUI application
void print_all_possible_paths_outer(graphPT the_graph, int start_index, std::vector <int>& mapped_vals, int hamiltonians_instead, std::vector <std::string>& all_possible_paths_strings);

//modified for IMGUI application
void BFS(graphPT the_graph, int startVertex, std::vector <int>& color_of_vertices, std::vector <int>& predecessors, std::vector <int>& distances, std::vector <int>& mapped_vals, std::vector <int>& order_visited);

//yea, this works all the way to 20! which is as large as the computer can represent it seems
unsigned long long factorial(unsigned long long a);

//since you can figure out the # of combinations, can pass as parameter
//then able to malloc the correct amount of space for the 2D array to return
int** n_choose_r(int arr[], int arr_size, int r, int num_combinations);

//let's try a struct per n choose r set of combinations, 
//with the number of combinations in each set stored as well
//and the structs stored in an array of structs
struct n_choose_r_combinations {
    int** combinations;
    int num_combinations;
};

void get_all_n_choose_r_combinations_not_0_or_n_though(int the_array[], int arr_size, std::vector <struct n_choose_r_combinations>& all_combinations);

//modified for IMGUI application
void find_largest_clique(graphPT g, std::vector < std::vector <int> >& all_cliques);

//modified for IMGUI application
void find_vertex_covers(graphPT g, std::vector <std::string>& all_vertex_covers);

//find Euler paths 
//modified for IMGUI application
//void find_euler_paths(graphPT g, int mapped_vals[]);
//void find_euler_paths(graphPT g, int mapped_vals[], int print_mapped_vals);
void find_euler_paths(graphPT g, std::vector <int> mapped_vals, int print_mapped_vals, std::vector <std::string>& all_euler_paths);

//get all factors of a #
void get_factors(int num, std::vector <int>& factors);
