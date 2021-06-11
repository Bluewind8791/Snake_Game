#include <stdio.h>
#include <windows.h>    //Sleep include
#include <conio.h>      //getch, kbhit include
#include <stdlib.h>     //system include

#define true 1
#define false 0

//방향키 define
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

void gotoxy(int x, int y) {
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void CursorView (char show)     //cursor 숨기기
{
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO ConsoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    ConsoleCursor.bVisible = show;
    ConsoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

int main() {

    char cursor = 0;
    int x = 0;
    int y = 0;
    CursorView(0);                  //0 = unvisiable
    
    while(true) {
        
        gotoxy(x,y);
        printf("★");
        
        if (kbhit())        //keyboard hit
        {
            cursor = getch();
            switch(cursor) {
            case UP: 
                y--;
                break;
            case LEFT: 
                x--;
                break;
            case DOWN: 
                y++;
                break;
            case RIGHT:
                x++;
                break;
             }
             system("cls");     //printing clear
             Sleep(10);
        }
    }
    

    return 0;
}