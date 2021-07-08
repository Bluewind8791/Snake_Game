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
#define p 112
#define P 80
#define SPACE 32
// drawing value define
#define WALL 1
#define EMPTY 0
#define CEILING -1
#define ACTIVE_BLOCK -2
#define INACTIVE_BLOCK 2
#define GAME_X 10+2			// game area size 12
#define GAME_Y 20+1			// game area y size 21
#define POS_ADJ_X 2     	// pos = position
#define POS_ADJ_Y 2

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

int block[7][4][4][4] = {
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

int space_org[GAME_Y][GAME_X] = {  // 세로 20+1(아래벽)칸, 가로 10+2(양쪽 벽)칸  
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},  // 5
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},  // 10
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},  // 15
    {1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},  // 20
	{1,1,1,1,1,1,1,1,1,1,1,1}
};

clock_t startDropT, endT, startGroundT;
RECT blockSize;

int space_cpy[GAME_X][GAME_Y];
int x = GAME_X/2; 
int y = 0;
int blockForm;
int blockRotation = 0;
int key;
int speed = 500;

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

void gotoxy(int x, int y);
void Init();
void CreateRandomForm();
bool CheckCrash(int x, int y);
void DropBlock();
void BlockToGround();
void RemoveLine();
void DrawMap();
void DrawBlock();
void InputKey();

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

int main() {
	Init();
	startDropT = clock();
	CreateRandomForm();

	while (true) {
		DrawMap();
		DrawBlock();
		DropBlock();
		BlockToGround();
		RemoveLine();
		InputKey();
	}
	return 0;
}

//----------------------------------------------------------------------------------------//
//----------------------------------------------------------------------------------------//

void Init() {
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = 0;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
	srand(time(NULL));
}


void CreateRandomForm() {
	blockForm = rand() % 7;
}

// x와 y를 대입하여 맞닿았는지 true, false로 return
bool CheckCrash(int x, int y) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				int t = space_org[i + y][j + x / 2];
				if (t == 1 || t == 2) { // 벽일 때, 블럭일 때
					return true;
				}
			}
		}
	}
	return false;
}


void DropBlock() {
	endT = clock();
	if ((float)(endT - startDropT) >= speed) {		// every speed drop block
		if (CheckCrash(x, y + 1) == true) return;
		y++;
		startDropT = clock();
		startGroundT = clock();
		system("cls");
	}
}


void BlockToGround() {
	if (CheckCrash(x, y + 1) == true) {
		if ((float)(endT - startGroundT) > 1500) {
			// 현재 블록 저장
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					if (block[blockForm][blockRotation][i][j] == 1) {
						space_org[i + y][j + x / 2] = 2;
					}
				}
			}
			x = GAME_X/2;
			y = 0;
			CreateRandomForm();
		}
	}
}


void RemoveLine() {
	for (int i = GAME_Y-1; i >= 0; i--) { // 벽라인 제외한 값
		int cnt = 0;
		for (int j = 1; j < GAME_X-1; j++) {
			if (space_org[i][j] == INACTIVE_BLOCK) {
				cnt++;
			}
		}
		if (cnt >= 10) {	// 벽돌이 다 차있다면
			for (int j = 0; i - j >= 0; j++) {
				for (int x = 1; x < GAME_X-1; x++) {
					if (i - j - 1 >= 0)
						space_org[i - j][x] = space_org[i - j - 1][x];
					else      // 천장이면 0 저장
						space_org[i - j][x] = EMPTY;
				}
			}
		}
	}
}

// 게임판 그리는 함수
void DrawMap() 
{
	int i, j;
	// gotoxy(0, 0);

	for (i = 0; i < GAME_Y; i++) 	{
		for (j = 0; j < GAME_X; j++) 	{
			// if(space_org[i][j] != space_cpy[i][j]) {
				gotoxy(POS_ADJ_X+(j*2), POS_ADJ_Y+i);
				switch(space_org[i][j]) {
					// case EMPTY:
					// 	printf(" ");
					// 	break;
					case WALL:
						printf("▩");
						break;
					case INACTIVE_BLOCK:
						printf("□");
						break;
				}
				// if (space_org[i][j] == WALL) {
				// 	gotoxy(j * 2, i);
				// 	printf("▩");
				// }
				// else if (space_org[i][j] == INACTIVE_BLOCK) {
				// 	gotoxy(j * 2, i);
				// 	printf("□");
				// }
			// }
		}
	}
	// 게임판을 그린 후 카피에 복사
	for (i = 0; i < GAME_Y; i++) {
		for(j = 0; j < GAME_X; j++) {
			space_cpy[i][j] = space_org[i][j];
		}
	}
}



void DrawBlock() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (block[blockForm][blockRotation][i][j] == 1) {
				gotoxy(x+(j*2), y+i);
				printf("■");
			}
		}
	}
}


void InputKey() {
	if (_kbhit()) {
		key = _getch();
		switch (key) {
		case UP:
			blockRotation = (blockRotation+1) % 4;
			startGroundT = clock();
			break;
		case LEFT:
			if (CheckCrash(x-2, y) == false) {
				x -= 2;
				startGroundT = clock();
			}
			break;
		case RIGHT:
			if (CheckCrash(x+2, y) == false) {
				x += 2;
				startGroundT = clock();
			}
			break;
		case DOWN:
			if (CheckCrash(x, y+1) == false)
				y++;
			break;
		}
		system("cls");
	}
}

void gotoxy(int x, int y) {
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}