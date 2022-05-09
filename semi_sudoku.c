#include "semi_sudoku.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

extern short **permu;
extern int permu_size;

static struct su {
    short size;  // = round
    short **matrix;
    unsigned checkxor;
};

static bool isValid(const struct su *su)
{
    unsigned check;
    for (int r = 0; r < su->size; r++) {
        check = su->checkxor;
        for (int c = 0; c < su->size; c++)
            check ^= 1u << su->matrix[r][c];
        if (check)
            return false;
    }
    for (int c = 0; c < su->size; c++) {
        check = su->checkxor;
        for (int r = 0; r < su->size; r++)
            check ^= 1u << su->matrix[r][c];
        if (check)
            return false;
    }
    return true;
}

static inline void vector_copy(short **dest, short **src, int size)
{
    for (int i = 0; i < size; i++)
        (*dest)[i] = (*src)[i];
}

static struct su *su_init(short round)
{
    struct su *su = (struct su *) malloc(sizeof(struct su));
    su->size = round;
    su->matrix = (short **) malloc(sizeof(short *) * su->size);
    for (int i = 0; i < su->size; i++)
        su->matrix[i] = (short *) malloc(sizeof(short) * su->size);
    su->checkxor = 0;
    for (int i = 0; i < su->size; i++)
        su->checkxor ^= 1u << i;
    return su;
}

/* Return value:
 * 0: success
 * 1: keep going
 * 2: cannot find a valid matrix
 */
static int backtrack(struct su *su, int row, int serial)
{
    // print_matrix(su->matrix, su->size);
    if (row == su->size) {  // end recursion
        if (isValid(su))
            return 0;
        else
            return 1;
    }
loop:
    for (serial++; serial < permu_size; serial++) {
        vector_copy(&su->matrix[row], &permu[serial], su->size);
        for (int c = 0; c < su->size; c++)
            if (su->matrix[row][c] == su->matrix[row - 1][c])  // quick check
                goto loop;
        int ret = backtrack(su, row + 1, serial);
        if (ret == 1)
            continue;
        else if (ret == 0)
            return 0;
    }

    if (row == 1)
        return 2;
    else
        return 1;
}

/* generate a sudoku without skew rule
 */
short **semi_sudoku(short round)
{
    struct su *su = su_init(round);
    /* Since column 0 is always increasing, matrix[0][0] must be 0 to guarantee
     * the existence of a solution.
     */
    int pp = 1;  // partial permutation
    for (int i = 1; i <= round - 1; i++)
        pp *= i;
    int start = rand() % pp;
    printf("start = %d\n", start);
    vector_copy(&su->matrix[0], &permu[start], su->size);
    while (backtrack(su, 1, start) == 2) {
        start = rand() % start;  // shrink the starting number and re-try
        printf("start = %d\n", start);
        vector_copy(&su->matrix[0], &permu[start], su->size);
    }
    // backtrack success
    short **ans = su->matrix;
    free(su);
    return ans;
}
