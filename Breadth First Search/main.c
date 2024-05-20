#include <stdio.h>
#include "lib.c"
#define max_size 256

int main()
{
    char filename[64];
    printf("Masukkan nama file input: ");
    scanf("%s", filename);

    int ***path;
    int **maze;
    int rows, cols;
    int start[2] = {-1, -1};
    int dest[2] = {-1, -1};

    FILE *fp = fopen(filename, "r");
    char buf[4096];
    char temp_buf[4096];

    // pengecekan adanya file
    if (fp == NULL)
    {
        printf("File tidak ditemukan.\n");
        return 0;
    }

    // pengecekan isi file
    if (fgets(buf, max_size, fp))
    {
        // tidak lakukan apa-apa
    }
    else
    {
        printf("File kosong.\n");
        return 0;
    }

    // memasukkan isi file ke dalam data buffer
    while (fgets(temp_buf, max_size, fp))
    {
        strcat(buf, temp_buf);
    }

    readMaze(buf, &maze, &rows, &cols, start, dest);

    if (start[0] == -1 || dest[0] == -1)
    {
        printf("Tidak terdapat titik awal atau titik akhir di dalam labirin");
        return 0;
    }

    int dist = mazeBFS(maze, start, dest, rows, cols, &path);

    if (dist == -1)
    {
        printf("Titik awal atau akhir di luar labirin");
        return 0;
    }

    else if (dist == -2)
    {
        printf("Tidak terdapat jalur yang menghubungkan titik awal dan titik akhir di dalam labirin");
        return 0;
    }

    drawPathBFS(maze, rows, cols, path, start, dest, dist);

    fclose(fp);

    return 0;
}
