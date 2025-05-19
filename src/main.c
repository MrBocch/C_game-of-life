#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <wchar.h>

#define SECONDS  0
#define NANOSEC  100000000

void clear_screen();
char **random_board(int rows, int cols, char alive, char dead);
void print_board(char **cells, int rows, int cols);
void next_frame(char **cells, int rows, int cols, char a, char d);

int main(int argc, char *argv[]){
    if(argc == 1 || argc != 5) {
        printf("How to use me:\n");
        puts("gol <ROWS> <COLS> <ALIVE_CHAR> <DEAD_CHAR>");
        return 1;
    }
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    char alive = argv[3][0];
    char dead = argv[4][0];

    char **cells = random_board(rows, cols, alive, dead);

    struct timespec ts;
    ts.tv_sec = SECONDS;
    ts.tv_nsec = NANOSEC;
    int i = 0;
    while(1){
        // clears screen
        printf("\033[2J\033[H");

        printf("Generation: %d\n", i);
        print_board(cells, rows, cols);
        next_frame(cells, rows, cols, alive, dead);

        i += 1;
        nanosleep(&ts, NULL);
    }

    return 0;
}

// does not work how I expect
void clear_screen() {
    printf("\033[2J\033[H");
}

// just assuming everything goes ok
char **random_board(int rows, int cols, char alive, char dead){

    char **board = malloc(rows * sizeof(char *));
    for(int i = 0; i < rows; i++){
        board[i] = malloc(cols * sizeof(char));
    }

    srand(time(NULL));
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            int random = rand() % 2;
            if(random == 0){
               board[i][j] = dead;
            } else {
                board[i][j] = alive;
            }
        }
    }
    return board;
}

void print_board(char **cells, int rows, int cols){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            printf("%c", cells[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

// adding all the code for adding the
// wrapping is super annoying, I must refactor
void next_frame(char **cells, int rows, int cols, char a, char d){
    bool changes[rows][cols]; // if true, 0 -> 1
                                 //          1 -> 0
                                 // else keep the same

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            int alive_n = 0;

            // right
            if(j+1 < cols){
                if(cells[i][j+1] == a) alive_n++;
            } else {
                // wrap around
                // if(cells[i][0] == A) alive_n++;
            }
            // left
            if(j-1 >= 0) {
                if(cells[i][j-1] == a) alive_n++;
            } else {
                // wrap around
                // if(cells[i][COLUMNS-1] == A) alive_n++;
            }
            // up
            if(i-1 >= 0){
                if(cells[i-1][j] == a) alive_n++;
            } else {
                // wrap around
                // if(cells[ROWS-1][j] == A) alive_n++;
            }
            // down
            if(i+1 < rows){
                if(cells[i+1][j] == a) alive_n++;
            } else {
                // wrap around
                // if(cells[0][j] == A) alive_n++;
            }

            // up left
            if(i-1 >= 0 && j-1 >= 0){
                if(cells[i-1][j-1] == a) alive_n ++;
            } else {
                // wrap around

            }

            // up right
            if(i-1 >= 0 && j+1 < cols){
                if(cells[i-1][j+1] == a) alive_n++;
            } else {
                // wrap around

            }

            // down left
            if(i+1 < rows && j-1 >= 0){
                if(cells[i+1][j-1] == a) alive_n++;
            } else {
                // wrap around

            }

            // down right
            if(i+1 < rows && j+1 < cols){
                if(cells[i+1][j+1] == a) alive_n += 1;
            } else {
                // wrap around

            }

            // printf("%d,%d has %d alive n\n", i, j, alive_n);
            if(cells[i][j] == a){
                if(alive_n < 2) changes[i][j] = true;
                if(alive_n >= 2 && alive_n <= 3) changes[i][j] = false;
                if(alive_n > 3) changes[i][j] = true;
            }
            if(cells[i][j] == d){
                if(alive_n == 3) {
                    changes[i][j] = true;
                } else {
                    changes[i][j] = false;
                }
            }
        }
    }

    // change board now
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(changes[i][j] == true){
                if(cells[i][j] == a) { cells[i][j] = d; continue; }
                if(cells[i][j] == d) { cells[i][j] = a; continue; }
            }
        }
    }
}
