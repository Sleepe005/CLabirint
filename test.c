#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void printRec(char tableRec[3][20], int row, int column){
    printw(tableRec[0]);    
    move(row+1+3*row,(column)*5); 

    printw(tableRec[1]);
    move(row+2+3*row,(column)*5); 

    printw(tableRec[2]);
    move(row+3*row,(column+1)*5);
}

int main(){
    setlocale(LC_ALL, "");
    initscr();
    int key;
    key = getch();
    printw("%d\n", key);
    // key = getch();
    // printw("%d\n", key);


    getch();
    
    return 0;
}