#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 4

#define DIR_W 1
#define DIR_A 2
#define DIR_S 3
#define DIR_D 4 

#define WIN 2
#define LOSE 1

#define T_LIMIT 900

typedef struct score{
    char name[30];
    int isWin;
    int moves;
    int combo;
    clock_t start;
    clock_t Total_Time;
}SCORE;


/* TO_DO
    1. action debug
    2. show_ranking, add_ranking complete
    3. report
        3-1. build C file in win, MacOS, Linux
        3-2. How to play game
        3-3. how to implement several features.
        3-4. troubleshooting points. 
    
    first, complete 2 and finish 3-1,2,3. Debugging, complete 3-4.
*/


void action(int *dir_board[SIZE][SIZE], SCORE *score){
    /* along the w/a/s/d direction board, move and merge numbers. */
    /* move numbers */
    int nums[SIZE];
    int k = 0, dummy, ch, flag = 0;
    for (int i = 0; i < SIZE; i++)
    {
        /* moving numbers */
        k = 0;
        for (int j = 0; j < SIZE; j++){
            if(dummy = *dir_board[i][j]){
                nums[k++] = dummy; 
                *dir_board[i][j] = 0;
            }
        }
        do{*dir_board[i][SIZE - 1 - k] = nums[k];} while(k--);
        /* merge numbers */
        ch = 0;
        while(!flag){
            if(*dir_board[i][ch] == *dir_board[i][ch+1] && !*dir_board[i][ch]){
                *dir_board[i][ch+1] += *dir_board[i][ch];
                *dir_board[i][ch] = 0;
                flag = 1;
            }
            if(++ch >= SIZE - 1) break;
        }
    }
    if(flag) (*score).combo += 1;
    else (*score).combo = 0;

    return;
}

void give_direction(int board[SIZE][SIZE], int dir, int *dir_board[SIZE][SIZE]){
    /* pointer array of board, corresponding to w/a/s/d */
    register int i, j;
    switch (dir){
        case DIR_W:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    dir_board[i][j] = &board[SIZE - 1 - j][i];
            break;
        case DIR_A:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    dir_board[i][j] = &board[i][SIZE - 1 -j];
            break;

        case DIR_S:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    dir_board[i][j] = &board[j][i];
            break;

        case DIR_D:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    dir_board[i][j] = &board[i][j];
            break;
    }
    return;
}

int blank(int board[SIZE][SIZE]){
    /* returns how many 0s in the board */
    int k = 0;
    for(register int i = 0; i < SIZE; i++)
        for(register int j = 0; j < SIZE; j++)
            if(!board[i][j]) k++;
    return k;
}


void random_gen(int board[SIZE][SIZE]){
    
    int k;
    if(!(k = blank(board))) return; 
    k = rand() % k + 1;
    
    for(register int i = 0; i < SIZE; i++)
        for(register int j = 0; j < SIZE; j++){
            if(!board[i][j]) k--;
            if(!k) board[i][j] = (rand()%2)? 2 : 4;
        }
    
    return;

}

int check(int board[SIZE][SIZE], int prev_board[SIZE][SIZE], SCORE *score){
    /* check if move was successful and decide game over or clear */
    /*move check*/
    int flag = 0;
    for (register int i = 0; i < SIZE; i++)
    {
        for (register int j = 0; j < SIZE; j++)
        {
            if (board[i][j] != prev_board[i][j])
            {
                    flag = 1;
                    break;
            }
        }
    }
    if (flag)
    {
        (*score).moves += 1;
        random_gen(board);
        for (register int i = 0; i < SIZE; i++)
        {
            for (register int j = 0; j < SIZE; j++)
            {
                    prev_board[i][j] = board[i][j];
            }
        }
    }

    /* win or lose or continue(0) check + time limit check */

    if (clock() >= score->start + T_LIMIT * CLOCKS_PER_SEC)
        return LOSE;

    for (register int x; x < SIZE; x++)
    {
        for (register int y; y < SIZE; y++)
        {
            if (board[x][y] == 2048)
                    return WIN;
        }
    }


    if (!blank(board))
    {
        /* if board is not blank, check adjacent cells are identical. */
        for (register int x; x < SIZE; x++)
        {
            for (register int y; y < SIZE; y++)
            {
                for (register int dx = (x > 0 ? -1 : 0); dx <= (x < SIZE - 1 ? 1 : 0); ++dx)
                {
                    for (register int dy = (y > 0 ? -1 : 0); dy <= (y < SIZE - 1 ? 1 : 0); ++dy)
                    {
                        if (dx + dy == 1 || dx + dy == -1)
                        {
                            if (board[x][y] == board[x + dx][y + dy])
                                return 0;
                        }
                    }
                }
            }
        }

        return LOSE;
    }

    return 0;
}

void add_ranking(SCORE score){
    /* open a txt file and add score*/
    printf("added ranking.");
    return;
}



void main_menu(){
    printf("=================\n");
    printf(" 2048 : by Deserted\n");
    printf("a. start game\n");
    printf("b. How to play\n");
    printf("c. ranking\n");
    printf("press e to exit\n");
    printf("=================\n");
    return;
}

void display_board(int board[SIZE][SIZE], SCORE score)
{
    /* print board status and combo, moves, remaining time. */
    int remain = (int) (T_LIMIT - (clock() - score.start) / CLOCKS_PER_SEC);
    register int row, columns;

    printf("%d combo, %d moves, %d : %d ...\n", score.combo, score.moves, remain / 60, remain % 60);

    for (row = 0; row < SIZE; row++)
    {
        for (columns = 0; columns < SIZE; columns++)
        {
            printf("%d     ", board[row][columns]);
        }
        printf("\n");
    }

    printf(">>>");
    return;
}

int in_game(){
    /* From start to ranking */
    int ch, wl, flag = 1;
    SCORE new_score;
    new_score.combo = 0;
    new_score.moves = 0;
    new_score.start = clock();
    int board[SIZE][SIZE] = {0};
    int prev_board[SIZE][SIZE] = {0};

    int* (*dir_board)[SIZE];
    dir_board = malloc(sizeof(int[SIZE][SIZE]));

    random_gen(board);
    for(register int i = 0; i < SIZE; i++){
        for(register int j = 0; j < SIZE; j++){
            prev_board[i][j] = board[i][j];
        }
    }

    while(flag){
        system("cls");
        display_board(board, new_score);
        if(ch = _getch()){
            switch (ch){
                case 'w':
                    give_direction(board, DIR_W, dir_board);                      
                    action(dir_board, &new_score);
                    break;
                case 'a':
                    give_direction(board, DIR_A, dir_board);                      
                    action(dir_board, &new_score);
                    break;
                case 's':
                    give_direction(board, DIR_S, dir_board);                      
                    action(dir_board, &new_score);
                    break;
                case 'd':
                    give_direction(board, DIR_D, dir_board);                      
                    action(dir_board, &new_score);
                    break;
                default:
                    break; 
            }
        }
        if (wl = check(board, prev_board, &new_score))
            flag = 0;
    }

    free(dir_board);

    if(wl == WIN) {
        printf("You Win! Your name? : ");
        new_score.isWin = WIN;
        new_score.Total_Time = clock() - new_score.start;
        scanf("%s", new_score.name);
        add_ranking(new_score);
        return 1;
    }
    else if (wl == LOSE){
        printf("You Lose. Your name? : ");
        new_score.isWin = LOSE;
        new_score.Total_Time = clock() - new_score.start;
        scanf("%s", new_score.name);
        add_ranking(new_score);
        return 1;
    }

    return 0;
}


int how_to(){
    int ch;
    printf("=================\n");
    printf(" 2048 : by Deserted\n");
    printf("start : in 4x4 board, randomly 2 or 4 generated.\n");
    printf("You can move numbers by pressing W/A/S/D\n");
    printf("if moving nums encounter same num, they merges.\n");
    printf("if you make 2048, you win.\n");
    printf("if you can't move anymore, you lose.\n");
    printf("press any key to exit\n");
    printf("=================\n");
    while(1){
        if(ch = _getch()) return 1;
    }
    return 0;
}

int show_ranking(){
    /* open txt file and print ranking. */
    int ch;
    printf("this is ranking. press any key to exit\n");
    while(1){
        if(ch = _getch()) return 1;
    }
    return 0;
}


int main(void){
    srand((unsigned int)time(NULL)); 
    /*keyboard press -> update screen*/    
    int ch;
    int flag = 1;
    while(flag){
        system("cls");
        main_menu();
        if (ch = _getch())
        {
            system("cls");
            switch (ch) {
                case 'a':
                    if(!in_game()){
                        printf("Unusual behavior.");
                        flag = 0;
                    }
                    break;
                case 'b':
                    if(!how_to()){
                        printf("Unusual behavior.");
                        flag = 0;
                    }
                    break;
                case 'c':
                    if(!show_ranking()){
                        printf("Unusual behavior");
                        flag = 0;
                    } 
                    break;
                case 'e':
                    exit(1);
                    break;
            } 
        }
    }
    return 0;
}
