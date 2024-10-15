#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int sizeA = 0, sizeB = 0;
int curseX = 1, curseY = 1;
char tableRec[3][20] = {     "┌───┐",
                             "│   │",
                             "└───┘"};

char tableBoldRec[3][20] = { "┏━━━┓",
                             "┃ * ┃",
                             "┗━━━┛"};

    char tableFullRec[3][20] = { "█████",
                                 "█████",
                                 "█████"};

void printRec(char tableRec[3][20], int row, int column){
    printw(tableRec[0]);    
    move(row+1+3*row,(column)*5); 

    printw(tableRec[1]);
    move(row+2+3*row,(column)*5); 

    printw(tableRec[2]);
    move(row+3*row,(column+1)*5);
}

void printMenu(){
    char menu[3][100] = {
        "(Навигация по меню осуществляется с помощью цифр)\n",
        "1. Создать лабиринт",
        "2. Пройти лабиринт"
    };

    for(int i = 0; i != 3; ++i){
        printw("%s\n", menu[i]);
    }
}

// Parse data
void Split(char data[6], int *value){
    char element[3] = "";
    int pos = 0;
    int j = 0;
    for(int i = 0; i < strlen(data); ++i){
        if(ispunct(data[i])){
            value[j] = atoi(element);
            j++;
            strcpy(element, "");
            pos = 0;
        }else{
            element[pos] = data[i];
            pos++;
        }
    }
    value[j] = atoi(element);
}


void doLab(){
    // Размер лабиринта
    if(sizeA == 0 || sizeB == 0){
        clear();
        printw("Введите размеры лабиринта в формате <X>:<Y>\n(Два числа разделённых двоеточием): ");

        char sizes[6];
        scanw("%s", &sizes);

        int sizesValues[2];
        Split(sizes, sizesValues);
        sizeA = sizesValues[0];
        sizeB = sizesValues[1];
    }

    clear();
    int key;

    while(true){
        for(int row = 0; row != sizeA; ++row){
            for(int column = 0; column != sizeB; ++column){
                if(curseX == column && curseY == row){
                    printRec(tableBoldRec, row, column);
                }else if(row == 0 || row == sizeA-1 || column == 0 || column == sizeB-1){
                    printRec(tableFullRec, row, column);
                }else{
                    printRec(tableRec, row, column);
                }
            }
            move(row+4+3*row, 0);
        }

        printw("Для управления используйте стрлочки\n");
        printw("Для перехода в главное меню два раза нажмите esc");

        key = getch();
        if(key == 27){
            key = getch();
            if(key == 91){
                key = getch();
                if(key == 65){
                    curseY--;
                    if(curseY < 0){
                        curseY = sizeA-1;
                    }
                }else if(key == 66){
                    curseY++;
                    if(curseY > sizeA-1){
                        curseY = 0;
                    }
                }else if(key == 68){
                    curseX--;
                    if(curseX < 0){
                        curseX = sizeB-1;
                    }
                }else if(key == 67){
                    curseX++;
                    if(curseX > sizeB-1){
                        curseX = 0;
                    }
                }
            }
            if (key == 27){
                clear();
                break;
            }

                clear();
        }
    }
}

int main(){
    setlocale(LC_ALL, "");
    initscr();

    int key;
    while(true){
        printMenu();

        key = getch();
        if(key == 49){
            doLab();
        }

        clear();
    }

    return 0;
}