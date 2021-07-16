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

#define WALL 1
#define EMPTY 0
#define HEAD 2
#define TAIL 3

#define GAME_X 22
#define GAME_Y 22
#define ADJ_X 4
#define ADJ_Y 2


int game_area[GAME_X][GAME_Y];
int x[100], y[100];
int length;                     // 뱀 몸통 길이
int dir;                        // snake direction
int food_x;
int food_y;
int key;
int speed;


void gotoxy(int x, int y);
void Init();
void PrintGameArea();
void MakeSnake();
void ResetGameArea();
void ResetGame();
void CheckKey();
void MovingSnake();


int main()
{
    Init();
    ResetGame();
    PrintGameArea();
    MakeSnake();

    while(true) {
        for(int i=0; i<5; i++) {
            CheckKey();
            MovingSnake();
            Sleep(speed);
        }
    }

    return 0;
}


void ResetGame()
{
    system("cls");
    length = 5;
    dir = LEFT;
    key = 0;
    speed = 200;
    
    ResetGameArea();
}



void CheckKey()
{
    key = 0;        // reset key

    if(kbhit()) {   // if key input
        key = getch();
        if(key == 224) {    // if key is direction key
            do {key = getch();}
            while(key == 224) ;

            switch(key) {
                case LEFT :
                    if(dir == RIGHT) {
                        CheckKey();
                        break;
                    }
                    else dir = LEFT;
                    break;
                case RIGHT :
                    if(dir == LEFT) {
                        CheckKey();
                        break;
                    }
                    else dir = RIGHT;
                    break;
                case DOWN :
                    if(dir == UP) {
                        CheckKey();
                        break;
                    }
                    else dir = DOWN;
                    break;
                case UP :
                    if(dir == DOWN) {
                        CheckKey();
                        break;
                    }
                    else dir = UP;
                    break;
            }
        }
    }
}


void MovingSnake()
{
    int i;

    // 음식과 충돌했는지 검사
    // 벽과 충돌했는지 검사
    if(x[0] == 0 || x[0] == GAME_X-1 || y[0] == GAME_Y-1 || y[0] == 0) {
        // game over
        return;
    }

    // 자신과 충돌했는지 검사
    for(i=1; i<length; i++) { //자기몸과 충돌했는지 검사 
        if(x[0] == x[i] && y[0] == y[i]) {
            // game over
            return;
        }
    }
    
    // 마지막 꼬리 삭제
    gotoxy(ADJ_X + x[length-1], ADJ_Y + y[length-1]);
    printf("  ");

    // 몸통 좌표 한칸씩 이동
    for(i=length-1; i>0; i--) {
        x[i] = x[i-1];
        y[i] = y[i-1];
    }

    // 머리 지우고 그 자리에 꼬리 출력
    gotoxy(ADJ_X + x[0], ADJ_Y + y[0]);
    printf("■");

    switch(dir) {
        case LEFT:
            --x[0];
            break;
        case RIGHT:
            ++x[0];
            break;
        case UP:
            --y[0];
            break;
        case DOWN:
            ++y[0];
            break;
    }

    gotoxy(ADJ_X + x[i], ADJ_Y + y[i]);
    printf("◆");
}


void MakeSnake()
{
    for(int i=0; i<length; i++) { // 뱀 몸통값 입력 
        x[i] = GAME_X / 2 + i;
        y[i] = GAME_Y / 2;
        gotoxy(ADJ_X + x[i], ADJ_Y + y[i]);
        printf("■");
    }
    gotoxy(ADJ_X + x[0], ADJ_Y + y[0]); // 뱀 머리 그림 
    printf("◆");
}


void ResetGameArea()
{
    int i, j;
    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            game_area[i][j] = EMPTY;     // reset game_area
        }
    }
    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            if(i == 0 || i == GAME_Y-1) {
                game_area[i][j] = WALL;
            }
            if(j == 0 || j == GAME_X-1) {
                game_area[i][j] = WALL;
            }
        }
    }
}


void PrintGameArea()
{
    for(int i=0; i<GAME_X; i++) {
        for(int j=0; j<GAME_Y; j++) {
            gotoxy(ADJ_X + j, ADJ_Y + i);
            if(game_area[i][j] == WALL) {
                printf("▩");
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
	pos.X = x * 2;  // 가로는 두칸씩 이동
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}