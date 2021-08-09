#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h> 
#include <stdbool.h>


// Keyboard value define
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ESC 27
#define p 112       // for pause
#define P 80        // for pause

#define WALL 1
#define EMPTY 0

#define GAME_X 22
#define GAME_Y 22
#define ADJ_X 4
#define ADJ_Y 2


int game_field[GAME_X][GAME_Y];  // game area


void gotoxy(int x, int y);
void Init();
void PritTitle();
void PrintField();


int main()
{
    Init();
    PritTitle();

    while(true) {

    }

    return 0;
}

void Init() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    system("mode con:cols=100 lines=50");
    SetConsoleTitle("TETRIS GAME v1.0");
}


void PritTitle() {

}


void PrintField() {
    int i, j;
    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            game_field[i][j] = EMPTY;
        }
    }
}


void gotoxy(int x, int y) {
	COORD pos;
	pos.X = x * 2;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}