#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define max_size 256

// struktur data untuk queue yang digunakan dalam BFS
typedef struct queuecell
{
    int x;    // koordinat x (row)
    int y;    // koordinat y (column)
    int dist; // jarak sel dari titik awal
} queuecell;

// digunakan untuk mendapatkan koordinat baris dan kolom
// pada tetangga sel
int rowNum[] = {-1, 0, 0, 1};
int colNum[] = {0, -1, 1, 0};

void readMaze(const char *text, int ***matrix, int *rows, int *cols, int start[2], int dest[2])
{
    // menentukan ukuran rows dan cols
    const char *p = text;
    *rows = 0;
    *cols = 0;
    int currentCols = 0;

    while (*p)
    {
        if (*p == '\n')
        {
            if (*rows == 0)
            {
                *cols = currentCols;
            }

            if (currentCols != *cols)
            {
                printf("Bentuk labirin bukan persegi atau persegi panjang!\n");
            }

            (*rows)++;
            currentCols = 0;
        }
        else
        {
            currentCols++;
        }
        p++;
    }
    (*rows)++;

    // alokasi memori untuk matriks
    *matrix = (int **)malloc(*rows * sizeof(int *));
    for (int i = 0; i < *rows; i++)
    {
        (*matrix)[i] = (int *)malloc(*cols * sizeof(int));
    }

    // mengisi matriks
    int row = 0;
    int col = 0;
    p = text;

    while (*p)
    {
        if (*p == '\n')
        {
            row++;
            col = 0;
        }
        else
        {
            if (*p == '#')
            {
                (*matrix)[row][col] = 0;
            }
            else if (*p == '.')
            {
                (*matrix)[row][col] = 1;
            }

            // menentukan start dan destination
            else if (*p == 'S')
            {
                (*matrix)[row][col] = 1;
                (start)[0] = row;
                (start)[1] = col;
            }
            else if (*p == 'E')
            {
                (*matrix)[row][col] = 1;
                (dest)[0] = row;
                (dest)[1] = col;
            }

            // ada karakter yang tidak valid
            else
            {
                printf("Terdapat karakter tidak valid!\n");
                return;
            }

            col++;
        }
        p++;
    }
}

// mengecek apakah suatu sel (row,col) valid atau tidak
bool isValid(int row, int col, int rows, int cols)
{
    // mengeluarkan true jika row dan col
    // di dalam jangkauan
    return (row >= 0) && (row < rows) &&
           (col >= 0) && (col < cols);
}

int mazeBFS(int **mat, int *start, int *dest, int rows, int cols, int ****m_path)
{
    // mengecek sel source dan destination
    // terdapat dalam matriks
    if (!mat[start[0]][start[1]] || !mat[dest[0]][dest[1]])
        return -1;

    // inisialisasi matriks visited
    int **visited = (int **)malloc((rows) * sizeof(int *));
    for (int i = 0; i < rows; i++)
    {
        visited[i] = (int *)malloc((cols) * sizeof(int));
    }

    // nilai sel = 1 apabila sel telah ditelusuri
    // nilai sel = 0 apabila sel belum ditelusuri
    for (int i = 0; i < rows; i++)
    {

        for (int j = 0; j < cols; j++)
        {
            visited[i][j] = 0;
        }
    }

    // inisialisasi queue frontier
    queuecell *frontier = malloc(sizeof(queuecell) * (rows * cols));

    // inisialisasi noda jalur
    int ***path = (int ***)malloc(sizeof(int **) * (rows));
    for (int i = 0; i < rows; i++)
    {
        path[i] = (int **)malloc(sizeof(int *) * (cols));
        for (int j = 0; j < cols; j++)
        {
            path[i][j] = (int *)malloc(sizeof(int) * 2);
        }
    }

    // catat source telah ditelusuri matriks visited
    visited[start[0]][start[1]] = 1;

    // catat source ada dalam frontier
    queuecell s = {start[0], start[1], 0};
    frontier[0] = s;

    // variabel front dan back dalam frontier
    // digunakan untuk  melacak kejadian "pop"
    int front = 0; // posisi sel yang paling awal masuk
    int back = 0;  // posisi sel yang paling terakhir masuk

    // ulangi loop saat frontier tidak kosong
    while (front - 1 != back)
    {
        queuecell currentcell = frontier[front]; // "pop" queue frontier
        front++;

        // jika dest = posisi currentcell, destinasi telah dicapai
        if (currentcell.x == dest[0] && currentcell.y == dest[1])
        {
            for (int i = 0; i < rows; i++)
            {
                free(visited[i]);
            }
            free(visited);
            free(frontier);

            *m_path = path;
            return currentcell.dist;
        }

        // cek seluruh posisi di samping currentcell
        for (int i = 0; i < 4; i++)
        {
            int row = currentcell.x + rowNum[i];
            int col = currentcell.y + colNum[i];

            // if sel di samping currentcell valid, bisa dicapai,
            // dan belum ditelusuri, dimasukkan ke dalam queue frontier
            if (isValid(row, col, rows, cols) && mat[row][col] && visited[row][col] == 0)
            {
                // catat sel sebagai telah ditelusuri
                visited[row][col] = 1;
                queuecell adjacentcell = {row, col, currentcell.dist + 1};

                // tambah sel dalam queue frontier
                back++;
                frontier[back] = adjacentcell;

                // catat noda sebelumnya dari sel
                path[row][col][0] = currentcell.x;
                path[row][col][1] = currentcell.y;
            }
        }
    }

    for (int i = 0; i < rows; i++)
    {
        free(visited[i]);
    }
    free(visited);
    free(frontier);

    // output = -2 saat tidak ada jalur dari sumber ke destinasi
    return -2;
}

void drawPathBFS(int **maze, int rows, int cols, int ***path, int *start, int *dest, int dist)
{
    char **map = (char **)malloc(sizeof(char *) * rows);
    for (int i = 0; i < cols; i++)
    {
        map[i] = (char *)malloc(sizeof(char) * cols);
    }

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            if (maze[y][x])
                map[y][x] = '.';
            else
                map[y][x] = '#';
        }
    }

    int cur_row = dest[0];
    int temp_row = cur_row;
    int cur_col = dest[1];
    int temp_dist = dist;

    while (temp_dist > 0)
    {
        map[cur_row][cur_col] = '+';

        temp_row = cur_row;
        cur_row = path[temp_row][cur_col][0];
        cur_col = path[temp_row][cur_col][1];
        temp_dist--;
    }

    map[start[0]][start[1]] = 'S';
    map[dest[0]][dest[1]] = 'E';

    printf("\n");
    printf("Jalur:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    printf("Banyaknya langkah: %d\n", dist);
}
