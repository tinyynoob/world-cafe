#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

int main()
{
    short round;
    printf("round: ");
    // scanf("%hd", &round);
    round = 9;
    srand(time(NULL));
    permu = permutation(round, &permu_size);
    printf("permu_size = %d\n\n", permu_size);

    short **matrix = semi_sudoku(round);
    print_matrix(matrix, round);

    for (int i = 0; i < round; i++)
        free(matrix[i]);
    free(matrix);
    for (int i = 0; i < permu_size; i++)
        free(permu[i]);
    free(permu);
    return 0;
}

