#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "permutation.h"

static bool isValid(const short *su[], short round, unsigned checkxor)
{
    unsigned check;
    for (int r = 0; r < round; r++) {
        check = checkxor;
        for (int c = 0; c < round; c++)
            check ^= 1u << su[r][c];
        if (check)
            return false;
    }
    for (int c = 0; c < round; c++) {
        check = checkxor;
        for (int r = 0; r < round; r++)
            check ^= 1u << su[r][c];
        if (check)
            return false;
    }
    return true;
}

static void printOut(const short *su[], short round)
{

}

int main()
{
    short round;
    scanf("%hd", &round);
    int permu_size;
    int **permu = permutation(round, &permu_size);
    printf("%d\n", permu_size);
    unsigned checkxor = 0;
    for (int i = 0; i < round; i++)
        checkxor ^= 1u << i;
    short su[round][round];   // sudoku-like rule
    // developing...
    for (short r = 0; r < round; r++) {

        for (short c = 0; c < round; c++) {
            su[r][c] = 1;
        }
    }
    for (int i = 0; i < permu_size; i++)
        free(permu[i]);
    free(permu);
    return 0;
}

