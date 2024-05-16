#pragma once
typedef struct node* nodePT;

struct node {
    int data;
    nodePT next;
};

nodePT build_list_of_ints();
nodePT array_2_list(int arr[], int sz);

//creates a new node, that contains the value specified in the argument,
//and that points to next_in
nodePT new_node(int value_in, nodePT next_in);

/* ------------- LIST  */
//List implementation uses a DUMMY NODE

//creates and returns an empty list
nodePT new_list();

void destroy_list(nodePT Ld);

//returns the pointer to the first node with data. (NOT a copy of that node)
//returns NULL if list is empty or NULL
nodePT get_first(nodePT L);

//Iterates through list and counts nodes
int compute_length(nodePT Ld);

//Inserts new_node at the beginning of thee list
//Assumes there is a dummy node
void insert_at_beginning(nodePT L, nodePT new_node);

void print_list_vert(nodePT my_list);

void print_list_horiz(nodePT my_list);

void print_list_pointer(nodePT my_list);
