#include <stdio.h>
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

#define WALL 1
#define EMPTY 0

#define GAME_X 22
#define GAME_Y 22
#define ADJ_X 4
#define ADJ_Y 2


int game_area[GAME_Y][GAME_X];
int n_tail = 2;
int tail_direction = LEFT;
int snake_x = GAME_X + ADJ_X;
int snake_y = GAME_Y/2 + ADJ_Y;

void gotoxy(int x, int y);
void Init();
void PrintGameArea();


int main()
{
    Init();
    PrintGameArea();
    
    gotoxy(snake_x, snake_y);
    printf("¡ß");
    for(int i=1; i<=n_tail; i++) {
        if(tail_direction == LEFT) {
            gotoxy(snake_x - i*2, snake_y);
            printf("¡ß");
        }
        else if(tail_direction == RIGHT) {
            gotoxy(snake_x + i*2, snake_y);
            printf("¡ß");
        }
        else if(tail_direction == DOWN) {
            gotoxy(snake_x, snake_y + i);
            printf("¡ß");
        }
        else if(tail_direction == UP) {
            gotoxy(snake_x, snake_y - i);
            printf("¡ß");
        }
    }

    while(true) {

    }

    return 0;
}

void PrintGameArea()
{
    for(int i=0; i<GAME_X; i++) {
        for(int j=0; j<GAME_Y; j++) {
            if(i == 0 || i == GAME_Y-1) {
                gotoxy(j*2+ADJ_X, i+ADJ_Y);
                printf("¡á");
            }
            if(j == 0 || j == GAME_X-1) {
                gotoxy(j*2+ADJ_X, i+ADJ_Y);
                printf("¡á");
            }
        }
    }
}

void Init() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	srand(time(NULL));
}

void gotoxy(int x, int y) {
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}