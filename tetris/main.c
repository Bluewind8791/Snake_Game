#include <stdio.h>
#include <windows.h>    //Sleep, gotoxy include
#include <conio.h>      //getch, kbhit include
#include <stdlib.h>     //system include
#include <time.h>


////////////////////////--define--////////////////////////
#define true 1
#define false 0

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
#define CEIL -1
#define ACTIVE_BLOCK -2
#define INACT_BLOCK 2

#define GAME_X 11*2                   // gaming area width value
#define GAME_Y 23                   // gaming area depth value
#define GAME_ADJ_X 3*2                // gaming area adjustment
#define GAME_ADJ_Y 1                // gaming area adjustment
#define MAIN_X GAME_X+GAME_ADJ_X+2    // print information position


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

int game_board[GAME_Y][GAME_X];         // gaming board

int block_type;
int block_rotation;
int block_type_next;
int best_score;
int score;
int level;
int bx, by;           // create block x, y site value
int speed;
int key;

//personal function
void gotoxy(short x, short y);
void HideCursor(char show);

void PrintMenu();
void Reset();
void ResetGame();
void PrintGame();
void PrintInfo();
void NewBlock();
void MoveBlock(int direction);
void Pause();


////////////////////////////////////////////////////////
////////////////////////--main--////////////////////////
int main() 
{
    srand((unsigned)time(NULL));    // random seed
    HideCursor(0);                  // 0 = unvisiable
    
    PrintMenu();
    Reset();

    while(1) {
        for(int i=0; i<10; i++) {
            PrintGame();
            Sleep(speed);
        }
        
    }

    return 0;
}
/////////////////////////////////////////////////////////////
////////////////////////--Function--////////////////////////
void PrintMenu() {
    int x=5, y=4;
    int cnt;
    
    gotoxy(x,y+0); printf("■□□□■■■□□■■□□■■");  Sleep(50);
    gotoxy(x,y+1); printf("■■■□  ■□□    ■■□□■");   Sleep(50);
    gotoxy(x,y+2); printf("□□□■              □■  ■"); Sleep(50);
    gotoxy(x,y+3); printf("■■□■■  □  ■  □□■□□");   Sleep(50);
    gotoxy(x,y+4); printf("■■□■□□□■■■□■■□□");  Sleep(50);

    gotoxy(x+9, y+2);   printf("T E T R I S");  Sleep(50);

    gotoxy(x,y+9);  printf("  △   : Shift");    Sleep(50);
    gotoxy(x,y+10); printf("◁  ▷ : Left / Right");  Sleep(50);
    gotoxy(x,y+11); printf("  ▽   : Soft Drop");    Sleep(50);
    gotoxy(x,y+12); printf(" SPACE : Hard Drop");   Sleep(50);
    gotoxy(x,y+13); printf("  ESC  : Quit");    Sleep(50);

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
    FILE *file = fopen("score.dat", "rt");      // connecting score.dat
    if(file == 0) {best_score = 0;}             // if file non-exist, best score = 0
    else {                                      // if file open
        fscanf(file, "%d", &best_score);        // scan best score
        fclose(file);                           // closing file
    }

    // reset variable
    level = 1;
    score = 0;
    speed = 100;

    system("cls");         //printing clear
    ResetGame();           // Reset main board
    PrintInfo();
    PrintGame();

    NewBlock();
}


void ResetGame()
{
    int i, j;

    for(i=0; i<GAME_Y; i++) {           // gaming board reset 0
        for(j=0; j<GAME_X; j++) {
            game_board[i][j] = 0;
        }
    }
    for(j=2; j<GAME_X; j++) {
        game_board[3][j] = CEIL;
    }
    for(i=0; i<GAME_Y; i++) {
        game_board[i][0] = WALL;        // making Left wall at the gaming board
        game_board[i][GAME_X-1] = WALL; // making Right wall at the gaming board
    }
    for(j=0; j<GAME_X; j++) {           // making Bottom wall at the gaming board
        game_board[GAME_Y-1][j] = WALL;
    }
}


void PrintGame() 
{               // printing gaming board
    int i, j;

    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            gotoxy(GAME_ADJ_X+j, GAME_ADJ_Y+i);
            switch(game_board[i][j]) {
                case WALL:
                    printf("▩");
                    break;
                case EMPTY:
                    printf("  ");
                    break;
                case CEIL:
                    printf(". ");
                    break;
                case INACT_BLOCK:
                    printf("□");
                    break;
                case ACTIVE_BLOCK:
                    printf("■");
                    break;
            }
        }
    }
}

void NewBlock()
{
    int i, j;

    bx = (GAME_X/2) - 1;            // creating block position
    by = 0;
    block_type = block_type_next;   // bringing next block type
    block_type_next = rand() % 7;   // random value next block type
    block_rotation = 0;             // rotation reset

    for(i=0; i<4; i++) {
        for(j=0; j<4; j++) {
            if(blocks[block_type][block_rotation][i][j] == 1) 
                game_board[by+i][bx+j] = ACTIVE_BLOCK;
        }
    }    
}

void CheckKey()
{
    key = 0;                // reset key

    if(kbhit()) {
        key = getch();
        if(key == 224) {    // if key is arrow key
            key = getch();
            switch(key) {
                case LEFT:
                    MoveBlock(LEFT);
                    break;
                case RIGHT:
                    MoveBlock(RIGHT);
                    break;
                case DOWN:
                    MoveBlock(DOWN);
                    break;
                case UP:
                    MoveBlock(UP);
                    break;
            }
        }
        else {          // if key is not arrow key
            switch(key) {
                case SPACE:
                    break;
                case P:
                case p:
                    Pause();
                    break;
                case ESC:
                    system("cls");  // clear monitor
                    exit(0);        // game exit
            }
        }
    }
}

void Pause()
{
    
    getch();        // waiting for input

}

void MoveBlock(int direction)
{
    int i,j;
    switch(direction) {
        case LEFT:
            for(i=0; i<4; i++) {                        // erase block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j] = EMPTY;
                }
            }
            for(i=0; i<4; i++) {                        // go x-- make active block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j-1] = ACTIVE_BLOCK;
                }
            }
            bx--;
            break;
        case RIGHT:
            for(i=0; i<4; i++) {                        // erase block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j] = EMPTY;
                }
            }
            for(i=0; i<4; i++) {                        // go x++ make active block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j+1] = ACTIVE_BLOCK;
                }
            }
            bx++;
            break;
        case DOWN:
            for(i=0; i<4; i++) {                     // erase block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j] = EMPTY;
                }
            }
            for(i=0; i<4; i++) {                      // go y++ make active block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i+1][bx+j] = ACTIVE_BLOCK;
                }
            }
            by++;
            break;
        case UP:
            for(i=0; i<4; i++) {                     // erase block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j] = EMPTY;
                }
            }
            block_rotation = (block_rotation+1) % 4;    // make block spin (+1 and no more 4)
            for(i=0; i<4; i++) {
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_rotation][i][j]==1) 
                        game_board[by+i][bx+j] = ACTIVE_BLOCK;
                }
            }
            break;
    }
}






void PrintInfo(void)
{
    int y = 3;

    gotoxy(MAIN_X, y);    printf(" LEVEL : %3d", level);
    gotoxy(MAIN_X, y+1);  printf("  GOAL : ");
    gotoxy(MAIN_X, y+2);  printf("+-  N E X T  -+ ");
    gotoxy(MAIN_X, y+3);  printf("|             | ");
    gotoxy(MAIN_X, y+4);  printf("|             | ");
    gotoxy(MAIN_X, y+5);  printf("|             | ");
    gotoxy(MAIN_X, y+6);  printf("|             | ");
    gotoxy(MAIN_X, y+7);  printf("+-- -  -  - --+ "); 
    gotoxy(MAIN_X, y+8);  printf(" YOUR SCORE : %5d", score);

    gotoxy(MAIN_X, y+15); printf("  △   : Shift        SPACE : Hard Drop");     
    gotoxy(MAIN_X, y+16); printf("◁  ▷ : Left / Right   P   : Pause");     
    gotoxy(MAIN_X, y+17); printf("  ▽   : Soft Drop     ESC  : Quit");
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
