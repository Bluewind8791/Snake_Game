#include <stdio.h>
#include <stdbool.h>
#include <windows.h>    //Sleep, gotoxy include
#include <conio.h>      //getch, kbhit include
#include <stdlib.h>     //system include
#include <time.h>

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

#define TRUE 1
#define FALSE 0
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
#define GAME_X 10+2
#define GAME_Y 20+1
#define POS_ADJ_X 2*2       // pos = position
#define POS_ADJ_Y 2

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

int blocks[7][4][4][4] = {
    {{0,0,0,0,
      0,1,1,0,
      0,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,1,1,0,
      0,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,1,1,0,
      0,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,1,1,0,
      0,1,1,0,
      0,0,0,0}},

    {{0,0,0,0,
      0,0,0,0,
      1,1,1,1,
      0,0,0,0},
     {0,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,0,0},
     {0,0,0,0,
      0,0,0,0,
      1,1,1,1,
      0,0,0,0},
     {0,1,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,0,0}},

    {{0,0,0,0,
      1,1,0,0,
      0,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,0,1,0,
      0,1,1,0,
      0,1,0,0},
     {0,0,0,0,
      1,1,0,0,
      0,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,0,1,0,
      0,1,1,0,
      0,1,0,0}},

    {{0,0,0,0,
      0,1,1,0,
      1,1,0,0,
      0,0,0,0},
     {0,0,0,0,
      1,0,0,0,
      1,1,0,0,
      0,1,0,0},
     {0,0,0,0,
      0,1,1,0,
      1,1,0,0,
      0,0,0,0},
     {0,0,0,0,
      1,0,0,0,
      1,1,0,0,
      0,1,0,0}},

    {{0,0,0,0,
      0,0,1,0,
      1,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      1,1,0,0,
      0,1,0,0,
      0,1,0,0},
     {0,0,0,0,
      0,0,0,0,
      1,1,1,0,
      1,0,0,0},
     {0,0,0,0,
      0,1,0,0,
      0,1,0,0,
      0,1,1,0}},

    {{0,0,0,0,
      1,0,0,0,
      1,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,1,0,0,
      0,1,0,0,
      1,1,0,0},
     {0,0,0,0,
      0,0,0,0,
      1,1,1,0,
      0,0,1,0},
     {0,0,0,0,
      0,1,1,0,
      0,1,0,0,
      0,1,0,0}},

    {{0,0,0,0,
      0,1,0,0,
      1,1,1,0,
      0,0,0,0},
     {0,0,0,0,
      0,1,0,0,
      0,1,1,0,
      0,1,0,0},
     {0,0,0,0,
      0,0,0,0,
      1,1,1,0,
      0,1,0,0},
     {0,0,0,0,
      0,1,0,0,
      1,1,0,0,
      0,1,0,0}}
};  //4x4 2차원 배열에 블록을 표현, 7가지 종류의 블럭이 4방향 회전
    // blocks[block_type][block_rotation][i][j]

// ※ 록 딜레이(Lock Delay, 테트로미노가 지면에 닿고 붙어버리기까지 걸리는 시간)은 0.5초로 한다.

int game_area[GAME_Y][GAME_X] = {
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},  // 10
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,1},  // 20
    {1,1,1,1,1,1,1,1,1,1,1,1}
};



int game_area_copy[GAME_Y][GAME_X];
int block_type;
int block_next_type;
int block_spin;
int blockX, blockY;
int new_block_flag;
int speed = 300;
int crush_flag;

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

void gotoxy(short x, short y);
void CursorView();
void PrintGameArea();
void PrintInfoArea();
void MakeNewBlock();
void DropBlock();
void MoveBlock(int direction);
bool CheckCrush(int x, int y);
void EraseBlock();

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//
int main()
{
    srand(time(NULL));                      // random seed
    system("mode con cols=60 lines=25 | title GGUL's TETRIS");    // setting cmd window size
    CursorView();

    PrintInfoArea();
    MakeNewBlock();
    
    while(1) {
        PrintGameArea();
        Sleep(speed);
        DropBlock();
        if(new_block_flag == 1)
            MakeNewBlock();    
    }

    return 0;
}
//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//


void MakeRandomForm()
{
    block_type = rand() % 7;
}

bool CheckCrush(int x, int y)
{
    int t;
    crush_flag = FALSE;     // crush flag reset

    // 게임판과 블럭모양을 비교하여 겹치면
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if(blocks[block_type][block_spin][i][j] == 1)
                t = game_area[i+y][j+x / 2];
                if( t == 1 || t == 2)
                    return TRUE;
        }
    }
}


void DropBlock()
{
    // CheckCrush();
    // 블록 드랍 불가능할 때
    // 밑이 비어있으면 crush flag off
    // 밑이 비어있지 않으면 crush flag on, block inactive mode change
    
    if(crush_flag == TRUE) {
        for(int i=0; i<GAME_Y; i++){
            for(int j=0; j<GAME_X; j++) {
                if(game_area[i][j] == ACTIVE_BLOCK)
                    game_area[i][j] = INACTIVE_BLOCK;
                    new_block_flag = TRUE;              // new block flag ON
                    crush_flag = FALSE;                 // crush flag RESET
                    return;
            }
        }
    }

    if(crush_flag == FALSE)
        MoveBlock(DOWN);
}


void EraseBlock()
{
    for(int i=0; i<4; i++) {                        // erase block
        for(int j=0; j<4; j++) {
            if(blocks[block_type][block_spin][i][j]==1) 
                game_area[blockY+i][blockX+j] = EMPTY;
        }
    }
}

void MoveBlock(int direction)
{
    int i,j;
    switch(direction) {
        case LEFT:
            EraseBlock();
            for(i=0; i<4; i++) {                        // go x-- make active block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_spin][i][j]==1) 
                        game_area[blockY+i][blockX+j-1] = ACTIVE_BLOCK;
                }
            }
            blockX--;
            break;
        case RIGHT:
            EraseBlock();
            for(i=0; i<4; i++) {                        // go x++ make active block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_spin][i][j]==1) 
                        game_area[blockY+i][blockX+j+1] = ACTIVE_BLOCK;
                }
            }
            blockX++;
            break;
        case DOWN:
            EraseBlock();
            for(i=0; i<4; i++) {                      // go y++ make active block
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_spin][i][j]==1)
                        game_area[blockY+i+1][blockX+j] = ACTIVE_BLOCK;
                }
            }
            blockY++;
            break;
        case UP:
            EraseBlock();
            block_spin = (block_spin+1) % 4;    // make block spin (+1 and no more 4)
            for(i=0; i<4; i++) {
                for(j=0; j<4; j++) {
                    if(blocks[block_type][block_spin][i][j]==1) 
                        game_area[blockY+i][blockX+j] = ACTIVE_BLOCK;
                }
            }
            break;
    }
}

void MakeNewBlock()
{
    blockX = (GAME_X/2);
    blockY = 0;
    new_block_flag = 0;              // new_block flag 초기화
    block_type = block_next_type;
    block_next_type = rand() % 7;   // block type random 지정
    block_spin = 0;                 // block spin 초기화

    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            if(blocks[block_type][block_spin][i][j] == 1)
                game_area[blockY+i][blockX+j] = ACTIVE_BLOCK;
        }
    }
}

void PrintInfoArea()
{
    unsigned int infoX = GAME_X+(POS_ADJ_X*2);
    unsigned int infoY = POS_ADJ_Y;

    gotoxy(infoX, infoY+01);   printf("T E T R I S");
    gotoxy(infoX, infoY+03);   printf("LEVEL : ");
    gotoxy(infoX, infoY+04);   printf("SCORE : ");
    gotoxy(infoX, infoY+05);   printf("NEXT BLOCK");
    gotoxy(infoX, infoY+10);   printf("  △   : Shift");
    gotoxy(infoX, infoY+11);   printf("◁  ▷ : Left / Right");
    gotoxy(infoX, infoY+12);   printf("  ▽   : Soft Drop");
    gotoxy(infoX, infoY+13);   printf(" SPACE : Hard Drop");
    gotoxy(infoX, infoY+14);   printf("  ESC  : Quit Game");

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
    // game area - ceiling /  블록이 지나가서 지워질때마다 그리기
    for(j=2; j<GAME_X-2; j++) {
        if(game_area[2][j] == EMPTY)
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

void CursorView()
{
    CONSOLE_CURSOR_INFO cursorInfo = { 0, };
    cursorInfo.dwSize = 1;          //커서 굵기 (1 ~ 100)
    cursorInfo.bVisible = FALSE;    //커서 Visible TRUE(보임) FALSE(숨김)
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void gotoxy(short x, short y)       // gotoxy default setting
{     
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}