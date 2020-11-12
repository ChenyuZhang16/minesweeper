#include <iostream>
#include "functions.h"

int main()
{
    using namespace std;
    // resetting the rand seed
    reset_rand(100);
    
    // define map size
    int num_row = 0, num_col = 0, num_mines = 0, snap = 0, marked_mines = 0;

    // Welcome and ask user for map setting
    welcome(num_row, num_col, num_mines);

    // Initialise the user map (what use will see and interact with)
    // and mine map (user wont see)
    int** map, ** usr;
    int usr_row = 0, usr_col = 0; // user input loc
    usr_init(usr, map, num_row, num_col, num_mines, snap, marked_mines, usr_row, usr_col);
    
    // amend map with mine counts at each location
    map_mine_count(map, num_row, num_col);

    // display map with mine locations and mine counts
    map_disp(map, num_row, num_col, num_mines, 0, 0);

    // check if game continue?
    int ifover = chk_over(usr, map, num_row, num_col, num_mines, marked_mines, usr_row, usr_col);
    while (!ifover)
    {
	// auto-reveal map
	map_reveal(usr, map, num_row, num_col, usr_row, usr_col);

	// user input
	map_disp(usr, num_row, num_col, num_mines, marked_mines, snap);
	cout << endl;
	int usr_inum = usr_input(usr, num_row, num_col, usr_row, usr_col, marked_mines);
	snap ++;
	while (usr_inum == 10) // 10 is the error code
	{
	    map_disp(usr, num_row, num_col, num_mines, marked_mines, snap);
	    cout << endl;
	    usr_inum = usr_input(usr, num_row, num_col, usr_row, usr_col, marked_mines);
	    snap ++;
	}

	// check if game continue?
	ifover = chk_over(usr, map, num_row, num_col, num_mines, marked_mines, usr_row, usr_col);
    }

    if (ifover == 1) //win
    	over_win(usr, num_row, num_col, num_mines, snap, marked_mines);
    else //loose
	cout << "==== You lose :( ====" << endl;
    
    // delete heap storage
    arr_del(map, num_row);
    arr_del(usr, num_row);
    
    return 0;
}
