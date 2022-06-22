#include <stdio.h>
#include <stdlib.h>

#include "list.h"

// -----------------Node functions

//creates a new link, that contains the value specified in the argument
//and that points to next_in
nodePT new_node(int value_in, nodePT next_in) {
    nodePT result = (nodePT)malloc(sizeof(struct node));
    result->data = value_in;
    result->next = next_in;

    return result;
}

// -------------------- LIST functions
//List implementation uses a DUMMY NODE

//Creates and returns an empty list with a dummy node
nodePT new_list() {
    nodePT dummy = new_node(0, NULL);

    return dummy;
}

//Deallocates memory for all nodes in the list and the list object itself
void destroy_list(nodePT Ld) {
    nodePT next;
    nodePT current = Ld;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    //when does it free the list object itself or is the dummy node the list object itself???
}

//assumes there is a dummy node
nodePT get_first(nodePT L) {
    if (L == NULL) {
        return NULL;
    }

    return L->next;
}

//Inserts new node after the node called "previous"
void insert_node(nodePT previous, nodePT new_node) {
    if (previous == NULL) {
        printf("\nCannot insert after a NULL node. No action taken.\n");
    }
    else {
        new_node->next = previous->next;
        previous->next = new_node;
    }
}

//Inserts new node at the beginning of the list L
//Assumes there is a dummy node
void insert_at_beginning(nodePT L, nodePT new_node) {
    insert_node(L, new_node);
}

void delete_by_data(nodePT L, int data) {
    if (L == NULL) {
        return;
    }

    //nodePT current = L->next;
    nodePT curr = L->next;
    nodePT prev = L;

    //assums L has a dummy node
    while (curr != NULL) {

        if (curr->data == data) {
            prev->next = curr->next;
            free(curr);
            break;
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
}

//Returns 0 for empty lists (that have just one dummy node) and NULL lists
int compute_length(nodePT Ld) {
    if (Ld == NULL) {
        return 0;
    }

    int counter = 0;
    nodePT curr;
    for (curr = Ld->next; curr != NULL; curr = curr->next) {
        counter++;
    }

    return counter;
}

void print_list_vert(nodePT my_list) {
    if (my_list == NULL) {
        printf("\n<print_list> : list is NULL\n");
        return;
    }

    int i = 0;
    nodePT curr;
    printf("\nList items:\n");
    for (i = 0, curr = my_list->next; curr != NULL; curr = curr->next) {
        printf("item %d: %d\n", i, curr->data);
        i++;
    }

    printf("Length of above list = %d\n", i);
}

void print_list_horiz(nodePT my_list) {
    if (my_list == NULL) {
        printf("\n<print_list_horiz> : List is NULL\n");
        return;
    }

    int i = 0;
    nodePT curr;

    printf("\nList of items: ");
    for (i = 0, curr = my_list->next; curr != NULL; curr = curr->next) {
        printf("%5d     ", curr->data);
        i++;
    }

    printf("\nLength of above list = %d\n", i);
}

void print_list_pointer(nodePT my_list) {
    if (my_list == NULL) {
        printf("\n<print_list_pointer> : List is NULL\n");
        return;
    }

    int i = 0;
    nodePT curr;
    printf("\nList items:     \n");
    for (i = 0, curr = my_list->next; curr != NULL; curr = curr->next) {
        printf("%-11d ", curr->data);
        i++;
    }

    printf("\nList pointers: ");
    for (i = 0, curr = my_list->next; curr != NULL; curr = curr->next) {
        printf("%p ", curr);
        i++;
    }
    printf("\nLength of above list = %d\n", i);
}
