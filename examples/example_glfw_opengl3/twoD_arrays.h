#pragma once
int** malloc2d(int rows, int columns);

void free2d(int** array, int row, int columns);

void printMatrix(int** array, int row, int columns);

void printMatrixWithMappedValues(int** array, int rows, int columns, int* mapped_vals);
//version for IMGUI application to convert to string and add to a vector of strings
//void copyMatrixWithMappedValuesToStringVector(int** array, int rows, int columns, int* mapped_vals, std::vector <std::string> all_rows);
void copyMatrixWithMappedValuesToStringVector(int** array, int rows, int columns, std::vector <int> mapped_vals, std::vector <std::string> all_rows);
//takes in premalloced 2D array to copy to
void copy_matrix(int** array, int rows, int columns, int** the_copy);

int** randomMatrix(int rows, int columns, int limit);

int** addMatrices(int** A, int** B, int rows, int columns);
