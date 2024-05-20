#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <sys/time.h>

#define MAX 100

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point points[MAX];
    int length;
} Path;

char maze[MAX][MAX];
bool visited[MAX][MAX];
int rows, cols;
Point start, end;
Path paths[MAX];
int pathCount = 0;

// Membaca maze dari file dan menentukan posisi start dan end
void readMaze(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    rows = 0;
    while (fgets(maze[rows], MAX, file)) {
        cols = strlen(maze[rows]);
        if (maze[rows][cols - 1] == '\n') {
            maze[rows][cols - 1] = '\0';
            cols--;
        }
        for (int i = 0; i < cols; i++) {
            if (maze[rows][i] == 'S') {
                start.x = rows;
                start.y = i;
            } else if (maze[rows][i] == 'E') {
                end.x = rows;
                end.y = i;
            }
        }
        rows++;
    }
    fclose(file);
}

// Memeriksa apakah posisi valid untuk dilalui
bool isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < rows && y < cols && maze[x][y] != '#' && !visited[x][y];
}

// Menyalin isi path dari src ke dest
void copyPath(Path *dest, Path *src) {
    dest->length = src->length;
    memcpy(dest->points, src->points, src->length * sizeof(Point));
}

// Menambahkan path ke daftar paths
void addPath(Path *path) {
    copyPath(&paths[pathCount++], path);
}

// Mencari semua jalur dari posisi (x, y) ke end
void findPaths(int x, int y, Path *currentPath) {
    if (x == end.x && y == end.y) {
        addPath(currentPath);
        return;
    }

    visited[x][y] = true;
    Point directions[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    for (int i = 0; i < 4; i++) {
        int newX = x + directions[i].x;
        int newY = y + directions[i].y;

        if (isValid(newX, newY)) {
            currentPath->points[currentPath->length].x = newX;
            currentPath->points[currentPath->length].y = newY;
            currentPath->length++;
            findPaths(newX, newY, currentPath);
            currentPath->length--;
        }
    }

    visited[x][y] = false;
}

// Mencetak path yang diberikan
void printPath(Path *path) {
    for (int i = 0; i < path->length; i++) {
        printf("(%d, %d)", path->points[i].x, path->points[i].y);
        if (i < path->length - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main() {
    char filename[MAX];
    printf("Masukkan nama file: ");
    scanf("%s", filename);

    readMaze(filename);

    Path currentPath;
    currentPath.length = 0;
    currentPath.points[currentPath.length++] = start;

    struct timeval begin, end;
    gettimeofday(&begin, 0);
    
    findPaths(start.x, start.y, &currentPath);
    
    gettimeofday(&end, 0);

    long sec = end.tv_sec - begin.tv_sec;
    long us = end.tv_usec - begin.tv_usec;
    double elapsed = sec * 1e3 + us * 1e-3;
    printf("Time: %lf ms\n", elapsed);

    if (pathCount == 0) {
        printf("No path found from start to end.\n");
        return 0;
    }

    printf("All possible paths from start to end:\n");
    for (int i = 0; i < pathCount; i++) {
        printf("Path %d: ", i + 1);
        printPath(&paths[i]);
    }

    int minLength = INT_MAX;
    int maxLength = 0;
    Path *shortestPath = NULL;
    Path *longestPath = NULL;

    // Menemukan path terpendek dan terpanjang
    for (int i = 0; i < pathCount; i++) {
        if (paths[i].length < minLength) {
            minLength = paths[i].length;
            shortestPath = &paths[i];
        }
        if (paths[i].length > maxLength) {
            maxLength = paths[i].length;
            longestPath = &paths[i];
        }
    }

    printf("\nShortest path from start to end:\n");
    printPath(shortestPath);

    printf("\nLongest path from start to end:\n");
    printPath(longestPath);

    return 0;
}
