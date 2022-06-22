//removing all print statements for IMGUI application
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <sstream>

#include "twoD_arrays.h"
#include "graph_matrix_6_functions.h"

#include "imgui.h"

//a graph where edges are recorded as a matrix
struct struct_graph {
    int undirected; //value 1 means it is undirected, value other than 1 means it is directed
    int N; //# of vertices
    int** E; //edges I guess with pointers to 2D arrays you need double pointer ** just because it's 2D...
};

//creates a graphPT with the specified # of vertices
graphPT newGraph(int N, int undirected) {
    graphPT result = (graphPT)malloc(sizeof(*result));   
    result->undirected = undirected;
    result->N = N;
    result->E = malloc2d(N, N);

    //initialize adjacencies to signify that at present the graph contains no edges
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            result->E[i][j] = 0; //no edge from i to j
        }
    }

    return result;
}

//deallocates the memory allocated to graph g
void destroyGraph(graphPT g) {
    if (g == NULL) {
        return;
    }

    free2d(g->E, g->N, g->N);
    free(g);
}

//returns the # of vertices in g
int numVertices(graphPT g) {
    if (g == NULL) {
        return 0;
    }

    return g->N;
}

//returns the # of edges in g...
//returns twice as many than you want if undirected graph
int numEdges(graphPT g) {
    int num_edges_found = 0;
    for (int i = 0; i < g->N; i++) {
        for (int j = 0; j < g->N; j++) {
            if (g->E[i][j] > 0) {
                num_edges_found++;
            }
        }
    }

    return num_edges_found;
}

//returns 1 if v is a valid # for a vertex of graph g
//this function does not have a declaration in graphs.h because using it internally
int isVertexValid(graphPT g, int v) {
    if ((v < 0) || (v >= g->N)) {
        return 0; //invalid # for a vertex
    }

    return 1;
}

//returns 1 if the specified edge exists, 0 otherwise
int edgeExists(graphPT g, int v1, int v2) {
    if (g == NULL) {
        return 0;
    }

    if (isVertexValid(g, v1) == 0) {
        return 0;
    }

    if (isVertexValid(g, v2) == 0) {
        return 0;
    }

    return g->E[v1][v2]; //it will return 0 if the edge matrix has 0 -> no edge between the 2 vertices. I think 1 if there is an edge connection between the 2 vertices
}

//returns 1 if the specified edge exists, 0 otherwise
int edgeExistsInModifiedAdjMatrixPassedIn(graphPT g, int v1, int v2, int** modified_adj_matrix) {
    if (g == NULL) {
        return 0;
    }

    if (isVertexValid(g, v1) == 0) {
        return 0;
    }

    if (isVertexValid(g, v2) == 0) {
        return 0;
    }

    return modified_adj_matrix[v1][v2]; //it will return 0 if the edge matrix has 0 -> no edge between the 2 vertices. I think 1 if there is an edge connection between the 2 vertices
}

//connects vertices v1 and v2 in the graph
//returns 1 if successful, 0 if errors occurred
int addEdge(graphPT g, int v1, int v2) {
    if (g == NULL) {
        return 0;
    }

    if ((isVertexValid(g, v1) == 0) || (isVertexValid(g, v2) == 0)) {
        return 0;
    }

    g->E[v1][v2] = 1; //add edge from v1 to v2

    if (g->undirected == 1) {
        g->E[v2][v1] = 1; //add edge from v2 to v1
    }

    return 1;
}

//disconnects vertices v1 and v2 in the graph
//returns 1 if successful, 0 if errors occurred
int removeEdge(graphPT g, int v1, int v2) {
    if (g == NULL) {
        return 0;
    }

    if (isVertexValid(g, v1) == 0) {
        return 0;
    }

    if (isVertexValid(g, v2) == 0) {
        return 0;
    }

    g->E[v1][v2] = 0;

    if (g->undirected == 1) {
        g->E[v2][v1] = 0;
    }

    return 1;
}

//returns an array with the neighbors of vertex v in graph g
//sets res_size to the size of the returned array
int* vertexNeighbors(graphPT g, int v, int* res_size) {
    if (g == NULL) {
        return NULL;
    }

    if (isVertexValid(g, v) == 0) {
        return NULL;
    }

    //has to count how much space to create, then later run through again to get which vertices are neighbours
    int count = 0;
    int i, n;
    for (n = 0; n < g->N; n++) {

        if (g->E[v][n] == 1) {
            count++;
        }
    }

    (*res_size) = count;
    int* res = (int*)malloc(count * sizeof(int));

    i = 0;
    for (n = 0; n < g->N; n++) {

        if (g->E[v][n] == 1) {
            res[i] = n;
            i++;
        }
    }

    return res;
}

//return 2D array of all vertexNeighbors of the graph with -1 filled in at end of each row
int** node_neighbors_2D_array(graphPT g) {
    if (g == NULL) {
        return NULL;
    }

    int** ret_2D_array = malloc2d(g->N, g->N);

    int i;
    for (i = 0; i < g->N; i++) {
        int how_many_neighbors = 0;
        int j;
        for (j = 0; j < g->N; j++) {
            if (g->E[i][j] == 1) {
                ret_2D_array[i][how_many_neighbors] = j;
                how_many_neighbors++;
            }
        }
        for (j = how_many_neighbors; j < g->N; j++) {
            ret_2D_array[i][j] = -1;
        }
    }
    return ret_2D_array;
}

//prints the graph, namely the vertices, and the neighbors of each vertex
//not printing anything to console in IMGUI application of this
void printGraph(graphPT g) {
    if (g == NULL) {
        return;
    }
}

//prints the neighbors of vertex v
//not printing anything to console in IMGUI application of this
void printNeighbors(graphPT g, int v) {
    if (g == NULL) {
        return;
    }

    if (isVertexValid(g, v) == 0) {
        return;
    }

    int i, neighbors_size;
    //vertexNeighbors returns a pointer to a malloced array
    int* neighbors = vertexNeighbors(g, v, &neighbors_size);

    free(neighbors);
}

//not printing anything to console in IMGUI application of this
void print_1D_int_array(int the_array[], int size) {
    //int i;
    //for (i = 0; i < size; i++) {
    //    //printf("%d ", the_array[i]);
    //}
    //printf("\n");
}

int** getAdjacencyMatrix(graphPT g) {
    return g->E;
}

//not printing anything to console in IMGUI application of this
void just_print_N_from_graph(graphPT g) {
    //printf("\n\ng->N: %d\n", g->N);
}

//moving queue stuff up here to be able to use in DFS perhaps...
struct queue {
    int items[5000]; //can you instead not hardcode???
    int front;
    int rear;
};

struct queue* createQueue() {
    struct queue* q = (queue*)malloc(sizeof(struct queue));
    q->front = -1;
    q->rear = -1;
    return q;
}

//Check if the queue is empty
int isEmpty(struct queue* q) {
    if (q->rear == -1) {
        return 1;
    }
    else {
        return 0;
    }
}

void enqueue(struct queue* q, int value) {
    if (q->rear == 5000 - 1) {
        //printf("\nQueue is Full!\n");
    }
    else {
        if (q->front == -1) {
            q->front = 0;
        }
        q->rear++; //in the 1st place: starts at -1, for the 1st enqueue both front and rear become 0th index

        q->items[q->rear] = value; //add to back of queue

    }

}

//removing elements from the queue
int dequeue(struct queue* q) {
    int item;
    if (isEmpty(q)) {
        item = -1;
    }
    else {
        item = q->items[q->front];
        q->front++;

        //basically just sliding the index of the queue to the back and when it crosses the backs index value, the queue is empty
        if (q->front > q->rear) {
            q->front = -1;
            q->rear = -1;
        }
    }

    return item;
}

//adding remove from back or queue
int remove_tail(struct queue* q) {
    int item;
    if (isEmpty(q)) {
        item = -1;
    }
    else {
        item = q->rear;
        //free(q->rear);//shouldn't you free memory though...
        q->rear--;

        if (q->front > q->rear) {
            q->front = -1;
            q->rear = -1;
        }
    }

    return item;

}

//print the queue
//not printing anything to console in IMGUI application of this
void printQueue(struct queue* q) {
    if (isEmpty(q)) {
        //printf("\nQueue is empty\n");
    }
    else {
        int i;
        for (i = q->front; i <= q->rear; i++) {
            //printf("%d, ", q->items[i]); //and hopefully items can just store the indices of the nodes
        }
    }
}

//print the queue with mapped values
//modified parameters for IMGUI application
std::string printQueueWithMappedVals(struct queue* q, std::vector <int> mapped_vals, FILE* outgoing) {
    std::string queue_with_mapped_vals_str = "";
    if (isEmpty(q)) {
        //printf("\nQueue is empty\n");
    }
    else {
        int i;
        for (i = q->front; i <= q->rear; i++) {
            std::ostringstream temp;
            temp << mapped_vals[q->items[i]];
            queue_with_mapped_vals_str = queue_with_mapped_vals_str + temp.str() + ", ";
        }
    }

    return queue_with_mapped_vals_str;
}

//YOU CAN ACTUALLY SPECIFY THE START NODE IN graph_main.c DON'T HAVE TO JUST START ON 0
//I BELIEVE THEY ARE ALL THE SAME THOUGH JUST CYCLIC PERMUTATIONS OR WHATEVER
//modified parameters for IMGUI application
void printAllHamiltonianCyclesFromNodeZero(struct queue* q, std::vector <int> mapped_vals, graphPT g, std::vector <std::string>& all_hamiltonians_strings) {
    if (isEmpty(q)) {
        //printf("\nQueue is empty\n");
    }
    else {
        int i;
        int counter = 0;
        std::vector <int> arr_to_print(g->N + 1);
        for (i = q->front; i <= q->rear; i++) {
            arr_to_print[counter] = q->items[i];
            counter++;
        }
        //looking for Hamiltonian cycles that start on node 0, 
        //you can actually specify the start node now doesn't have to be just 0
        if (q->items[q->front] == q->items[q->rear] && counter == g->N + 1) {
            //print_1D_int_array(arr_to_print, g->N + 1); //not going to print to console in this application anyway, but will need to do something with arr_to_print...
            std::string arr_to_print_str = "";
            for (int i = 0; i < arr_to_print.size(); i++) {
                std::ostringstream temp;
                temp << arr_to_print[i];
                if (i == arr_to_print.size() - 1) {
                    arr_to_print_str = arr_to_print_str + temp.str();
                }
                else {
                    arr_to_print_str = arr_to_print_str + temp.str() + ", ";
                }
            }
            all_hamiltonians_strings.push_back(arr_to_print_str);

        }
    }
}

//depth first search implementation
//the inner part called from within DFS 
//modified parameters for IMGUI application
void DFS_visit(graphPT the_graph, int vertex, int* time, std::vector <int>& color_of_vertices, std::vector <int>& predecessors, std::vector <int>& start_times, std::vector <int>& finish_times, int* cycle, struct queue* a_path_to_a_dead_end) {
    (*time) = (*time) + 1; //increment counter
    start_times[vertex] = *time; //record currently considered vertex's start time
    color_of_vertices[vertex] = 1; //grey, mark node as visited but not completely explored (haven't explored all of its adjacent vertices)

    //explore adjacent vertices
    //THIS IS WHERE YOU WOULD SAVE TIME BY USING AN adjacency LIST INSTEAD OF MATRIX
    //BECAUSE WOULDN'T NEED TO GO THROUGH ALL OF THE VERTICES FOR EACH LOOP BELOW, JUST THE ONES ADJACENT INSTEAD
    //V^2 vs (V + E)
    int i;
    for (i = 0; i < the_graph->N; i++) {
        if (the_graph->E[vertex][i] == 1) {
            //IF YOU FIND A VERTEX THAT IS COLORED GREY IN THE ADJACENT VERTICES OF THE VERTEX YOU ARE EXPLORING
            //THEN YOU HAVE FOUND A CYCLE	
            if (color_of_vertices[i] == 1) {
                (*cycle) = 1;
            }
            else if (color_of_vertices[i] == 0) {
                //recursive call to explore the adjacencies of one of the vertices adjacent to this vertex 
                enqueue(a_path_to_a_dead_end, i);

                DFS_visit(the_graph, i, time, color_of_vertices, predecessors, start_times, finish_times, cycle, a_path_to_a_dead_end);
            }
        }
    }

    //color vertex black, have explored each adjacent node
    color_of_vertices[vertex] = 2;
    (*time) = (*time) + 1; //increment counter
    finish_times[vertex] = (*time); //record finish time for this vertex (all of the node's adjacencies have been explored and have returned from the various recursive calls)
}

//the outer part that calls DFS_visit for each of the vertices (if not visited by another vertex path)
//stores the arrays needed outside of the inner function call
//modified parameters for IMGUI application
//COULD YOU CHANGE TO BE ABLE TO START AT ANY INDEX NOT JUST 0???
void DFS(graphPT the_graph, std::vector <int>& color_of_vertices, std::vector <int>& predecessors, std::vector <int>& start_times, std::vector <int>& finish_times, int* cycle) {
    //initialize all veritices and corresponding array indices to default unvisited values
    int i;
    //let's add an array to save the visit order so can use if you want
    for (i = 0; i < the_graph->N; i++) {
        color_of_vertices[i] = 0; //initialize each vertex to 0 white unvisited
        predecessors[i] = -1; //no vertex has a predecessor yet, no vertex is labeled with a negative value, so signifying unexplored
        start_times[i] = -1; //time can't be negative, so start with a negative value to signify before the beginning of time
        finish_times[i] = -1;
    }

    int time = 0; //clear the clock, to get ready to start;

    //trying the queue thing, I don't think that you ever use just passing around for no reason now...
    struct queue* a_path_to_a_dead_end = createQueue();
    enqueue(a_path_to_a_dead_end, 0);
    //above sets up all initial values, now will actually do the traversing of the graph
    for (i = 0; i < the_graph->N; i++) {
        if (color_of_vertices[i] == 0) { //if vertex has not been visited by any path yet
            DFS_visit(the_graph, i, &time, color_of_vertices, predecessors, start_times, finish_times, cycle, a_path_to_a_dead_end);
        }
    }
}

//need a new array of the same size as array that holds the data to hold the ordered indices that point to the data records
//modified parameters for IMGUI application
void insertionSortIndirect(std::vector <int>& arr, std::vector <int>& arr_indirect, int n) {
    int i, key, j;

    //set up arr_indirect with values of index in order 0, 1, 2, 3, ... n -1
    for (i = 0; i < n; i++) {
        arr_indirect[i] = i;
    }

    for (i = 1; i < n; i++) {
        key = arr[i]; //key is holder to hold considered index value outside of the array
        j = i - 1; //j will be the index just below the considered index

        while (j >= 0 && arr[arr_indirect[j]] > key) {
            //you are just moving the index successively from the left to the right until the considered index is not greater than the key
            arr_indirect[j + 1] = arr_indirect[j];
            j = j - 1; //check the next to the left index
        }

        arr_indirect[j + 1] = i; //then insert the key into the now empty spot in the array
    }
}

//traverse and print every possible path
//modified parameters for IMGUI application
void print_all_possible_paths_inner(graphPT the_graph, std::vector <int>& predecessor_arr, struct queue* the_q, int current_index, int already_explored_in_current_context, int ok_to_print, std::vector <int>& mapped_vals, std::vector <int>& visited_already, int* num_paths, FILE* outgoing, int hamiltonians_instead, std::vector <std::string>& all_possible_paths_strings) {

    visited_already[current_index]++;

    //adding 8.17.20
    //going to change to -1 so that if you find a cycle you can hold the cycle vertex in the variable and use down the road
    int found_a_cycle = -1;
    int i;
    for (i = already_explored_in_current_context; i < the_graph->N; i++) {
        if (the_graph->E[current_index][i] == 1) {
            enqueue(the_q, i);
            ok_to_print = 1;

            if (visited_already[i] != 0) {
                found_a_cycle = i; //so that I can use the cycle vertex down the road
                break;
            }
            else {
                predecessor_arr[i] = current_index;
                current_index = i;
            }

            print_all_possible_paths_inner(the_graph, predecessor_arr, the_q, current_index, 0, ok_to_print, mapped_vals, visited_already, num_paths, outgoing, hamiltonians_instead, all_possible_paths_strings);

            //THIS RETURN IS WHAT YOU NEEDED TO NOT HAVE OTHER PATHS THAT DIDN'T START WITH STARTING INDEX PRINT!!!
            return;
        }
    }

    //exited for loop so implies dead end I think...
    if (ok_to_print && !isEmpty(the_q)) {
        //add branch to check for all Hamilonian cycles starting/ending on node 0
        //specified as parameter (0/1) to print_all_possible_paths_outer
        if (hamiltonians_instead) {
            //printAllHamiltonianCyclesFromNodeZero(the_q, mapped_vals, the_graph);
            printAllHamiltonianCyclesFromNodeZero(the_q, mapped_vals, the_graph, all_possible_paths_strings);
        }
        else {
            (*num_paths)++;

            std::string one_possible_path_string = printQueueWithMappedVals(the_q, mapped_vals, outgoing);
            all_possible_paths_strings.push_back(one_possible_path_string);
        }
    }

    remove_tail(the_q);

    if (!isEmpty(the_q)) {
        if (found_a_cycle != -1) {
            print_all_possible_paths_inner(the_graph, predecessor_arr, the_q, current_index, found_a_cycle + 1, 0, mapped_vals, visited_already, num_paths, outgoing, hamiltonians_instead, all_possible_paths_strings);
        }
        else {
            /*
            special scenario with visited already needed:
                mark visited_already for all nodes THAT ARE STILL IN THE QUEUE,
                mark unvisited all nodes NOT IN QUEUE.
                You won�t infinite loop because already_explored_in_current_context: current_index + 1,
            */
            //using modified code from printQueue function
            //1st set all visited_already to unvisiited 0
            int i;
            for (i = 0; i < the_graph->N; i++) {
                visited_already[i] = 0;
            }

            //set all vertices that are still in queue to visited
            if (isEmpty(the_q)) { //shouldn't be needed in this context as was already a precondition to be here
                //printf("\nQueue is empty\n");
            }
            else {
                //int i;
                for (i = the_q->front; i <= the_q->rear; i++) {
                    visited_already[the_q->items[i]] = 1;
                }
            }
            print_all_possible_paths_inner(the_graph, predecessor_arr, the_q, predecessor_arr[current_index], current_index + 1, 0, mapped_vals, visited_already, num_paths, outgoing, hamiltonians_instead, all_possible_paths_strings);
        }
    }
}

//modified parameters for IMGUI application
void print_all_possible_paths_outer(graphPT the_graph, int start_index, std::vector <int>& mapped_vals, int hamiltonians_instead, std::vector <std::string>& all_possible_paths_strings) {
    std::vector <int> predecessor_arr(the_graph->N);

    struct queue* the_q = createQueue();
    enqueue(the_q, start_index);

    std::vector <int> next_index_to_explore_arr(the_graph->N);

    for (int i = 0; i < the_graph->N; i++) {
        next_index_to_explore_arr[i] = 0;
    }

    //let's count the # of paths to output
    int num_paths = 0;

    //add visited_already array to check for cycles
    std::vector <int> visited_already(the_graph->N);

    int i;
    for (i = 0; i < the_graph->N; i++) { //mark all nodes as unvisited
        visited_already[i] = 0;
    }

    //should print all possible paths to text file...
    //NEED TO DITCH THE FILE IO STUFF FOR THIS APPLICATION...
    FILE* outgoing; //keeping for now because it's a parameter to print_all_possible_paths_inner
    outgoing = fopen("F:\\shell_script_messing\\SuperCollider_File_From_Graph_Data\\all_possible_paths_default_output_file.txt", "w");
    if (outgoing == NULL) {
        //printf("\nFailed to open file for whatever reason...\n");
    }

    print_all_possible_paths_inner(the_graph, predecessor_arr, the_q, start_index, 0, 0, mapped_vals, visited_already, &num_paths, outgoing, hamiltonians_instead, all_possible_paths_strings);
}

//modified parameters for IMGUI application
void BFS(graphPT the_graph, int startVertex, std::vector <int>& color_of_vertices, std::vector <int>& predecessors, std::vector <int>& distances, std::vector <int>& mapped_vals, std::vector <int>& order_visited) {
    struct queue* q = createQueue();

    color_of_vertices[startVertex] = 1; //grey, mark node as visited but not completely explored (haven't explored all of its adjacent vertices)
    enqueue(q, startVertex); //add start node to beginning of queue (and end at this point)

    //passing this vector in now
    //save an array of the order in which the nodes were visited...
    //std::vector <int> order_visited(the_graph->N);

    //initialize all initial values to -1, so if node is not able to be reached, then it has no order
    int a;
    for (a = 0; a < the_graph->N; a++) {
        order_visited[a] = -1;
    }
    int counter_order_visited = 0;

    while (!isEmpty(q)) {
        int currentVertex = dequeue(q);

        order_visited[counter_order_visited] = currentVertex;
        counter_order_visited++;

        //using adjacency list implementation since that's the path that you took...
        int i;
        for (i = 0; i < the_graph->N; i++) {
            if ((the_graph->E[currentVertex][i] == 1) && (color_of_vertices[i] == 0)) {
                color_of_vertices[i] = 1; //visit vertex

                enqueue(q, i);
            }
        }

    }
}

//yea, this works all the way to 20! which is large as the computer can represent it seems
unsigned long long factorial(unsigned long long a) {
    if (a > 20) {
        //printf("larger than 20! is too big to represent\n");
        return -1;
    }
    unsigned long long answer = 1;
    for (unsigned long long i = 1; i <= a; i++) {
        answer = answer * i;
    }
    return answer;
}

//since you can figure out the # of combinations, can pass as parameter
//then able to malloc the correct amount of space for the 2D array to return
int** n_choose_r(int arr[], int arr_size, int r, int num_combinations) {
    int* indexes_arr = (int*)malloc(r * sizeof(int));
    for (int i = 0; i < r; i++) {
        indexes_arr[i] = i;
    }

    //since you know how many combinations and the size of each combination (r)
    //can malloc the 2D array that you will be returning
    int** twoD_return_array = malloc2d(num_combinations, r);

    int index_counter = r - 1; //right wall

    int twoD_return_array_counter = 0;

    while (1) {
        //add indexes_arr to twoD_return_array
        for (int i = 0; i < r; i++) {
            twoD_return_array[twoD_return_array_counter][i] = arr[indexes_arr[i]]; //use the original array values, so arr[indexes_arr[i]]
        }

        twoD_return_array_counter++;
        indexes_arr[index_counter]++;

        if (indexes_arr[index_counter] == arr_size) {
            index_counter--;
            if (index_counter < 0) {
                break;
            }
            //find a spot where there will be enough room left to the right to fit valid #s
            int enough_space_to_right_for_higher_nums = 0;
            while (!enough_space_to_right_for_higher_nums) {
                if ((indexes_arr[index_counter] + r - 1 - index_counter) >= arr_size - 1) {
                    index_counter--;
                    if (index_counter < 0) {
                        break;
                    }
                }
                else {
                    enough_space_to_right_for_higher_nums = 1;
                }
            }
            if (!enough_space_to_right_for_higher_nums) {
                break;
            }
            //increment the index that will have enough space to its right 
            indexes_arr[index_counter]++;
            //set all indexes to the right to one higher than the index before
            index_counter++;
            while (index_counter < r) {
                indexes_arr[index_counter] = indexes_arr[index_counter - 1] + 1;
                index_counter++;
            }

            index_counter = r - 1; //rightest index
        }
    }

    free(indexes_arr);
    return twoD_return_array;
}
/*
//PUT IN graph.h FILE
//struct per n choose r set of combinations,
//with the number of combinations in each set stored as well
//and the structs stored in an array of structs
struct n_choose_r_combinations{
    int **combinations;
    int num_combinations;
};
*/
//pass array of n_choose_r_combinations structs FROM OUTSIDE to put all of the combinations into 
//declare outside like struct n_choose_r_combinations all_combinations[arr_size - 1];
//void get_all_n_choose_r_combinations_not_0_or_n_though(int the_array[], int arr_size, std::vector <struct n_choose_r_combinations> all_combinations) {
void get_all_n_choose_r_combinations_not_0_or_n_though(int the_array[], int arr_size, std::vector <struct n_choose_r_combinations>& all_combinations) {
    //getting all of power set except for n choose 0 and n choose n
    int i;
    for (int i = 1; i < arr_size; i++) {
        //recursive factorial function may be responsible for it breaking as it gets bigger...
        //you could maybe just try to precalculate the values of factorial up to whatever value and pull from there
        //also num_combinations could be an unsigned long long instead...
        //12! is the highest factorial that an int can handle
        unsigned long long num_combinations = factorial(arr_size) / (factorial(i) * factorial(arr_size - i));
        int** all_n_element_combinations = n_choose_r(the_array, arr_size, i, num_combinations);

        struct n_choose_r_combinations one_struct;
        one_struct.combinations = all_n_element_combinations;
        one_struct.num_combinations = num_combinations;
        all_combinations[i - 1] = one_struct;
    }

    //maybe should include the n member set (it's only one)...
}

//function to find largest clique
//it breaks at somewhere around more than 10 nodes
//WELL IT DEPENDS ON THE AMOUNT OF EDGES TOO, I'VE GOT 14 NODES THAT WERE ABLE TO BE SOLVED BEFORE
//AND THEN ADDED ONE EDGE AND IT BROKE, WELL DON'T ASK FOR DFS (COMMAND 6) AND YOU CAN GO FURTHER
//I'VE BEEN ABLE TO DO 16 NODES WITH DFS OFF
//Clique is a set of nodes that are all adjacent (one edge connects any other node to any other node in the set)
//take 2, just enumerating all possible combinations of all of the nodes of the graph
//then trying each one to see if completely connected (starting at biggest)
//if completely connected, check the rest of the combinations in that size of combination set to see if there is more than one possible biggest clique
void find_largest_clique(graphPT g, std::vector < std::vector <int> >& all_cliques) {
    //just create an array representing all of the nodes
    int* all_nodes = (int*)malloc(g->N * sizeof(int));
    for (int i = 0; i < g->N; i++) {
        all_nodes[i] = i;
    }

    std::vector <struct n_choose_r_combinations> all_combinations(g->N - 1);

    get_all_n_choose_r_combinations_not_0_or_n_though(all_nodes, g->N, all_combinations);

    //ok you have all possible combinations, now try them all 
    //(excluding all_combinations[0] because they are all single node sets)
    //I SUPPOSE YOU COULD STOP CHECKING HIGHER num_elements_in_combination_one_less ONCE YOU DON'T FIND ANY CLIQUES...
    int num_elements_in_combination_one_less;
    //for (num_elements_in_combination_one_less = 1; num_elements_in_combination_one_less < g->N; num_elements_in_combination_one_less++) {
    //changing to num_elements_in_combination_one_less < g->N - 1 gets rid of run time error
    //seems like the algorithm still accurate
    for (num_elements_in_combination_one_less = 1; num_elements_in_combination_one_less < g->N - 1; num_elements_in_combination_one_less++) {
        //for every combination of certain size
        for (int h = 0; h < all_combinations[num_elements_in_combination_one_less].num_combinations; h++) {
            int every_node_is_connected_to_every_other_node = 1;
            for (int i = 0; i < num_elements_in_combination_one_less + 1; i++) {
                for (int j = 0; j < num_elements_in_combination_one_less + 1; j++) {
                    if (i == j) {
                        continue;
                    }
                    if (!edgeExists(g, all_combinations[num_elements_in_combination_one_less].combinations[h][i], all_combinations[num_elements_in_combination_one_less].combinations[h][j])) {
                        every_node_is_connected_to_every_other_node = 0;
                    }
                }
            }
            if (every_node_is_connected_to_every_other_node) {
                //not printing anything in IMGUI application
                //NEED TO SAVE THE CLIQUE IN 2D INT VECTOR INSTEAD FOR IMGUI APPLICATION
                std::vector <int> clique_to_push_back;
                for (int i = 0; i < num_elements_in_combination_one_less + 1; i++) {
                    clique_to_push_back.push_back(all_combinations[num_elements_in_combination_one_less].combinations[h][i]);
                }
                all_cliques.push_back(clique_to_push_back);
            }
        }

    }

    //check if the entire graph is connected, checked all of the subgraphs already
    int every_node_is_connected_to_every_other_node = 1;
    std::vector <int> all_nodes_set_to_check(g->N); //visual studio won't let you get away with dynamically specifying the size of an array...

    for (int i = 0; i < g->N; i++) {
        all_nodes_set_to_check[i] = i;
    }
    for (int i = 0; i < g->N; i++) {
        for (int j = 0; j < g->N; j++) {
            if (i == j) {
                continue;
            }
            if (!edgeExists(g, all_nodes_set_to_check[i], all_nodes_set_to_check[j])) {
                every_node_is_connected_to_every_other_node = 0;
            }
        }
    }
    if (every_node_is_connected_to_every_other_node) {
        //not printing anything in IMGUI application
        //NEED TO SAVE THE CLIQUE IN 2D INT VECTOR INSTEAD FOR IMGUI APPLICATION
        std::vector <int> clique_to_push_back;
        for (int i = 0; i < g->N; i++) {
            clique_to_push_back.push_back(all_nodes_set_to_check[i]);
        }
        all_cliques.push_back(clique_to_push_back);
    }

    //FREE ALL OF THE 2D ARRAYS IN pointers_to_the_various_n_choose_r_arrays
    for (int i = 0; i < g->N - 1; i++) {
        free2d(all_combinations[i].combinations, all_combinations[i].num_combinations, i + 1); //freed2D does not take 2 args error???
    }

    free(all_nodes);
}

//doesn't check the set of every node, because that's obviously a vertex cover
//btw the independent set is the complement of the vertex cover, so should take note of that as well...
//vertex cover - set of vertices that includes at least one endpoint of every edge
void find_vertex_covers(graphPT g, std::vector <std::string>& all_vertex_covers) {
    //just create an array representing all of the nodes
    int* all_nodes = (int*)malloc(g->N * sizeof(int));
    for (int i = 0; i < g->N; i++) {
        all_nodes[i] = i;
    }

    std::vector <struct n_choose_r_combinations> all_combinations(g->N - 1); //visual studio won't let you get away with dynamically specifying the size of an array...

    get_all_n_choose_r_combinations_not_0_or_n_though(all_nodes, g->N, all_combinations);
    //ok you have all possible combinations, now try them all 

    //should eventually provide option for directed or not directed because it would matter if you 0d off all the diagonal symmetric 1s
    int directed = 0;
    //make copy of g->E 
    //allocate space for copy
    int** copy_adjacency_matrix = malloc2d(g->N, g->N);
    copy_matrix(g->E, g->N, g->N, copy_adjacency_matrix);
    //get 2 copies actually
    int** copy_adjacency_matrix_to_not_obliterate = malloc2d(g->N, g->N);
    copy_matrix(g->E, g->N, g->N, copy_adjacency_matrix_to_not_obliterate);

    //check each node in the combination
    //mark all nodes in copy_matrix as 0 for that row (and column if undirected)
    if (!directed) {
        int num_elements_in_combination_one_less;
        for (num_elements_in_combination_one_less = 0; num_elements_in_combination_one_less < g->N - 1; num_elements_in_combination_one_less++) {
            for (int h = 0; h < all_combinations[num_elements_in_combination_one_less].num_combinations; h++) {
                for (int i = 0; i < num_elements_in_combination_one_less + 1; i++) {
                    for (int j = 0; j < g->N; j++) { //walk down row marking 0s
                        //mark all rows at the current node that you're looking at as 0
                        copy_adjacency_matrix[all_combinations[num_elements_in_combination_one_less].combinations[h][i]][j] = 0;
                        //mark all columns at at the current node that you're looking as 0
                        copy_adjacency_matrix[j][all_combinations[num_elements_in_combination_one_less].combinations[h][i]] = 0;
                    }
                }

                //check if the copy_matrix is all 0s
                //if all in the copy of g->E are 0s, then the set is a vertex cover
                //if not, then it's not a vertex cover
                int all_zeros = 1;
                //think need to put all of the original values back in the copy array now since it is obliterated
                for (int i = 0; i < g->N; i++) {
                    for (int j = 0; j < g->N; j++) {
                        //checking if all_zeros at the same time as you recopy the original values
                        if (copy_adjacency_matrix[i][j] != 0) {
                            all_zeros = 0;
                        }
                        copy_adjacency_matrix[i][j] = copy_adjacency_matrix_to_not_obliterate[i][j];
                    }
                }

                if (all_zeros) { //found a vertex cover
                    std::string vertex_cover_str = "";
                    for (int i = 0; i < num_elements_in_combination_one_less + 1; i++) {
                        //not printing anything in IMGUI application
                        std::ostringstream temp;
                        temp << all_combinations[num_elements_in_combination_one_less].combinations[h][i];
                        vertex_cover_str = vertex_cover_str + temp.str() + ", ";
                    }
                    all_vertex_covers.push_back(vertex_cover_str);
                }
            }
        }
    }
    else {
        //directed graph stuff instead
        //just treats it as an undirected graph even if directed
        //directed isn't really valid unless every edge goes in both directions like an undirected graph
        //but it's fine to take note of it like this in this situation
    }

    //free the copy
    free2d(copy_adjacency_matrix, g->N, g->N);
    //free the other copy
    free2d(copy_adjacency_matrix_to_not_obliterate, g->N, g->N);

}

//find Euler paths for undirected graphs it's working 
//for the 1st starting node option when 2 nodes have odd degree
//and when all node degrees are even 

//KEEP GETTING RUN TIME EXCEPTION IN if(dead_end) CONDITIONAL...
//void find_euler_paths(graphPT g, int mapped_vals[], int print_mapped_vals) {
void find_euler_paths(graphPT g, std::vector <int> mapped_vals, int print_mapped_vals, std::vector <std::string>& all_euler_paths) {
    //create array for each vertex degree
    int* vertex_degree_arr = (int*)(malloc(sizeof(int) * g->N));

    //calculate each vertex degree, can use printNeighbors, 
    //will have side effect of printing the neighbors, 
    //but you pass in the address of an int that is changed to the number of neighbors, 
    //which is the same as the number of edges, 
    //so then check if odd or even
    for (int i = 0; i < g->N; i++) {
        int degree = 0;
        vertexNeighbors(g, i, &degree);
        vertex_degree_arr[i] = degree;
    }
    //just printing
    //print_1D_int_array(vertex_degree_arr, g->N);
    //determine # of odd degree vertices
    int num_odd_degree_vertices = 0;
    for (int i = 0; i < g->N; i++) {
        if (vertex_degree_arr[i] % 2 != 0) {
            num_odd_degree_vertices++;
        }
    }
    //just printing
    //printf("number odd degree vertices: %d\n", num_odd_degree_vertices);

    //copy adjacency matrix to remove already used edges as you go 
    int** adj_matrix_copy;

    //26 edges in euler_path_6_2.txt seems to be the limit of what the computer can handle
    //added 2 more edges and the program just quits
    int num_edges_in_graph = numEdges(g);
    //numEdges double counts for undirected graph so divide by 2
    num_edges_in_graph = num_edges_in_graph / 2;

    //MALLOC A MORE THAN YOU THINK YOU WILL NEED SIZE 2D ARRAY TO HOLD ALL OF THE POTENTIAL EULER SEQUENCES
    //LATER JUST DECLARE HERE FOR ACCESS
    //int hopefully_more_than_enough_rows = 1000000;
    int hopefully_more_than_enough_rows = 50;

    //there will be one more node in sequence than there are edges because of the starting node
    int** euler_sequences = NULL;

    int current_euler_sequence_that_you_are_building = 0;

    int num_euler_sequences = 0;

    //JUST CHECK IF AN EULER PATH/CYCLE IS POSSIBLE AT ALL HERE
    if (!(num_odd_degree_vertices == 2 || num_odd_degree_vertices == 0)) {
        exit(0);
    }
    else { //not printing anything in IMGUI application...
        for (int i = 0; i <= g->N; i++) {
            //the end stopping condition (have explored starting on every valid starting node)
            //print the last euler_sequences and then stop
            if (i == g->N) {
                if (euler_sequences != NULL) {
                    //print the values mapped to the nodes or print the node #s 0, 1, 2, 3, ... 
                    if (print_mapped_vals) {
                        //printMatrixWithMappedValues(euler_sequences, current_euler_sequence_that_you_are_building, num_edges_in_graph + 1, mapped_vals);
                        copyMatrixWithMappedValuesToStringVector(euler_sequences, current_euler_sequence_that_you_are_building, num_edges_in_graph + 1, mapped_vals, all_euler_paths);
                    }
                    else {
                        //printMatrix(euler_sequences, current_euler_sequence_that_you_are_building, num_edges_in_graph + 1);
                    }
                    free2d(euler_sequences, hopefully_more_than_enough_rows, num_edges_in_graph + 1);
                    free2d(adj_matrix_copy, g->N, g->N);
                    //printf("\nnum_euler_sequences: %d\n", num_euler_sequences);
                    break;
                }
            }

            //a starting node in a graph with 2 odd degree vertices OR 
            //if all node degrees are even, then every node is a valid starting node
            if (vertex_degree_arr[i] % 2 != 0 || num_odd_degree_vertices == 0) {
                //if you are here and euler_sequences is not empty, 
                //then you just finished exploring all possible euler paths
                //starting from the previous node that you were exploring
                //so print all of the paths because you're about to free euler_sequences 
                //to fill with euler paths starting from the current i node 
                if (euler_sequences != NULL) {
                    //print the values mapped to the nodes or print the node #s 0, 1, 2, 3, ... 
                    if (print_mapped_vals) {
                        //printMatrixWithMappedValues(euler_sequences, current_euler_sequence_that_you_are_building, num_edges_in_graph + 1, mapped_vals);
                        copyMatrixWithMappedValuesToStringVector(euler_sequences, current_euler_sequence_that_you_are_building, num_edges_in_graph + 1, mapped_vals, all_euler_paths);
                    }
                    else {
                        //printMatrix(euler_sequences, current_euler_sequence_that_you_are_building, num_edges_in_graph + 1);
                    }

                    free2d(euler_sequences, hopefully_more_than_enough_rows, num_edges_in_graph + 1);
                    free2d(adj_matrix_copy, g->N, g->N);
                }

                current_euler_sequence_that_you_are_building = 0;

                //copy adjacency matrix to remove already used edges as you go 
                adj_matrix_copy = malloc2d(g->N, g->N);
                copy_matrix(g->E, g->N, g->N, adj_matrix_copy);

                //there will be one more node in sequence than there are edges because of the starting node
                //MALLOC A MORE THAN YOU THINK YOU WILL NEED SIZE 2D ARRAY TO HOLD ALL OF THE POTENTIAL EULER SEQUENCES
                euler_sequences = malloc2d(hopefully_more_than_enough_rows, num_edges_in_graph + 1);
                //set all to -1 to signify no node at that spot in the sequence
                for (int a = 0; a < hopefully_more_than_enough_rows; a++) {
                    for (int b = 0; b < num_edges_in_graph + 1; b++) {
                        euler_sequences[a][b] = -1;
                    }
                }


                euler_sequences[0][0] = i; //i will be the start node since it fulfills the % 2 != 0 criteria
                //stopping condition will be when have backtracked all the way back to
                //euler_sequence[0] and there are no available edges to travel 
                //that are not have not already been explored for the node at euler_sequence[0]
                int current_considered_index = 0;
                int a_node_that_you_found_yourself_in_a_dead_end_from_exploring_last_time = -1;
                int stop_after_this_many_times_for_testing = 0;
                while (current_considered_index >= 0 && current_considered_index < num_edges_in_graph) {
                    //check to see if no possible node is able to be traveled to dead end
                    int dead_end = 1;
                    for (int a = 0; a < g->N; a++) {
                        //if you found a dead end last time through and you're coming back through here again
                        //need to set a to one higher than it was when you dead ended on the last node that you tried
                        if (a_node_that_you_found_yourself_in_a_dead_end_from_exploring_last_time >= 0) { //it's -1 if not set from a dead end
                            a = a_node_that_you_found_yourself_in_a_dead_end_from_exploring_last_time + 1;
                            a_node_that_you_found_yourself_in_a_dead_end_from_exploring_last_time = -1;
                        }

                        if (edgeExistsInModifiedAdjMatrixPassedIn(g, euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index], a, adj_matrix_copy)) {
                            int dupe_checking_counter = current_considered_index + 1;
                            int previous_euler_sequences_counter = current_euler_sequence_that_you_are_building - 1; //didn't already check the most recent euler sequence 

                            //SO IF IT'S THE VERY FIRST EULER SEQUENCE THAT YOU ARE WORKING ON
                            //THEN YOU NEED TO IGNORE THE DUPE CHECKING (BECAUSE THERE IS NO PREVIOUS EULER SEQUENCE)
                            //AND SO JUST PAY ATTENTION TO THE adj_matrix_copy
                            if (previous_euler_sequences_counter < 0) {
                                adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]][a] = 0;
                                adj_matrix_copy[a][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]] = 0;
                                current_considered_index++;
                                euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index] = a;
                                dead_end = 0;
                                break; //break out of for loop because valid edge found
                            }
                            //check for dupe seq in previously found euler sequences
                            else { //there are previous euler sequences, so better check for dupe
                                int no_dupes_go_ahead_and_add_node = 1;
                                while (previous_euler_sequences_counter >= 0) {
                                    if (euler_sequences[previous_euler_sequences_counter][dupe_checking_counter] == a) {
                                        //need to check all the way back to the beginning of sequence
                                        dupe_checking_counter--;
                                        int non_dupe_element_found = 0;
                                        while (dupe_checking_counter >= 0) {
                                            //unless you find that there is a difference of nodes in the sequence further back to the left
                                            //then go ahead and stop checking this previous euler sequence and check the next to check
                                            if (euler_sequences[previous_euler_sequences_counter][dupe_checking_counter] != euler_sequences[current_euler_sequence_that_you_are_building][dupe_checking_counter]) {
                                                //not a dupe then
                                                dupe_checking_counter = current_considered_index + 1; //set back to the index that you are looking to add a node at to start checking the next sequence for a dupe
                                                previous_euler_sequences_counter--;
                                                non_dupe_element_found = 1;
                                                break; //check next previous euler sequence 
                                            }
                                            dupe_checking_counter--;
                                        }

                                        //if you get here and you didn't encounter a non_dupe_element and broke out of while loop, 
                                        //then there was a dupe found
                                        if (!non_dupe_element_found) {
                                            no_dupes_go_ahead_and_add_node = 0;
                                            break;
                                        }

                                    }
                                    //check the next closest previous successful euler sequence to see if a dupe
                                    previous_euler_sequences_counter--;
                                }

                                if (no_dupes_go_ahead_and_add_node) {
                                    euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index + 1] = a;

                                    //mark the edge as explored
                                    adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]][a] = 0;
                                    adj_matrix_copy[a][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]] = 0;
                                    current_considered_index++; //advance to find next node to add

                                    dead_end = 0;
                                    break;
                                }
                            }

                        }
                    }
                    if (dead_end) {
                        //save the node that you were at when you got stuck so that you can start a in the for loop at the next node
                        a_node_that_you_found_yourself_in_a_dead_end_from_exploring_last_time = euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index];

                        //remove the edge (as in make it available again to travel on) from the current_considered_index to the previous node from the adj_matrix_copy
                        //EXCEPTION HERE CURRENTLY...
                        adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index - 1]] = 1;
                        adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index - 1]][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]] = 1;

                        //set the current entry in the array to -1 and current_considered_index--
                        euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index] = -1;

                        //move back to previous node before the current one in the current euler sequence that you're working on
                        current_considered_index--;
                    }
                    //all edges have been traversed so save it and backtrack
                    if (current_considered_index == num_edges_in_graph) {
                        num_euler_sequences++;
                        //save current sequence in the giant 2D array euler_sequences						
                        //copy the current euler sequence success to the row below to backtrack with
                        for (int a = 0; a < num_edges_in_graph + 1; a++) {
                            euler_sequences[current_euler_sequence_that_you_are_building + 1][a] = euler_sequences[current_euler_sequence_that_you_are_building][a];
                        }

                        //go to the next row to start backtracking and trying to build the next euler sequence
                        current_euler_sequence_that_you_are_building++;

                        //BACKTRACK
                        int have_only_backtracked_so_far = 1;
                        //don't want to mess with index 0 because it will be incremented after leaving this backtracking block
                        //so just current_considered_index > 0
                        while (have_only_backtracked_so_far && current_considered_index > 0) {
                            //set the last edge that you are backtracking on to valid (unexplored)
                            adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index - 1]] = 1;
                            adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index - 1]][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]] = 1;

                            euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index] = -1; //-1 signifying haven't filled in any node yet
                            current_considered_index--; //look at the node 1 back
                            for (int a = 0; a < g->N; a++) {
                                //check for valid edge that has not already been explored in previous successful euler sequences
                                if (edgeExistsInModifiedAdjMatrixPassedIn(g, euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index], a, adj_matrix_copy)) {
                                    //check if node that you are considering traveling to is in the same array position as the MOST RECENT successful euler sequence
                                    //if it is and you've done nothing but backtrack so far, you know all other nodes to the left will be the same since you copied them all to the current one that you're building 
                                    //and haven't affected any change to the left of current_considered_index
                                    if (euler_sequences[current_euler_sequence_that_you_are_building - 1][current_considered_index + 1] == a) {
                                        continue;
                                    }
                                    else {
                                        //check all nodes in previous successful euler sequences at the index that you are looking to add
                                        int dupe_checking_counter = current_considered_index + 1;
                                        int previous_euler_sequences_counter = current_euler_sequence_that_you_are_building - 2; //already checked the most recent euler sequence 
                                        int no_dupes_go_ahead_and_add_node = 1;
                                        int non_dupe_element_found = 0;
                                        while (previous_euler_sequences_counter >= 0) {
                                            if (euler_sequences[previous_euler_sequences_counter][dupe_checking_counter] == a) {
                                                //need to check all the way back to the beginning of sequence
                                                dupe_checking_counter--;
                                                while (dupe_checking_counter >= 0) {
                                                    //unless you find that there is a difference of nodes in the sequence further back to the left
                                                    //then go ahead and stop checking this previous euler sequence and check the next to check
                                                    if (euler_sequences[previous_euler_sequences_counter][dupe_checking_counter] != euler_sequences[current_euler_sequence_that_you_are_building][dupe_checking_counter]) {
                                                        //not a dupe then
                                                        non_dupe_element_found = 1;
                                                        //set back to the index that you are looking to add a node at to start checking the next sequence for a dupe
                                                        dupe_checking_counter = current_considered_index + 1;
                                                        break; //check next previous euler sequence 
                                                    }
                                                    dupe_checking_counter--;
                                                }

                                                //if you get here and didn't find a non_dupe_element as you checked all the way back to the beginning, 
                                                //then there was a dupe found
                                                if (!non_dupe_element_found) {
                                                    no_dupes_go_ahead_and_add_node = 0;
                                                    break;
                                                }

                                            }

                                            previous_euler_sequences_counter--; //check the next closest previous successful euler sequence to see if a dupe
                                        }

                                        if (no_dupes_go_ahead_and_add_node) {
                                            euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index + 1] = a;
                                            //mark the edge as explored
                                            adj_matrix_copy[euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]][a] = 0;
                                            adj_matrix_copy[a][euler_sequences[current_euler_sequence_that_you_are_building][current_considered_index]] = 0;

                                            current_considered_index++; //advance to find next node to add

                                            have_only_backtracked_so_far = 0;
                                            stop_after_this_many_times_for_testing++;
                                            break;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        free(vertex_degree_arr);
    }
}

void get_factors(int num, std::vector <int>& factors) {
    for (int a = 2; a < num; a++) {

        if ((num % a) == 0) {
            factors.push_back(a);
        }
    }
}

