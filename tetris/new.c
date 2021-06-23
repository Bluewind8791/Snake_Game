#include <stdio.h>
#include <windows.h>    //Sleep, gotoxy include
#include <conio.h>      //getch, kbhit include
#include <stdlib.h>     //system include
#include <time.h>


// Keyboard value define
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define p 112
#define P 80
#define SPACE 32

// drawing value define
#define WALL 1
#define EMPTY 0
#define CEILING -1
#define ACTIVE_BLOCK -2
#define INACTIVE_BLOCK 2

// 위 20줄은 가려놓아 플레이어는 가로 10칸, 세로 20줄을 보며 플레이한다
#define GAME_X 10*2
#define GAME_Y 20
#define POS_ADJ_X 2*2       // pos = position
#define POS_ADJ_Y 2


////////////////////////--Global Variable--////////////////////////
int blocks[7][4][4][4] = {
    {{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
     {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}},
    {{0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0},
     {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},{0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0}},
    {{0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0},
     {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},{0,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0}},
    {{0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0},
     {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},{0,0,0,0,1,0,0,0,1,1,0,0,0,1,0,0}},
    {{0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0},{0,0,0,0,1,1,0,0,0,1,0,0,0,1,0,0},
     {0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,0,1,1,0}},
    {{0,0,0,0,1,0,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,0,0,1,1,0,0},
     {0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0},{0,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0}},
    {{0,0,0,0,0,1,0,0,1,1,1,0,0,0,0,0},{0,0,0,0,0,1,0,0,0,1,1,0,0,1,0,0},
     {0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0},{0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0}}
};  //4x4 2차원 배열에 블록을 표현, 7가지 종류의 블럭이 4방향 회전
    // blocks[block_type][block_rotation][i][j]

// ※ 록 딜레이(Lock Delay, 테트로미노가 지면에 닿고 붙어버리기까지 걸리는 시간)은 0.5초로 한다.

int game_area[GAME_X][GAME_Y];
int game_area_copy[GAME_X][GAME_Y];
int block_type;
int block_next_type;
int block_spin;
int blockX, blockY;


// hiding cursor enum
typedef enum { NOCURSOR, SOLIDCURSOR, NORMALCURSOR } CURSOR_TYPE; 
// hiding cursor function
void setcursortype(CURSOR_TYPE c) {
     CONSOLE_CURSOR_INFO CurInfo;
 
     switch (c) {
     case NOCURSOR:
          CurInfo.dwSize=1;
          CurInfo.bVisible=FALSE;
          break;
     case SOLIDCURSOR:
          CurInfo.dwSize=100;
          CurInfo.bVisible=TRUE;
          break;
     case NORMALCURSOR:
          CurInfo.dwSize=20;
          CurInfo.bVisible=TRUE;
          break;
     }
     SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}
void gotoxy(short x, short y) {     // gotoxy default setting
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void PrintGameArea();


int main()
{
    srand(time(NULL));              // random seed
    setcursortype(NOCURSOR);        // hiding cursor

    
    while(1) {
        PrintGameArea();
    }

    return 0;
}

void PrintGameArea()
{
    int i, j;

    // game area - both side wall
    for(i=0; i<GAME_Y; i++) {
        game_area[i][0] = WALL;
        game_area[i][GAME_X-2] = WALL;
    }
    // game area - bottom side wall
    for(j=2; j<GAME_X-2; j++) {
        game_area[GAME_Y-1][j] = WALL;
    }
    // game area - ceiling
    for(j=2; j<GAME_X-2; j++) {
        game_area[2][j] = CEILING;
    }

    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            gotoxy(POS_ADJ_X+j, POS_ADJ_Y+i);
            switch(game_area[i][j]) {
                case WALL:
                    printf("▩");
                    break;
                case EMPTY:
                        printf("  ");
                        break;
                case CEILING:
                    printf(". ");
                    break;
                case INACTIVE_BLOCK:
                    printf("□");
                    break;
                case ACTIVE_BLOCK:
                    printf("■");
                    break;
            }
        }
    }
}