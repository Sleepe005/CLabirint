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

char tableFullRec[3][20] = { "┌███┐",
                             "│███│",
                             "└███┘"};

char startRec[3][20] = { "┏━━━┓",
                         "┃ S ┃",
                         "┗━━━┛"};

char endRec[3][20] = {"┏━━━┓",
                      "┃ E ┃",
                      "┗━━━┛"};

int* labirint;

bool haveLabirint = false;
bool hasStart = false;
bool hasEnd = false;

typedef struct{
    int row, col;
} Point;

int rowDir[] = {-1, 1, 0, 0};
int colDir[] = {0, 0, -1, 1};

// Напечатать квадрат
void printRec(char tableRec[3][20], int row, int column){
    printw(tableRec[0]);    
    move(row+1+3*row,(column)*5); 

    printw(tableRec[1]);
    move(row+2+3*row,(column)*5); 

    printw(tableRec[2]);
    move(row+3*row,(column+1)*5);
}

// Вывести меню
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
        if(!isdigit(data[i])){
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

// Найти координату элемента в лабиринте
void findInLabirint(int itemToFind, int *retValues){
    bool finding = false;
    for(int row = 0; row != sizeA; ++row){
        for(int column = 0; column != sizeB; ++column){
            if(*(labirint + row*sizeB + column) == itemToFind){
                retValues[0] = row;
                retValues[1] = column;
                finding = true;
            }
        }
    }
    if(!finding){
        retValues[0] = -1;
        retValues[1] = -1;
    }
}

// Создать лабиринт
void doLab(){
    if(haveLabirint){
        clear();
        printw("1. Создать новый лабиринт\n");
        printw("2. Редактировать уже существующий лабиринт\n");

        int key = getch();
        if(key == 49){
            haveLabirint = false;
            hasStart = false;
            hasEnd = false;
            sizeA = 0;
            sizeB = 0;
            free(labirint);
        }
    }

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

        labirint = (int*)malloc(sizeA * sizeB * sizeof(int));
    }

    // Генерируем лабиринт
    if(!haveLabirint){
        for(int row = 0; row != sizeA; ++row){
            for(int column = 0; column != sizeB; ++column){
                if(row == 0 || row == sizeA-1 || column == 0 || column == sizeB-1){
                    *(labirint + row*sizeB + column) = 1;
                }
                else{
                    *(labirint + row*sizeB + column) = 0;
                }
            }
        }
        haveLabirint = true;
    }

    clear();
    int key;

    
    while(true){
        for(int row = 0; row != sizeA; ++row){
            for(int column = 0; column != sizeB; ++column){
                if(curseX == column && curseY == row){
                    printRec(tableBoldRec, row, column);
                }else if(*(labirint + row*sizeB + column) == 1){
                    printRec(tableFullRec, row, column);
                }else if(*(labirint + row*sizeB + column) == 2){
                    printRec(startRec, row, column);
                }else if(*(labirint + row*sizeB + column) == 3){
                    printRec(endRec, row, column);
                }else{
                    printRec(tableRec, row, column);
                }
            }
            move(row+4+3*row, 0);
        }

        printw("Для управления используйте стрлочки\n");
        printw("Для перехода в главное меню два раза нажмите esc\n");
        printw("Для установки стены нажмите enter\n");
        printw("Для удаления стены нажмите delete\n");
        printw("Для установки точки старта нажмите S\n");
        printw("Для установки точки выхода нажмите E\n");

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
            }if (key == 27){
                if(hasStart && hasEnd){
                    clear();
                    break;
                }else{
                    clear();
                    printw("В лабиринте должна быть точка старта и точка выхода\n");
                    printw("Для продолжение создания лабиринта нажмите любую клавишу...\n");
                    getch();
                }
            }  

            clear();
        }else if(key == 10){
            if(*(labirint + curseY*sizeB + curseX) == 2){
                hasStart = false;
            }
            if(*(labirint + curseY*sizeB + curseX) == 3){
                hasEnd = false;
            }
            *(labirint + curseY*sizeB + curseX) = 1;
        }else if(key == 127){
            if(*(labirint + curseY*sizeB + curseX) == 2){
                hasStart = false;
            }
            if(*(labirint + curseY*sizeB + curseX) == 3){
                hasEnd = false;
            }
            *(labirint + curseY*sizeB + curseX) = 0;
        }else if(key == 115){
            hasStart = true;
            int retValues[2] = {0,0};
            findInLabirint(2, retValues);
            if(retValues[0] != -1){
                *(labirint + retValues[0]*sizeB + retValues[1]) = 0;
            }
            *(labirint + curseY*sizeB + curseX) = 2;
        }else if(key == 101){
            hasEnd = true;
            int retValues[2] = {0,0};
            findInLabirint(3, retValues);
            if(retValues[0] != -1){
                *(labirint + retValues[0]*sizeB + retValues[1]) = 0;
            }
            *(labirint + curseY*sizeB + curseX) = 3;
        }
    }
}

int isValid(int row, int col, int visited[sizeA][sizeB]) {
    return (row >= 0 && row < sizeA && col >= 0 && col < sizeB && *(labirint + row*sizeB + col) != 1 && !visited[row][col]);
}

bool solveLab(int visited[sizeA][sizeB], Point current, Point end, Point* path, int step){
     // Если достигли выхода, сохраняем текущее положение в путь и возвращаем успех
    if (current.row == end.row && current.col == end.col) {
        path[step] = current;
        return 1;
    }

    // Помечаем текущую клетку как посещённую
    visited[current.row][current.col] = 1;
    path[step] = current;  // Добавляем текущую клетку в путь

    // Проходим по всем направлениям
    for (int i = 0; i < 4; i++) {
        int newRow = current.row + rowDir[i];
        int newCol = current.col + colDir[i];

        if (isValid(newRow, newCol, visited)) {
            Point newPoint = {newRow, newCol};

            // Рекурсивно ищем путь из новой клетки
            if (solveLab(visited, newPoint, end, path, step + 1)) {
                return 1;  // Если выход найден, возвращаем успех
            }
        }
    }

    // Если путь через эту клетку не найден, возвращаемся назад (backtracking)
    visited[current.row][current.col] = 0;  // Снимаем отметку посещённости
    return 0;  // Путь не найден
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
        }else if(key == 50){
            clear();
            int visited[10][10] = {0};

            int valStart[2] = {0};
            findInLabirint(2, valStart);

            int valEnd[2] = {0};
            findInLabirint(3, valEnd);

            Point start = {valStart[0], valStart[1]};
            Point end = {valEnd[0], valEnd[1]};

            Point path[100];

            if(solveLab(visited, start, end, path, 0)){
                // printw("Путь найден:\n");
                // for (int i = 0; i < sizeA * sizeB; i++) {
                //     if (path[i].row == 0 && path[i].col == 0) break;
                //     printw("(%d, %d) -> ", path[i].row, path[i].col);
                // }
                // printw("EXIT\n");

                int sl[10][10] = {0};
                for(int i = 0; i < sizeA*sizeB; i++){
                    if (path[i].row == 0 && path[i].col == 0) break;
                    sl[path[i].row][path[i].col] = 1;
                }

                for(int row = 0; row != sizeA; ++row){
                    for(int column = 0; column != sizeB; ++column){
                        if(sl[row][column] == 1){
                            printRec(tableBoldRec, row, column);
                        }else if(*(labirint + row*sizeB + column) == 1){
                            printRec(tableFullRec, row, column);
                        }else if(*(labirint + row*sizeB + column) == 2){
                            printRec(startRec, row, column);
                        }else if(*(labirint + row*sizeB + column) == 3){
                            printRec(endRec, row, column);
                        }else{
                            printRec(tableRec, row, column);
                        }
                    }
                    move(row+4+3*row, 0);
                }

            } else {
                printw("Путь не найден.\n");
            }
            getch();
        }

        clear();
    }

    return 0;
}