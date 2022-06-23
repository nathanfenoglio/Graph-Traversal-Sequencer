#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>

int** malloc2d(int rows, int columns) {
    int row;

    //allocate space to keep a pointer for each row
    int** result = (int**)malloc(rows * sizeof(int*));

    //allocate the space for each row
    for (row = 0; row < rows; row++) {
        result[row] = (int*)malloc(columns * sizeof(int));
    }

    return result;
}

void free2d(int** array, int rows, int columns) {
    int row;

    //free the space for each row
    for (row = 0; row < rows; row++) {
        free(array[row]);
    }

    //free the space with the pointer to each row
    free(array);
}

void printMatrix(int** array, int rows, int columns) {
    int row, col;
    for (row = 0; row < rows; row++) {

        for (col = 0; col < columns; col++) {
            printf("%5d", array[row][col]);
        }
        printf("\n");
    }
    printf("\n");
}

void printMatrixWithMappedValues(int** array, int rows, int columns, int* mapped_vals) {
    //count # of mapped values to make sure same amount as # of distinct values that you want to map to
    //not using because need to know the amount of nodes in the graph
    //not the number of columns
    //could add that info later to check

    /*
    int i = 0;
    int num_mapped_vals = 0;
    while(mapped_vals[i] != -1){
        num_mapped_vals++;
        i++;
    }
    printf("num_mapped_vals: %d\n", num_mapped_vals);


    if(num_mapped_vals != columns){
        printf("\n# of mapped values is not the same as the # of distinct elements in array\n");
        return;
    }
    */
    int row, col;
    for (row = 0; row < rows; row++) {

        for (col = 0; col < columns; col++) {
            printf("%d, ", mapped_vals[array[row][col]]);
        }
        printf("\n");
    }
    printf("\n");

}

//version for IMGUI application to convert to string and add to a vector of strings
//void copyMatrixWithMappedValuesToStringVector(int** array, int rows, int columns, int* mapped_vals, std::vector <std::string> all_rows) {
void copyMatrixWithMappedValuesToStringVector(int** array, int rows, int columns, std::vector <int> mapped_vals, std::vector <std::string> all_rows) {
    //count # of mapped values to make sure same amount as # of distinct values that you want to map to
    //not using because need to know the amount of nodes in the graph
    //not the number of columns
    //could add that info later to check

    /*
    int i = 0;
    int num_mapped_vals = 0;
    while(mapped_vals[i] != -1){
        num_mapped_vals++;
        i++;
    }
    printf("num_mapped_vals: %d\n", num_mapped_vals);


    if(num_mapped_vals != columns){
        printf("\n# of mapped values is not the same as the # of distinct elements in array\n");
        return;
    }
    */
    int row, col;
    for (row = 0; row < rows; row++) {
        std::string one_row_str = "";
        for (col = 0; col < columns; col++) {
            //printf("%d, ", mapped_vals[array[row][col]]);
            std::ostringstream temp;
            temp << mapped_vals[array[row][col]];
            if (col == columns - 1) {
                one_row_str = one_row_str + temp.str();
            }
            else {
                one_row_str = one_row_str + temp.str() + ", ";
            }
        }
        all_rows.push_back(one_row_str);
        //printf("\n");
    }
    //printf("\n");

}

//takes in premalloced 2D array to copy to
void copy_matrix(int** array, int rows, int columns, int** the_copy) {
    //copy every value of original matrix to the_copy
    int row, col;
    for (row = 0; row < rows; row++) {
        for (col = 0; col < columns; col++) {
            the_copy[row][col] = array[row][col];
        }
    }
}

int** randomMatrix(int rows, int columns, int limit) {
    int** result = malloc2d(rows, columns);

    int row, col;
    for (row = 0; row < rows; row++) {

        for (col = 0; col < columns; col++) {
            int value = rand() % limit;
            result[row][col] = value;
        }
    }

    return result;
}

int** addMatrices(int** A, int** B, int rows, int columns) {
    int** result = malloc2d(rows, columns);

    int row, col;
    for (row = 0; row < rows; row++) {

        for (col = 0; col < columns; col++) {
            result[row][col] = A[row][col] + B[row][col];
        }
    }

    return result;
}
