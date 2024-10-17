#include <stdio.h>

// Структура для представления координат клетки
typedef struct {
    int row, col;
} Point;

// Направления для движения (вверх, вниз, влево, вправо)
int rowDir[] = {-1, 1, 0, 0};
int colDir[] = {0, 0, -1, 1};

// Проверка, можно ли двигаться в данную клетку
int isValid(int row, int col, int maze[5][5], int visited[5][5], int numRows, int numCols) {
    return (row >= 0 && row < numRows && col >= 0 && col < numCols && maze[row][col] != 1 && !visited[row][col]);
}

// Рекурсивная функция DFS с возвратом пути
int dfs(int maze[5][5], int visited[5][5], Point current, Point end, int numRows, int numCols, Point* path, int step) {
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

        if (isValid(newRow, newCol, maze, visited, numRows, numCols)) {
            Point newPoint = {newRow, newCol};

            // Рекурсивно ищем путь из новой клетки
            if (dfs(maze, visited, newPoint, end, numRows, numCols, path, step + 1)) {
                return 1;  // Если выход найден, возвращаем успех
            }
        }
    }

    // Если путь через эту клетку не найден, возвращаемся назад (backtracking)
    visited[current.row][current.col] = 0;  // Снимаем отметку посещённости
    return 0;  // Путь не найден
}

int main() {
    // Пример лабиринта
    int maze[5][5] = {
        {1, 1, 1, 1, 1},
        {1, 0, 2, 0, 1},
        {1, 0, 1, 0, 1},
        {1, 3, 0, 0, 1},
        {1, 1, 1, 1, 1}
    };

    int numRows = 5, numCols = 5;

    // Массив для отслеживания посещённых клеток
    int visited[5][5] = {0};

    // Координаты старта и выхода
    Point start = {1, 2};
    Point end = {3, 1};

    // Массив для хранения пути (максимальная длина пути = numRows * numCols)
    Point path[25];
    
    // Запуск DFS
    if (dfs(maze, visited, start, end, numRows, numCols, path, 0)) {
        printf("Путь найден:\n");
        for (int i = 0; i < numRows * numCols; i++) {
            if (path[i].row == 0 && path[i].col == 0) break;
            printf("(%d, %d) -> ", path[i].row, path[i].col);
        }
        printf("EXIT\n");
    } else {
        printf("Путь не найден.\n");
    }

    return 0;
}
