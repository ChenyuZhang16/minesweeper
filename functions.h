#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// initialise and zero a 2D num_row*num_col matrix
int** arr_init(int num_row, int num_col);

// display map
void map_disp(int** map, int num_row, int num_col, int num_mines, int marked_mines, int snap);

// map initialisation. size: num_row * num_col, init_ stands for the first user
// position that user interacted; num_mines: num of mines
int** map_init(int num_row, int num_col, int init_row, int init_col, int num_mines);

// create a vector of length (size + 1), with values of each entry
// from the first to the last entry initialised to: {0,1,2,...,size-2,size-1,-1}
// the last entry -1 is to mark the end of this sequence
int* vector_init(int size);

// delete an entry inside a vector with value num and
// shift all the other entries on the right of num to left by 1 step
void vector_del(int* vctr, int num);

// reset seed for rand() to current time and run rand() for count times
void reset_rand(int count);

// reset seed for rand() to current time and run rand() for count times
void reset_rand(int count);

// clear a 2D array and set the pointer to NULL
void arr_del(int**& ptr, int num_row);

// count and return the num of mines touching the location (row, col)
int count_mine(int** map, int num_row, int num_col, int row, int col);

// append the map with mine count info
void map_mine_count(int** map, int num_row, int num_col);

// Welcome info and prompt user for map size and num of mines info
void welcome(int& num_row, int& num_col, int& num_mines);

// Prompt user with inputs and alter usr array accordingly
// Return -2: if user choose to reveal a location
// Return -1: if user choose to mark a mine location
// Return 0: user choose to erase current mark
// return 10: error
int usr_input(int** usr, int num_row, int num_col, int& usr_row, int& usr_col, int& marked_mines);

// User initial input and initialised the map
void usr_init(int**& usr, int**& map, int num_row, int num_col, int num_mines, int& snap, int& marked_mines, int& usr_row, int& usr_col);

// check if game is over, return:
// 1 if wins; -1 if lose; 0 if game continue
int chk_over(int** usr, int** map, int num_row, int num_col, int num_mines, int marked_mines, int usr_row, int usr_col);

// map revealing - recursion
void map_reveal(int** usr, int** map, int num_row, int num_col, int rvl_row, int rvl_col);

// Game-over: win. Disp congrats
void over_win(int** usr, int num_row, int num_col, int num_mines, int snap, int marked_mines);

#endif
