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
#define GAME_Y 23                     // gaming area depth value
#define GAME_ADJ_X 3*2                // gaming area adjustment
#define GAME_ADJ_Y 1                  // gaming area adjustment
#define INFO_X GAME_X+GAME_ADJ_X+2    // information position


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
int copy_board[GAME_Y][GAME_X];
int block_type;                         // block type
int block_rotation;                     // block spin
int block_type_next;                    // next block type
int bx, by;                             // create block x, y site value
int speed;                              // block falling speed
int key;                                // keyboard key
int new_block_on = 0;                   // if need new block => 1
int space_key_on = 0;                   // if space key pushed => 1
int crush_on = 0;                       // if block crush => 1
//int best_score;                         // best score
//int score;                              // game score
//int level;                              // game level

//personal function
void gotoxy(short x, short y);  //void HideCursor(char show);
void PrintMenu(void);
void Reset(void);
void ResetGame(void);
void ResetCopyBoard(void);
void PrintGame(void);
void PrintInfo();
void NewBlock();
void MoveBlock(int direction);
void Pause();
void DropBlock();
void CheckKey(void);
int CheckCrush(int bx, int by, int block_rotation);

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


////////////////////////////////////////////////////////
////////////////////////--main--////////////////////////
int main() 
{
    srand((unsigned)time(NULL));    // random seed
    setcursortype(NOCURSOR);        // hiding cursor
    
    PrintMenu();
    Reset();

    while(1) {
        for(int i=0; i<10; i++) {       // block falling 1 block, could insert key 10 times
            CheckKey();
            PrintGame();
            Sleep(speed);               // block falling speed
            if(crush_on && CheckCrush(bx, by+1, block_rotation) == false)
                Sleep(100);             // if block crush, give time to spin

            if(space_key_on == 1) {     // if space bar hit, can't control
                space_key_on = 0;
                break;
            }
        }
        DropBlock();
        // check level up
        // check game over
        if (new_block_on == 1)          // if need new block, create new block
            NewBlock();
    }

    return 0;
}
/////////////////////////////////////////////////////////////
////////////////////////--Function--////////////////////////
void PrintMenu(void) {
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

}

void Reset(void)
{
    // FILE *file = fopen("score.dat", "rt");      // connecting score.dat
    // if(file == 0) {best_score = 0;}             // if file non-exist, best score = 0
    // else {                                      // if file open
    //     fscanf(file, "%d", &best_score);        // scan best score
    //     fclose(file);                           // closing file
    // }

    // reset variable
    // level = 1;
    // score = 0;
    speed = 100;
    key = 0;
    crush_on = 0;

    system("cls");         //printing clear
    ResetGame();           // Reset main board
    PrintInfo();
    PrintGame();

    block_type_next = rand() % 7;
    NewBlock();
}


void ResetGame(void)
{
    int i, j;

    for(i=0; i<GAME_Y; i++) {           // gaming board reset 0
        for(j=0; j<GAME_X; j++) {
            game_board[i][j] = 0;
            copy_board[i][j] = 333;     // non-use number
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

void ResetCopyBoard(void)
{
    for(int i=0; i<GAME_Y; i++) {
        for(int j=0; j<GAME_X; j++) {
            copy_board[i][j] = 333;     // non-use number
        }
    }
}

void PrintGame(void)
{               // printing gaming board
    int i, j;
    for(j=2; j<GAME_X; j++) {               // if ceil empty re-drawing ceil
        if(game_board[3][j] == EMPTY)
            game_board[3][j] = CEIL;
    }

    for(i=0; i<GAME_Y; i++) {
        for(j=0; j<GAME_X; j++) {
            if(game_board[i][j] != copy_board[i][j]) {      // if copy table and original table
                                                            // different, drawing
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

    for(i=0; i<GAME_Y; i++) {           // printing gaming board and copy
        for(j=0; j<GAME_X; j++) {
            game_board[i][j] = copy_board[i][j];
        }
    }
}

// check crush at the appointed location
int CheckCrush(int bx, int by, int block_rotation)
{
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if(blocks[block_type][block_rotation][i][j] == 1
            && game_board[by+i][bx+j] > 0)
                return false;
        }
    }
    return true;
}

void CheckKey(void)
{
    key = 0;                // reset key

    if(kbhit()) {
        key = getch();
        if(key == 224) {    // if key is arrow key
            key = getch();
            switch(key) {
                case LEFT:
                    if(CheckCrush(bx-1, by, block_rotation) == true)
                        MoveBlock(LEFT);
                    break;
                case RIGHT:
                    if(CheckCrush(bx+1, by, block_rotation) == true)
                        MoveBlock(RIGHT);
                    break;
                case DOWN:
                    if(CheckCrush(bx, by+1, block_rotation) == true)
                        MoveBlock(DOWN);
                    break;
                case UP:
                    if(CheckCrush(bx, by, (block_rotation+1)%4 ) == true)
                        MoveBlock(UP);
                    break;
            }
        }
        else {          // if key is not arrow key
            switch(key) {
                case SPACE:
                    space_key_on = 1;
                    while(crush_on == 0) {
                        DropBlock();        // drop block until bottom
                        // score up
                    }
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
    int x=10, y=5;
    getch();        // waiting for input

    system("cls");  // clear screen
    ResetCopyBoard();
    PrintGame();
    PrintInfo();

    for(int i=1; i<3; i++) {        // Drawing next block
        for(int j=0; j<4; j++) {
            if(blocks[block_type_next][0][i][j] == 1) {
                gotoxy(GAME_X + GAME_ADJ_X+3+j, i+6);
                printf("■");
            }
            else {
                gotoxy(GAME_X + GAME_ADJ_X+3+j, i+6);
                printf("  ");
            }
        }
    }

}

void NewBlock(void)
{
    int i, j;

    bx = (GAME_X/2) - 1;            // creating block position
    by = 0;
    block_type = block_type_next;   // bringing next block type
    block_type_next = rand() % 7;   // random value next block type
    block_rotation = 0;             // rotation reset
    new_block_on = 0;               // reset new block flag

    for(i=0; i<4; i++) {
        for(j=0; j<4; j++) {
            if(blocks[block_type][block_rotation][i][j] == 1) 
                game_board[by+i][bx+j] = ACTIVE_BLOCK;
        }
    }    
}


void DropBlock()
{
    // 밑이 비어있으면 crush on flog off
    if(crush_on && CheckCrush(bx, by+1, block_rotation) == true)
        crush_on = false;

    // 밑이 비어있지 않고, when crush flog on
    if(crush_on && CheckCrush(bx, by+1, block_rotation) == false) {
        for(int i=0; i<GAME_Y; i++) {
            for(int j=0; i<GAME_X; j++) {
                if(game_board[i][j] == ACTIVE_BLOCK)
                    game_board[i][j] = INACT_BLOCK;     // active block -> inactive block
            }
        }
        crush_on = 0;       // reset crush flag
        new_block_on = 1;   // need new block
        return;             // function exit
    }
    if(CheckCrush(bx, by+1, block_rotation) == true)    // empty down, move block
        MoveBlock(DOWN);
    if(CheckCrush(bx, by+1, block_rotation) == false)   // if down is not empty, crush flag on
        crush_on++;
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

    gotoxy(INFO_X, y);    printf(" LEVEL : ");
    gotoxy(INFO_X, y+1);  printf("  GOAL : ");
    gotoxy(INFO_X, y+2);  printf("+-  N E X T  -+ ");
    gotoxy(INFO_X, y+3);  printf("|             | ");
    gotoxy(INFO_X, y+4);  printf("|             | ");
    gotoxy(INFO_X, y+5);  printf("|             | ");
    gotoxy(INFO_X, y+6);  printf("|             | ");
    gotoxy(INFO_X, y+7);  printf("+-- -  -  - --+ "); 
    gotoxy(INFO_X, y+8);  printf(" YOUR SCORE : ");

    gotoxy(INFO_X, y+15); printf("  △   : Shift        SPACE : Hard Drop");     
    gotoxy(INFO_X, y+16); printf("◁  ▷ : Left / Right   P   : Pause");     
    gotoxy(INFO_X, y+17); printf("  ▽   : Soft Drop     ESC  : Quit");
}

void gotoxy(short x, short y) {     // gotoxy default setting
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
