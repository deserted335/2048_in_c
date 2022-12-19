#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define SIZE 4
#define DIR_W 1
#define DIR_A 2
#define DIR_S 3
#define DIR_D 4 


typedef struct score{
    char name;
    int isWin;
    int moves;
    int combo;
}SCORE;

int board[4][4] = {0};
int prev_board[4][4] = {0};

void action(int ***dir_board, SCORE *score){
    /* along the w/a/s/d direction board, move and merge numbers. */
    /* move numbers */
    int nums[SIZE];
    int k = 0, dummy, ch, flag = 1;
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
        flag = 0;
        while(!flag){
            if(*dir_board[i][ch] == *dir_board[i][ch+1] && !*dir_board[i][ch]){
                *dir_board[i][ch+1] += *dir_board[i][ch];
                *dir_board[i][ch] = 0;
                flag = 1;
            }
            if(++ch >= 3) break;
        }
        (flag) ? (*score).combo += 1 : (*score).combo = 0;
    }
    return;
}

int ***dir_board(int **board, int dir){
    /* pointer array of board, corresponding to w/a/s/d */
    int i, j;
    int ***res[SIZE][SIZE];
    switch (dir){
        case DIR_W:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    res[i][j] = &board[3 - j][i];
            break;
        case DIR_A:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    res[i][j] = &board[i][3-j];
            break;

        case DIR_S:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    res[i][j] = &board[j][i];
            break;

        case DIR_D:
            for(i = 0; i < SIZE; i++)
                for(j = 0; j < SIZE; j++)
                    res[i][j] = &board[i][j];
            break;
    }
    return res;
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

int in_game(){
    printf("this is in game. press any key to exit\n");
    int ch;
    while(1){
        while(ch = _getch()) return 1;
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
        while(ch = _getch()) return 1;
    }
    return 0;
}

int show_ranking(){
    int ch;
    printf("this is ranking. press any key to exit\n");
    while(1){
        while(ch = _getch()) return 1;
    }
    return 0;
}


int main(void){
    /*keyboard press -> update screen*/    
    int ch;
    int flag = 1;
    while(flag){
        system("cls");
        main_menu();
        while (ch = _getch())
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
