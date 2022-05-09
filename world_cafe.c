#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "permutation.h"
#include "semi_sudoku.h"

short **permu;
int permu_size;

void print_matrix(short **matrix, short round)
{
    for (int i = 0; i < round; i++) {
        for (int j = 0; j < round; j++)
            printf("%hd ", matrix[i][j]);
        putchar('\n');
    }
    putchar('\n');
}

static inline void swapint(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    short round;
    printf("round (1 ~ 10): ");
    scanf("%hd", &round);
    while (round <= 0 || round > 10) {
        printf("1 ~ 10 plz: ");
        scanf("%hd", &round);
    }
    int player;
    printf("number of players: ");
    scanf("%d", &player);
    while (player <= 0) {
        printf("try again: ");
        scanf("%d", &player);
    }
    srand(time(NULL));
    permu = permutation(round, &permu_size);
    FILE *f = fopen("world_cafe.csv", "w");
    for (int count = 0; count < player + round;) {
        short **matrix = semi_sudoku(round);
        int order[round];
        for (int i = 0; i < round; i++)
            order[i] = i;
        for (int i = 0; i < round; i++)  // shuffle
            swapint(&order[i], &order[rand() % round]);
        for (int i = 0; i < round; i++) {
            fprintf(f, "name##%d,", ++count);
            for (int j = 0; j < round; j++)
                fprintf(f, "%hd,", matrix[order[i]][j] + 1);
            putc('\n', f);
        }
        for (int i = 0; i < round; i++)
            free(matrix[i]);
        free(matrix);
    }
    fclose(f);
    for (int i = 0; i < permu_size; i++)
        free(permu[i]);
    free(permu);
    return 0;
}
