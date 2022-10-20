#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int DIAGONAL  = 10 ;
#define MINE -1
int NUMBER_OF_MINES ;

typedef struct Cell {
    int adjacent_mines;
    bool is_visible;
} Cell;


void fill_with_zeros(Cell mat[][10]) {
    int i, j;
    for (i = 0; i < DIAGONAL; ++i) {
        for (j = 0; j < DIAGONAL; ++j) {
            mat[i][j].adjacent_mines = 0;
            mat[i][j].is_visible = false;
        }
        // type(mat[i][j]) == Cell
    }
}

void place_mines_randomly(Cell mat[][10]) {
    //to know when to stop placing
    int number_of_placed_mines = 0;

    while (number_of_placed_mines < NUMBER_OF_MINES) {
        int row = rand() % DIAGONAL;
        int column = rand() % DIAGONAL;
        Cell cell = mat[row][column];

        if (cell.adjacent_mines != MINE) {
            mat[row][column].adjacent_mines = MINE; // -1
            number_of_placed_mines++;
        }
    }
}


bool in_bounds(int i) {
    return i < DIAGONAL && i >= 0;
}


int count_number_of_adjacent_mines(Cell mat[][10], int i, int j) {
    int number_of_mines = 0;
    int adjacent_row;
    int adjacent_column;

    for (adjacent_row = i - 1; adjacent_row <= i + 1; ++adjacent_row) {
        for (adjacent_column = j - 1; adjacent_column <= j + 1; ++adjacent_column) {
            if (adjacent_row == i && adjacent_column == j)
                continue;

            if (
                in_bounds(adjacent_row) && in_bounds(adjacent_column)
                && mat[adjacent_row][adjacent_column].adjacent_mines == MINE
            )
                number_of_mines++;
        }
    } 

    return number_of_mines;
}


void compute_adjacent_mines(Cell mat[][10]) { 
    int i, j;
    for (i = 0; i < DIAGONAL; ++i) {
        for (j = 0; j < DIAGONAL; ++j) {
            if (mat[i][j].adjacent_mines != MINE) {
                mat[i][j].adjacent_mines = count_number_of_adjacent_mines(mat, i, j);
            }
        }
    }
}


void init_game(Cell mat[][10]) {
    srand(time(0));
    
    fill_with_zeros(mat);//////////
    
    place_mines_randomly(mat);/////////////

    compute_adjacent_mines(mat);/////////// 
}


void print_cell(Cell cell) {
    if (cell.is_visible) {
        if (cell.adjacent_mines == MINE) {
            printf(" * ");
        }
        else if (cell.adjacent_mines == 0) {
            printf(" . ");
        }
        else {
            printf(" %d ", cell.adjacent_mines);
        }
    }
    else {
       printf("   "); 
    }
}


void print_game(Cell mat[][10]) {
    int i, j;

    printf(" ");
    
    for (int column = 0; column < DIAGONAL; ++column) {
        printf(" %d ", column);
    }
    
    printf("\n");

    for (i = 0; i < DIAGONAL; ++i) {
        
        printf("%c", 'A' + i);

        for (j = 0; j < DIAGONAL; ++j) {
            print_cell(mat[i][j]);
        }
        printf("\n");
    }
}


bool select_cell(Cell mat[][10], int row, int column) {
    Cell selected_cell = mat[row][column];

    if (selected_cell.adjacent_mines == MINE) {
        return false;
    }

    mat[row][column].is_visible = true;

    return true;
}


int main() {

printf("Enter the  number of mines [3-30]: ");
    scanf("%d", &NUMBER_OF_MINES);
    while (NUMBER_OF_MINES < 3 || NUMBER_OF_MINES > 30)
    {
        printf("Wrong value ! try again !\nEnter the  number of mines [3-30]: ");
        scanf("%d", &NUMBER_OF_MINES);
    }


    Cell mat[DIAGONAL][DIAGONAL];
    char row_as_char;
    int row, column;
    bool not_exploded = true;

    int number_of_unveiled_cells = 0;

    int winning_condition = DIAGONAL * DIAGONAL - NUMBER_OF_MINES;


    init_game(mat); /// placed mines 

    print_game(mat);
    

    do {
        printf("enter coordinate e.g. A9: ");
        scanf("%c%d", &row_as_char, &column);
        row = row_as_char - 'A';  /// A = 40 - 40 = 0;
                                // (B) 41 - 40 = 1
                                //etc

        not_exploded = select_cell(mat, row, column);

        ++number_of_unveiled_cells;

        printf("\n");
        print_game(mat);
    } while (not_exploded && number_of_unveiled_cells < winning_condition);
    

    if (not_exploded) {
        printf("You've won!\n");
    } else {
        printf("%c%d was a mine!\n", row_as_char, column);
        printf("BOOM!!!!! Exploded!\n");
    }

    return 0;
}