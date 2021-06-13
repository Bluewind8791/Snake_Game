#include <stdio.h>
#include <windows.h>    //Sleep, gotoxy include
#include <conio.h>      //getch, kbhit include
#include <stdlib.h>     //system include
#include <time.h>


////////////////////////--define--////////////////////////
#define true 1
#define false 0

#define GAME_X 11       // gaming area width value
#define GAME_Y 23       // gaming area depth value
#define MAIN_X 5        // gaming right board position
#define MAIN_Y 1        // gaming right board position

// Keyboard value define
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define p 112
#define P 80


#define WALL 1
#define EMPTY 0
#define CEIL -1
#define ACTIVE_BLOCK -2
#define INACT_BLOCK 2

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
int block_type;
int block_rotation;

int game_board[GAME_X][GAME_Y];         //gaming board



//personal function
void gotoxy(short x, short y);
void HideCursor(char show);

void PrintMenu();
void Reset();
void ResetMain();
void PrintMap();
void PrintMain();


////////////////////////////////////////////////////////
////////////////////////--main--////////////////////////
int main() 
{
    HideCursor(0);                  //0 = unvisiable
    
    PrintMenu();
    Reset();

    while(1) {
        PrintMain();
    }

    return 0;
}
/////////////////////////////////////////////////////////////
////////////////////////--Function--////////////////////////
void PrintMenu() {
    int x=5, y=4;
    int cnt;
    
    gotoxy(x,y+0);  printf("■□□□■■■□□■■□□■■");  Sleep(50);
    gotoxy(x,y+1);  printf("■■■□  ■□□    ■■□□■");   Sleep(50);
    gotoxy(x,y+2);  printf("□□□■              □■  ■"); Sleep(50);
    gotoxy(x,y+3);  printf("■■□■■  □  ■  □□■□□");   Sleep(50);
    gotoxy(x,y+4);  printf("■■□■□□□■■■□■■□□");  Sleep(50);

    gotoxy(x+9, y+2);   printf("T E T R I S");  Sleep(50);

    gotoxy(x,y+9);
    printf("  △   : Shift");    Sleep(50);
    gotoxy(x,y+10);
    printf("◁  ▷ : Left / Right");  Sleep(50);
    gotoxy(x,y+11);
    printf("  ▽   : Soft Drop");    Sleep(50);
    gotoxy(x,y+12);
    printf(" SPACE : Hard Drop");   Sleep(50);
    gotoxy(x,y+13);
    printf("  ESC  : Quit");    Sleep(50);

    for(cnt=0; ; cnt++) {       //blinking
        if(kbhit()) break;
        if( (cnt%100) == 0) {
            gotoxy(x,y+7);  printf(" < PRESS ANY KEY TO START >");
        }
        if( (cnt%100-50)==0 ) {
            gotoxy(x,y+7);  printf(" <                        >");
        }
        Sleep(10);
    }

    //while(kbhit())  getch();        // ??
}

void Reset()
{    
    system("cls");         //printing clear
    PrintMain();
    ResetMain();           // Reset main board

    PrintMap();

    while(1) {
    }
    
}


void ResetMain()
{
    int i, j;

    for(i=0; i<GAME_Y; i++) {           // gaming board reset 0
        for(j=0; j<GAME_X; j++) {
            game_board[i][j] = 0;
        }
    }

    for(i=1; i<GAME_Y-1; i++) {           // making L,R wall at the gaming board
        game_board[i][0] = WALL;
        game_board[i][GAME_X-1] = WALL;
    }
    for(j=0; j<GAME_X; j++) {           // making Bottom wall at the gaming board
        game_board[GAME_Y-1][j] = WALL;
    }
}


void PrintMap() 
{               // printing gaming board
    int i, j;

    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            gotoxy(MAIN_X+j, MAIN_Y+i);
            switch(game_board[i][j]) {
                case WALL:
                    printf("▩");
                    break;
                case EMPTY:
                    printf("  ");
                    break;
            }
        }
    }
    
}

void PrintMain()
{
    int y = 3;

    gotoxy(GAME_X+MAIN_X+1, y);    printf(" LEVEL : ");
    gotoxy(GAME_X+MAIN_X+1, y+1);  printf(" GOAL : ");
    gotoxy(GAME_X+MAIN_X+1, y+2);  printf("+-  N E X T  -+ ");
    gotoxy(GAME_X+MAIN_X+1, y+3);  printf("|             | ");
    gotoxy(GAME_X+MAIN_X+1, y+4);  printf("|             | ");
    gotoxy(GAME_X+MAIN_X+1, y+5);  printf("|             | ");
    gotoxy(GAME_X+MAIN_X+1, y+6);  printf("|             | ");
    gotoxy(GAME_X+MAIN_X+1, y+7);  printf("+-- -  -  - --+ "); 
    gotoxy(GAME_X+MAIN_X+1, y+8);  printf(" YOUR SCORE :"); 

    gotoxy(GAME_X+MAIN_X+1, y+15);  printf("  △   : Shift        SPACE : Hard Drop");     
    gotoxy(GAME_X+MAIN_X+1, y+16);  printf("◁  ▷ : Left / Right   P   : Pause");     
    gotoxy(GAME_X+MAIN_X+1, y+17);  printf("  ▽   : Soft Drop     ESC  : Quit");
}







void gotoxy(short x, short y) {     // gotoxy default setting
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void HideCursor(char show)     // hiding cursor
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}
