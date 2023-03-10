#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <assert.h>

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
    int cur_combo;
    int Total_score;
    clock_t start;
    clock_t Total_Time;
}SCORE;


/* TO_DO
    1. action debug
    FINISH >> 2. show_ranking, add_ranking complete
    FINISH >> 3. report
        3-1. build C file in win, MacOS, Linux
        3-2. How to play game
        3-3. how to implement several features.
        3-4. troubleshooting points. 
    FINISH >> 4. combo : maximum combos 
    FINISH >> 5. ranking score sorting
    first, complete 2 and finish 3-1,2,3. Debugging, complete 3-4.
*/


void action(int *dir_board[SIZE][SIZE], SCORE *score){
    /* along the w/a/s/d direction board, move and merge numbers. */
    /* move numbers */
    int nums[SIZE] = {0};
    int k, ch, dummy,  flag, merger;
    for (int i = 0; i < SIZE; i++)
    {
        k = 0;
        
        /* moving numbers */
        for (register int j = 0; j < SIZE; j++){
            nums[j] = 0; /*initialization starting every i*/
            if(dummy = *dir_board[i][j]){
                nums[k] = dummy;
                k++;
            }
            *dir_board[i][j] = 0;
        } 
            
        /* merge numbers */
        ch = k - 2;
        if(k >= 2){
            for (register int j = 0; j < k; j++)
            {
                *dir_board[i][SIZE - 1 - j] = nums[k - 1 - j];
            }

            do{
                if (*dir_board[i][ch] == *dir_board[i][ch + 1] && (merger = *dir_board[i][ch]))
                {
                    *dir_board[i][ch + 1] *= 2;
                    *dir_board[i][ch] = 0;
                    (*score).Total_score += (int)log2(merger) * merger * 2;
                    flag = 1;
                    break;
                }
            }while(--ch >= 0);

            for (register int j = SIZE - 2; j >= 0; j--)
            {
                if (*dir_board[i][j] && (!*dir_board[i][j + 1]))
                {
                    dummy = *dir_board[i][j];
                    *dir_board[i][j] = 0;
                    *dir_board[i][j + 1] = dummy;
                }
            }

        }
        else if (k == 1){
            *dir_board[i][SIZE - 1] = nums[0];
        }
        else continue;

    }
    if(flag) (*score).cur_combo += 1;
    else (*score).cur_combo = 0;
    if((*score).combo < (*score).cur_combo) (*score).combo = (*score).cur_combo;

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
    
    for(register int i = 0; i < SIZE; i++){
        for(register int j = 0; j < SIZE; j++){
            if(!board[i][j]) k--;
            if(!k) board[i][j] = (rand()%2)? 2 : 4;
        }
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

    for (register int x = 0; x < SIZE; x++)
    {
        for (register int y = 0; y < SIZE; y++)
        {
            if (board[x][y] == 2048) return WIN;
        }
    }


    if (!blank(board))
    {
        /* if board is not blank, check adjacent cells are identical. */
        for (register int x = 0; x < SIZE; x++)
        {
            for (register int y = 0; y < SIZE; y++)
            {
                for (register int dx = (x > 0 ? -1 : 0); dx <= (x < SIZE - 1 ? 1 : 0); ++dx)
                {
                    for (register int dy = (y > 0 ? -1 : 0); dy <= (y < SIZE - 1 ? 1 : 0); ++dy)
                    {
                        if (dx + dy == 1 || dx + dy == -1)
                        {
                            if (board[x][y] == board[x + dx][y + dy]) return 0;
                        }
                    }
                }
            }
        }

        return LOSE;
    }

    return 0;
}

void add_ranking(SCORE record){
    /* open a txt file and add score*/
    FILE *fp, *out;
    fp = fopen("save.txt", "r+");
    out = fopen("tmp.txt", "w+");
    SCORE tmp;
    int flag = 0;

    if(!fp || !out){
        fprintf(stderr, "Failed to open save files.");
        if(_getch()) exit(1);
    }

    while (fscanf(fp, "%s %d %d %ld %d %d\n", tmp.name,  &tmp.isWin, &tmp.moves, &tmp.Total_Time, &tmp.combo, &tmp.Total_score) == 6)
    {
        if (flag)
            fprintf(out, "%s %d %d %ld %d %d\n", tmp.name, tmp.isWin, tmp.moves, tmp.Total_Time, tmp.combo, tmp.Total_score);
        else
        {
            if (tmp.isWin < record.isWin)
            {
                fprintf(out, "%s %d %d %ld %d %d\n", record.name, record.isWin, record.moves, record.Total_Time, record.combo, record.Total_score);
                flag = 1;
            }
            else if (tmp.Total_score < record.Total_score)
            {
                fprintf(out, "%s %d %d %ld %d %d\n", record.name, record.isWin, record.moves, record.Total_Time, record.combo, record.Total_score);
                flag = 1;
            }
            fprintf(out, "%s %d %d %ld %d %d\n", tmp.name, tmp.isWin, tmp.moves, tmp.Total_Time, tmp.combo, tmp.Total_score);
        }
    }
    /*if there is no record in fp*/
    if(!flag) fprintf(out, "%s %d %d %ld %d %d\n", record.name, record.isWin, record.moves, record.Total_Time, record.combo, record.Total_score);

    fclose(fp);
    fclose(out);
    remove("save.txt");
    if (rename("tmp.txt", "save.txt"))
    {
        printf("Rename Error");
        if(_getch()) return;
    }
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
    int dummy; 

    printf("%d combo, %d moves, %d score, %d : %d ...\n", score.combo, score.moves, score.Total_score, remain / 60, remain % 60);

    for (row = 0; row < SIZE; row++)
    {
        printf("---------------------\n|");
        for (columns = 0; columns < SIZE; columns++)
        {
            if(!(dummy = board[row][columns])) printf("    |");
            else printf("%4d|", dummy);
        }
        printf("\n");
    }

    printf("---------------------\n");

    printf(">>>");
    return;
}

int in_game(){
    /* From start to ranking */
    int ch, wl;
    SCORE new_score;
    new_score.combo = 0;
    new_score.cur_combo = 0;
    new_score.moves = 0;
    new_score.start = clock();
    new_score.Total_score = 0;
    int board[SIZE][SIZE] = {0};
    int prev_board[SIZE][SIZE] = {0};

    int *dir_board[SIZE][SIZE] = {0};
    
    random_gen(board);
    for(register int i = 0; i < SIZE; i++){
        for(register int j = 0; j < SIZE; j++){
            prev_board[i][j] = board[i][j];
        }
    }

    while(1){
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
        if (wl = check(board, prev_board, &new_score)) break;
    }


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
    FILE *fp;
    SCORE tmp;
    fp = fopen("save.txt","r+");
    if(!fp){
        fprintf(stderr, "Failed to open save.txt");
        if(_getch())
            return 1;
    }

    printf("=======================================================\n");
    printf("|name    |W or L  |moves   |Time    |combos  |score   |\n");
    printf("-------------------------------------------------------\n");
    while(!feof(fp)){

        if(fscanf(fp, "%s %d %d %ld %d %d\n", tmp.name, &tmp.isWin, &tmp.moves, &tmp.Total_Time, &tmp.combo, &tmp.Total_score) != 6) break;
        else printf("|%8s|%8s|%8d|%8d|%8d|%8d|\n", tmp.name, (tmp.isWin == WIN)? "Win" : "Lose", tmp.moves, (int) tmp.Total_Time / CLOCKS_PER_SEC, tmp.combo, tmp.Total_score);
    }
    printf("=======================================================\n");


    fclose(fp);
    printf("press e to exit.");
    while (1)
    {
        if (_getch() == 'e')
            return 1;
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
                        if(_getch()) flag = 0;
                    }
                    break;
                case 'b':
                    if(!how_to()){
                        printf("Unusual behavior.");
                        if(_getch()) flag = 0;
                    }
                    break;
                case 'c':
                    if(!show_ranking()){
                        printf("Unusual behavior");
                        if(_getch()) flag = 0;
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
