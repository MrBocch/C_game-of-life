#include <stdio.h>
#include <wchar.h>
#include <stdbool.h>
#include <time.h>

#define ROWS     10
#define COLUMNS  10
#define ALIVE    '#'
#define DEAD     '.'
#define SECONDS  0
#define NANOSEC  100000000
void clear_screen();
void print_board(char cells[ROWS][COLUMNS]);
void next_frame(char cells[ROWS][COLUMNS]);

int main(){

    // a glider
    char cells[ROWS][COLUMNS] = {
        {DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {ALIVE, ALIVE, ALIVE, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
        {DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD, DEAD},
    };

    struct timespec ts;
    ts.tv_sec = SECONDS;
    ts.tv_nsec = NANOSEC;
    int i = 0;
    while(1){
        // clears screen
        printf("\033[2J\033[H");

        printf("Generation: %d\n", i);
        print_board(cells);
        next_frame(cells);

        i += 1;
        nanosleep(&ts, NULL);
    }

    return 0;
}

// does not work how I expect
void clear_screen() {
    printf("\033[2J\033[H");
}

void print_board(char cells[ROWS][COLUMNS]){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            printf("%c", cells[i][j]);
        }
        printf("\n");
    }
    printf("\n");
    return;
}

// the rules are
// live cell dies if it has fewer than 2 neighbours
// life cell lives on if has 2-3 neighbours
// life cell dies if it has more then 3 neighbours
// dead cell comes alive if it has exaclty 3 neighbours
void next_frame(char cells[ROWS][COLUMNS]){
    bool changes[ROWS][COLUMNS]; // if true, 0 -> 1
                                 //          1 -> 0
                                 // else keep the same

    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            int alive_n = 0;

            // right
            if(j+1 < COLUMNS){
                if(cells[i][j+1] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }
            // left
            if(j-1 >= 0) {
                if(cells[i][j-1] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }
            // up
            if(i-1 >= 0){
                if(cells[i-1][j] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }
            // down
            if(i+1 < ROWS){
                if(cells[i+1][j] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }

            // up left
            if(i-1 >= 0 && j-1 >= 0){
                if(cells[i-1][j-1] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }

            // up right
            if(i-1 >= 0 && j+1 < COLUMNS){
                if(cells[i-1][j+1] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }

            // down left
            if(i+1 < ROWS && j-1 >= 0){
                if(cells[i+1][j-1] == ALIVE) alive_n +=1;
            } else {
                // wrap around
            }

            // down right
            if(i+1 < ROWS && j+1 < COLUMNS){
                if(cells[i+1][j+1] == ALIVE) alive_n += 1;
            } else {
                // wrap around
            }

            // printf("%d,%d has %d alive n\n", i, j, alive_n);
            if(cells[i][j] == ALIVE){
                if(alive_n < 2) changes[i][j] = true;
                if(alive_n >= 2 && alive_n <= 3) changes[i][j] = false;
                if(alive_n > 3) changes[i][j] = true;
            }
            if(cells[i][j] == DEAD){
                if(alive_n == 3) {
                    changes[i][j] = true;
                } else {
                    changes[i][j] = false;
                }
            }
        }
    }

    // change board now
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLUMNS; j++){
            if(changes[i][j] == true){
                if(cells[i][j] == ALIVE) { cells[i][j] = DEAD; continue; }
                if(cells[i][j] == DEAD) { cells[i][j] = ALIVE; continue; }
            }
        }
    }
}
