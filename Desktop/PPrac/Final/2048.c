#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

typedef struct score{
    char name;
    int isWin;
    int moves;
    int combo;
}SCORE;


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
