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
#define HEAD 2
#define TAIL 3

#define GAME_X 22
#define GAME_Y 22
#define ADJ_X 4
#define ADJ_Y 2


int game_area[GAME_X][GAME_Y];  // game area
int x[100], y[100];             // snake x, y position
int length;                     // snake length include head and tail
int dir;                        // snake direction
int key;                        
int speed;
int food_x;
int food_y;
int score;
int best_score;


void gotoxy(int x, int y);
void Init();
void PrintGameArea();
void MakeSnake();
void ResetGameArea();
void ResetGame();
void CheckKey();
void MovingSnake();
void Food();
void PrintTitle();
void GameOver();
void PrintInfo();
void Pause();


////            MAIN FUNCTION              ////
int main()
{
    Init();
    PrintTitle();
    ResetGame();

    while(true) {
        for(int i=0; i<5; i++) {
            CheckKey();
            MovingSnake();
            Sleep(speed);
        }
    }

    return 0;
}
////            MAIN FUNCTION END              ////


void Init() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}


void PrintTitle()
{
    printf("\n\n");
    printf("  　　　　　■■■　　　　　　　　　　　　　　　　　　　　　　　　　　　　　　■■         \n");
    printf("  　　■■■■■■　　　　　　　　　　　　　　　　　■■■　　　　　■■　　■■　　　　　　■■    \n");
    printf("  　■■■　　　　　　　　■■　　■■■　　　　　■■■■　　　　　■■　■■■　　　■■■■■          \n");
    printf("  　■■　　　　　　　　　■■■　■■　　　　　　■■■■　　　　　■■■■■　　　　　■■          \n");
    printf("  　■■■　　　　　　　■■■■　■■　　　　　■■　■■　　　　　■■■　　　　　　■■■■■        \n");
    printf("  　　　■■■　　　　　■■■■　■■　　　　　■■■■■■　　　■■■　　　　　　■■■■■           \n");
    printf("  　　　　■■■　　　　■■　■■■　　　■■■■■　■■　　　　■■■　　　　　　　■■　　　       \n");
    printf("  　　　　■■　　　　■■　　■■■　　　　■■　　　■■　　　■■■■■■■　　　　■■　　■        \n");
    printf("  ■■■■■　　　　　■■　　■■■　　　■■■　　　■■　　　■■　　　■■　　　■■■■■■　         \n");
    printf("  ■■■　　　　　　　　　　　　　　　　　■■　　　■■■　　　　　　　　　　　　　　■■■■");      
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+11); printf("    < PRESS ANY KEY TO START >");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+13+1); printf("+------- HOW TO PLAY GAME -------+");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+14+1); printf("|                                |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+15+1); printf("|     △   : Move Up             |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+16+1); printf("|   ◁  ▷ : Move Left / Right   |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+17+1); printf("|     ▽   : Move Down           |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+18+1); printf("|                                |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+19+1); printf("|  ESC : Quit Game / P : Pause   |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+20+1); printf("|                                |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+21+1); printf("+--------------------------------+");

    while(true) {
        if(kbhit()) { // if key input
            key = getch();
            if(key == ESC) exit(0); // if key is ESC, exit program
            else break; // else blink
        }
        gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+11); printf("    < PRESS ANY KEY TO START >");
        Sleep(400);
        gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+11); printf("                              ");
        Sleep(400);
    }
}


void ResetGame()
{
    FILE *file=fopen("score.dat", "rt"); // score.dat 파일을 연결 write text mode
    if (file==0) { best_score = 0; } //파일이 없으면 걍 최고점수에 0을 넣음 
    else {
        fscanf(file,"%d", &best_score); // 파일이 열리면 최고점수를 불러옴 
        fclose(file); //파일 닫음 
    }

    length = 5;
    dir = LEFT;
    key = 0;
    speed = 200;
    score = 0;

    system("cls");
    ResetGameArea();
    PrintGameArea();
    MakeSnake();
    Food();
    PrintInfo();
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


void Food()
{
    int food_crush_flag = false;
    int random = 0; // for seed random
    
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+6); printf(" YOUR SCORE : %4d", score);
    
    while(true) {
        food_crush_flag = false;
        srand((unsigned)time(NULL) + random);
        food_x = (rand() % (GAME_X - 2)) + 1;
        food_y = (rand() % (GAME_Y - 2)) + 1;

        for(int i=0; i<length; i++) {
            if(food_x == x[i] && food_y == y[i]) {  // 뱀이 먹었는지 확인
                food_crush_flag = true;
                random++;
                break;
            }
        }

        if(food_crush_flag == true) continue;  // 부딛치면 while문 다시 시작
        
        gotoxy(ADJ_X + food_x, ADJ_Y + food_y); printf("♣"); // print food
        break;
    }
}


void PrintInfo()
{
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+2); printf("[ S N A K E   G A M E ]");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+5); printf(" BEST SCORE : %4d", best_score);
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+6); printf(" YOUR SCORE : %4d", score);
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+8); printf("   △   : Move Up");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+9); printf(" ◁  ▷ : Move Left / Right");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+10); printf("   ▽   : Move Down");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+12); printf(" ESC : Quit Game");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+13); printf("  P  : Pause");
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
        else {  // if key is not dirction key
            switch(key) {
                case P :
                case p :
                    Pause();
                    break;
                case ESC :
                    system("cls");
                    exit(true);
                    break;
            }
        }
    }
}


void Pause()
{
    while(true) {
        if(kbhit()) { // if keyboard hit  (P 누르면 제대로 안됨)
            gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+16); printf("                    ");
            break;
        }
        gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+16); printf(" ※ P A U S I N G ※");
        Sleep(400);
        gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+16); printf("                    ");
        Sleep(400);
    }
}


void MovingSnake()
{
    int i;

    // 음식과 충돌했는지 검사
    if(x[0] == food_x && y[0] == food_y) {
        score += (250 - speed); // 점수 증가
        speed -= 5; // 속도 증가
        Food();
        length++;   // 몸 길이 증가
        x[length-1] = x[length-2];  
        y[length-1] = y[length-2];
    }
    
    // 벽과 충돌했는지 검사
    if(x[0] == 0 || x[0] == GAME_X-1 || y[0] == GAME_Y-1 || y[0] == 0) {
        GameOver();
        return;
    }

    // 자신과 충돌했는지 검사
    for(i=1; i<length; i++) { //자기몸과 충돌했는지 검사 
        if(x[0] == x[i] && y[0] == y[i]) {
            GameOver();
            return;
        }
    }
    
    // 마지막 꼬리 삭제
    gotoxy(ADJ_X + x[length-1], ADJ_Y + y[length-1]); printf("  ");

    // 몸통 좌표 한칸씩 이동
    for(i=length-1; i>0; i--) {
        x[i] = x[i-1];
        y[i] = y[i-1];
    }

    // 머리 지우고 그 자리에 꼬리 출력
    gotoxy(ADJ_X + x[0], ADJ_Y + y[0]); printf("■");
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
    gotoxy(ADJ_X + x[i], ADJ_Y + y[i]); printf("◆"); // print head
}


void GameOver()
{
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+0); printf("▶▶▶▶▶▶▶▶▶▶▶▶▶▶▶▶▶");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+1); printf("▲                              ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+2); printf("▲       G A M E   O V E R      ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+3); printf("▲                              ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+4); printf("▲                              ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+5); printf("▲    Your Score : %4d         ▼", score);
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+6); printf("▲                              ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+7); printf("▲   Press any key to RESTART   ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+8); printf("▲                              ▼");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+9); printf("◀◀◀◀◀◀◀◀◀◀◀◀◀◀◀◀◀");

    if (score > best_score) {
        best_score = score;
        gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+4); printf("▲       ☆ BEST SCORE ☆       ▼");
    }

    while(true) {
        if(kbhit()) { // if key input
            key = getch();
            if (key == ESC) exit(0); // if key is ESC, exit program
            else if (key != ESC) {
                ResetGame();
                return;
            }
            else break; // else blink
        }
        gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+7); printf("▲   Press any key to RESTART   ▼");
        Sleep(400);
        gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+7); printf("▲                              ▼");
        Sleep(400);
    }
}


void gotoxy(int x, int y) {
	COORD pos;
	pos.X = x * 2;  // 가로는 두칸씩 이동
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}