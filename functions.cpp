#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "functions.h"

// initialise and zero a 2D num_row*num_col matrix
int** arr_init(int num_row, int num_col)
{
    int** ptr = new int* [num_row];
    for (int row = 0; row < num_row; row ++)
    {
	ptr[row] = new int [num_col];
	for (int col = 0; col < num_col; col ++)
	    ptr[row][col] = 0;
    }
    
    return ptr;
}

// display map
void map_disp(int** map, int num_row, int num_col, int num_mines, int marked_mines, int snap)
{
    using namespace std;

    // map header:
    cout << "- - - - - - - - - -" << endl;
    cout << "Snapshot: " << snap;
    cout.width(20);
    cout << "Map size: " << num_row << " x " << num_col;
    cout.width(30);
    cout << "Mine(s) remaining: " << num_mines - marked_mines << endl << endl;

    // column location
    char gap[10] = "      ";
    cout << gap << " ";
    for (int col = 0; col < num_col; col++)
    {
	if (col < 100)
	{
	    cout.width(2);
	    cout << col << "  ";    
	}
	else if (col >= 100)
	{
	    cout.width(3);
	    cout << col << " ";    
	}
    }
    cout << endl;

    cout << gap << "  ";
    for (int col = 0; col < num_col; col++)
	cout << "v   ";
    cout << endl;
    
    for (int row = 0; row < num_row; row ++)
    {
	cout << gap;
	for (int col = 0; col < num_col; col ++)
	    cout << "+---";
	cout << "+" << endl;

	cout.width(3);
	cout << row << " > ";
	for (int col = 0; col < num_col; col ++)
	{
	    cout << "| ";
	    if (map[row][col] == -1)
		cout << "x";
	    else if (map[row][col] == 0)
		cout << " ";
	    else if (map[row][col] == -3)
		cout << ".";
	    else
		cout << map[row][col];
	    cout << " ";
	}
	cout << "|" << endl;
    }
    cout << gap;
    for (int col = 0; col < num_col; col ++)
	cout << "+---";
    cout << "+" << endl;
    
    return;
}

// map initialisation. size: num_row * num_col, init_ stands for the first
// position that user interacted; num_mines: num of mines
int** map_init(int num_row, int num_col, int init_row, int init_col, int num_mines)
{
    using namespace std;
    // if too many mines
    if (num_mines >= num_row * num_col)
    {
	cerr << "ERROR: map initialisation failed: num of mines >= map size" << endl;
	exit (1);
    }
    
    // initilise zero the map array    
    int** map = arr_init(num_row, num_col);

    // create a 1D -> 2D mapping of the matrix indexes
    int* vctr = vector_init(num_row * num_col);

    // eliminate the first chosen position (init_row, init_col) from the 1D mapping
    int init_idx = num_col * init_row + init_col;
    vector_del(vctr, init_idx);

    int vctr_length = num_row * num_col; // (initial size of vctr) - 1 // WHY DO I NEED THIS????

    for (int count = 1; count <= num_mines; count ++)
    {
	int mine_idx = rand() % (vctr_length - 1);

	// location of mine in 2D map
	int mine_row = vctr[mine_idx] / num_col;
	int mine_col = vctr[mine_idx] % num_col;
	map[mine_row][mine_col] = -1;
	
	vector_del(vctr, vctr[mine_idx]);
	
	vctr_length --;
    }

    delete [] vctr;
    
    return map;
}

// create a vector of length (size + 1), with values of each entry
// from the first to the last entry initialised to: {0,1,2,...,size-2,size-1,-1}
// the last entry -1 is to mark the end of this sequence
int* vector_init(int size)
{
    int* vctr = new int[size + 1];

    for (int idx = 0; idx < size; idx ++)
    	vctr[idx] = idx;

    vctr[size] = -1;

    return vctr;
}

// delete an entry inside a vector with value num and
// shift all the other entries on the right of num to left by 1 step
void vector_del(int* vctr, int num)
{
    using namespace std;
    
    //search for the number
    int idx;
    for (idx = 0; !(vctr[idx] == num || vctr[idx] == -1); idx ++)
    // vctr[idx] == num or idx == length of vctr

    if (vctr[idx] == -1)
    {
	cerr << "ERROR: vector_del could not find " << num << " in the vector" << endl;
	exit(1);
    }

    for (int count = idx; vctr[count] != -1; count ++)
	vctr[count] = vctr[count + 1];

    return;
}


// reset seed for rand() to current time and run rand() for count times
void reset_rand(int count)
{
    srand(time(0));
    for (int i = 1; i <= count; i ++)
	rand();
    return;	    
}

// clear a 2D array and set the pointer to NULL
void arr_del(int**& ptr, int num_row)
{
    for (int row = 0; row < num_row; row ++)
	delete [] ptr[row];
    delete [] ptr;
    ptr = NULL;
    return;
}

// count and return the num of mines touching the location (row, col)
int count_mine(int** map, int num_row, int num_col, int row, int col)
{
    using namespace std;
    if (map[row][col] == -1)
	cerr << "WARNING: count_mine() called at a mine location!" << endl;

    int count = 0;
    for (int del_row = -1; del_row <= 1; del_row ++)
	for (int del_col = -1; del_col <= 1; del_col ++)
	{
	    int row_now = row + del_row;
	    int col_now = col + del_col;
	    
	    if ( !((del_row == 0 && del_col == 0)
		  || row_now < 0 || col_now < 0
		  || row_now >= num_row || col_now >= num_col) )
	    {
		if (map[row_now][col_now] == -1)
		    count ++;
	    }
	}
    
    return count;
}

// append the map with mine count info
void map_mine_count(int** map, int num_row, int num_col)
{
    for (int row = 0; row < num_row; row ++)
	for (int col = 0; col < num_col; col ++)
	    if (map[row][col] != -1)
		map[row][col] = count_mine(map, num_row, num_col, row, col);
    return;
}

// Welcome info and prompt user for map size and num of mines info
void welcome(int& num_row, int& num_col, int& num_mines)
{
    using namespace std;

    cout << "---- Minesweaper ----" << endl;
    cout << "(To enter a input, follow the input value by RETERN key)" << endl << endl;
    cout << "Setting map size:" << endl;
    cout << "Enter number of rows: ";
    cin >> num_row;
    cout << "Enter number of columns: ";
    cin >> num_col;
    cout << "Enter number of mines: ";
    cin >> num_mines;
    while (num_mines >= num_row * num_col)
    {
	cout << num_mines << " mines are too many for the current map size!" << endl
	     << "Please enter a smaller number of mines followed by RET: ";
	cin >> num_mines;
    }
    cout << "Map size setting complete." << endl << endl;

    cout << "==== Game Starts ====" << endl << endl;

    return;
}

// User initial input
void usr_init(int**& usr, int**& map, int num_row, int num_col, int num_mines, int& snap, int& marked_mines, int& usr_row, int& usr_col)
{
    using namespace std;

    usr = arr_init(num_row, num_col);
    
    map_disp(usr, num_row, num_col, num_mines, marked_mines, snap);
    cout << endl;

    // prompt user for input and action
    int usr_inum = usr_input(usr, num_row, num_col, usr_row, usr_col, marked_mines);
    snap ++;

    while (usr_inum != -2) // if user is not revealing a position
    {
	map_disp(usr, num_row, num_col, num_mines, marked_mines, snap);
	cout << endl;
	usr_inum = usr_input(usr, num_row, num_col, usr_row, usr_col, marked_mines);
	snap ++;
    }

    map = map_init(num_row, num_col, usr_row, usr_col, num_mines);

    return;
}

// Prompt user with inputs and alter usr array accordingly
// Return -2: if user choose to reveal a location
// Return -1: if user choose to mark a mine location
// Return 0: user choose to erase current mark
// return 10: error
int usr_input(int** usr, int num_row, int num_col, int& usr_row, int& usr_col, int& marked_mines)
{
    using namespace std;
    char str[10];
    cout << "Enter a location (row, column)" << endl
	 << "row: ";
    cin >> usr_row;
    cout << "column: ";
    cin >> usr_col;
    cout << "Action? (m: mark mine position; r: reveal; e: erase current mine mark): ";    
    cin.ignore(); // skip the \n from previous cin >> statement
    cin.getline(str, 10);
    char usr_ch = str[0]; // only take the first char to avoid corrupted cin stream
    
    while (usr_ch != 'm' && usr_ch != 'M' && usr_ch != 'r' && usr_ch != 'R'
	   && usr_ch != 'e' && usr_ch != 'E')
    {
	cout << "Unknown action. Please input either m, r or e: " << flush;
	cin.getline(str, 10);
	usr_ch = str[0];
    }
    
    int usr_inum;
    switch (usr_ch)
    {
    case 'm':
    case 'M':
	if (usr[usr_row][usr_col] != 0)
	{
	    cout << "Can only mark undefined area as mine!" << endl;
	    return 10;
	}
	usr_inum = -1;
	marked_mines ++;
	break;
    case 'r':
    case 'R':
	if (usr[usr_row][usr_col] != 0)
	{
	    cout << "Can only reveal undefined area!" << endl;
	    return 10;
	}
	usr_inum = -2;
	break;
    case 'e':
    case 'E':
	if (usr[usr_row][usr_col] != -1)
	{
	    cout << "Can only unmark a mine location!" << endl;
	    return 10;
	}
	usr_inum = 0;
	marked_mines --;
	break;
    }

    usr[usr_row][usr_col] = usr_inum;

    return usr_inum;
}

// check if game is over, return:
// 1 if wins; -1 if lose; 0 if game continue
int chk_over(int** usr, int** map, int num_row, int num_col, int num_mines, int marked_mines, int usr_row, int usr_col)
{
    if (map[usr_row][usr_col] == -1 && usr[usr_row][usr_col] == -2)
	return -1;

    if (num_mines == marked_mines)
    {
	for (int row = 0; row < num_row; row ++)
	    for (int col = 0; col < num_col; col ++)
		if (map[row][col] == -1 && usr[row][col] != -1)
		    return 0;
	return 1;
    }
    
    return 0;
}

// map revealing - recursion
void map_reveal(int** usr, int** map, int num_row, int num_col, int rvl_row, int rvl_col)
{
    if (rvl_row < 0 || rvl_col < 0 || rvl_row >= num_row || rvl_col >= num_col)
	return;

    if (usr[rvl_row][rvl_col] != 0 && usr[rvl_row][rvl_col] != -2)
	return;

    if (map[rvl_row][rvl_col] != 0)
	usr[rvl_row][rvl_col] = map[rvl_row][rvl_col];
    else
    {
	usr[rvl_row][rvl_col] = -3;
	for (int d_row = -1; d_row <= 1; d_row ++)
	    for (int d_col = -1; d_col <= 1; d_col ++)
		if (!(d_row == 0 && d_col == 0))
		    map_reveal(usr, map, num_row, num_col, rvl_row + d_row, rvl_col + d_col);
    }
    return;
}

// Game-over: win. Disp congrats
void over_win(int** usr, int num_row, int num_col, int num_mines, int snap, int marked_mines)
{
    using namespace std;
    map_disp(usr, num_row, num_col, num_mines, marked_mines, snap);
    cout << endl;

    cout << "==== You Win!! ====" << endl;
    return;
}
