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

enum {
    BLACK,
    DARK_BLUE,
    DARK_GREEN,
    DARK_SKYBLUE,
    DARK_RED,
    DARK_VOILET,
    DARK_YELLOW,
    GRAY,
    DARK_GRAY,
    BLUE,
    GREEN,
    SKYBLUE,
    RED,
    VIOLET,
    YELLOW,
    WHITE,
};

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
void PrintField();
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
void SetColor(unsigned short text);


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
    system("mode con:cols=110 lines=30");
    SetConsoleTitle("SNAKE GAME v1.0");
}


void PrintTitle()
{
    printf("\n\n");
    printf("  �������������ᡡ�������������������������������������������������������������         \n");
    printf("  ����������ᡡ�����������������������������������ᡡ����������ᡡ����ᡡ�������������    \n");
    printf("  �����ᡡ����������������ᡡ�����ᡡ������������ᡡ����������ᡡ���ᡡ����������          \n");
    printf("  ����ᡡ�������������������ᡡ��ᡡ��������������ᡡ�������������ᡡ�����������          \n");
    printf("  �����ᡡ����������������ᡡ��ᡡ����������ᡡ��ᡡ�����������ᡡ����������������        \n");
    printf("  ���������ᡡ������������ᡡ��ᡡ��������������ᡡ�������ᡡ����������������           \n");
    printf("  �����������ᡡ��������ᡡ���ᡡ���������ᡡ��ᡡ���������ᡡ��������������ᡡ����       \n");
    printf("  ����������ᡡ��������ᡡ�����ᡡ��������ᡡ������ᡡ�����������ᡡ��������ᡡ����        \n");
    printf("  �����ᡡ����������ᡡ�����ᡡ�������ᡡ������ᡡ������ᡡ������ᡡ����������ᡡ         \n");
    printf("  ���ᡡ����������������������������������ᡡ�������ᡡ�������������������������������");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+11); printf("    < PRESS ANY KEY TO START >");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+13+1); printf("+------- HOW TO PLAY GAME -------+");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+14+1); printf("|                                |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+15+1); printf("|     ��   : Move Up             |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+16+1); printf("|   ��  �� : Move Left / Right   |");
    gotoxy(ADJ_X+(GAME_X/2), ADJ_Y+17+1); printf("|     ��   : Move Down           |");
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
    FILE *file=fopen("score.dat", "rt"); // score.dat ������ ���� write text mode
    if (file==0) { best_score = 0; } //������ ������ �� �ְ������� 0�� ���� 
    else {
        fscanf(file,"%d", &best_score); // ������ ������ �ְ������� �ҷ��� 
        fclose(file); //���� ���� 
    }

    length = 5;
    dir = LEFT;
    key = 0;
    speed = 200;
    score = 0;

    system("cls");
    ResetGameArea();
    PrintField();
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


void PrintField()
{
    for(int i=0; i<GAME_X; i++) {
        for(int j=0; j<GAME_Y; j++) {
            gotoxy(ADJ_X + j, ADJ_Y + i);
            if(game_area[i][j] == WALL) {
                printf("��");
            }
        }
    }
}


void MakeSnake()
{
    for(int i=0; i<length; i++) { // �� ���밪 �Է� 
        x[i] = GAME_X / 2 + i;
        y[i] = GAME_Y / 2;
        gotoxy(ADJ_X + x[i], ADJ_Y + y[i]);
        printf("��");
    }
    gotoxy(ADJ_X + x[0], ADJ_Y + y[0]); // �� �Ӹ� �׸� 
    printf("��");
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
            if(food_x == x[i] && food_y == y[i]) {  // ���� �Ծ����� Ȯ��
                food_crush_flag = true;
                random++;
                break;
            }
        }

        if(food_crush_flag == true) continue;  // �ε�ġ�� while�� �ٽ� ����
        
        gotoxy(ADJ_X + food_x, ADJ_Y + food_y); printf("��"); // print food
        break;
    }
}


void PrintInfo()
{
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+2); printf("[ S N A K E   G A M E ]");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+5); printf(" BEST SCORE : %4d", best_score);
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+6); printf(" YOUR SCORE : %4d", score);
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+8); printf("   ��   : Move Up");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+9); printf(" ��  �� : Move Left / Right");
    gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+10); printf("   ��   : Move Down");
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
        if(kbhit()) { // if keyboard hit  (P ������ ����� �ȵ�)
            gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+17); printf("                              ");
            gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+16); printf("                    ");
            break;
        }
        gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+17); printf("Please Press Any Key to Resume");
        gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+16); printf(" �� P A U S I N G ��");
        Sleep(400);
        gotoxy((ADJ_X*2)+GAME_X-2, ADJ_Y+16); printf("                    ");
        Sleep(400);
    }
}


void MovingSnake()
{
    int i;

    // ���İ� �浹�ߴ��� �˻�
    if(x[0] == food_x && y[0] == food_y) {
        score += (250 - speed); // ���� ����
        speed -= 5; // �ӵ� ����
        Food();
        length++;   // �� ���� ����
        x[length-1] = x[length-2];  
        y[length-1] = y[length-2];
    }
    
    // ���� �浹�ߴ��� �˻�
    if(x[0] == 0 || x[0] == GAME_X-1 || y[0] == GAME_Y-1 || y[0] == 0) {
        GameOver();
        return;
    }

    // �ڽŰ� �浹�ߴ��� �˻�
    for(i=1; i<length; i++) { //�ڱ���� �浹�ߴ��� �˻� 
        if(x[0] == x[i] && y[0] == y[i]) {
            GameOver();
            return;
        }
    }
    
    // ������ ���� ����
    gotoxy(ADJ_X + x[length-1], ADJ_Y + y[length-1]); printf("  ");

    // ���� ��ǥ ��ĭ�� �̵�
    for(i=length-1; i>0; i--) {
        x[i] = x[i-1];
        y[i] = y[i-1];
    }

    // �Ӹ� ����� �� �ڸ��� ���� ���
    gotoxy(ADJ_X + x[0], ADJ_Y + y[0]); printf("��");
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
    gotoxy(ADJ_X + x[i], ADJ_Y + y[i]); printf("��"); // print head
}


void GameOver()
{
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+0); printf("����������������������������������");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+1); printf("��                              ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+2); printf("��       G A M E   O V E R      ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+3); printf("��                              ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+4); printf("��                              ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+5); printf("��    Your Score : %4d         ��", score);
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+6); printf("��                              ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+7); printf("��   Press any key to RESTART   ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+8); printf("��                              ��");
    gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+9); printf("����������������������������������");

    if (score > best_score) {
        best_score = score;
        gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+4); printf("��       �� BEST SCORE ��       ��");
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
        gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+7); printf("��   Press any key to RESTART   ��");
        Sleep(400);
        gotoxy(ADJ_X+(GAME_X/2)-9, ADJ_Y+(GAME_Y/3)-1+7); printf("��                              ��");
        Sleep(400);
    }
}

void SetColor(unsigned short text)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

void gotoxy(int x, int y) {
	COORD pos;
	pos.X = x * 2;  // ���δ� ��ĭ�� �̵�
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}