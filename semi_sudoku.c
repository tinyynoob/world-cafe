#include <stdlib.h>
#include <stdbool.h>
#include "semi_sudoku.h"

extern short **permu;
extern int permu_size;

static struct su {
    short size;     // = round
    short **matrix;
    int serial;     // count to permu_size
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

static inline void swap(short *a, short *b)
{
    short temp = *a;
    *a = *b;
    *b = temp;
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
    // su->serial = rand() % permu_size;
    su->serial = 0;
    vector_copy(&su->matrix[0], &permu[su->serial], su->size);
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
static int backtrack(struct su *su, int row)
{
    print_matrix(su->matrix, su->size);
    if (row == su->size) {  // end recursion
        if (isValid(su))
            return 0;
        else
            return 1;
    }
    do {
        ++su->serial;
        vector_copy(&su->matrix[row], &permu[su->serial], su->size);
        // if (su->matrix[row][row - 1] == su->matrix[row - 1][row - 1])   // quick check
        //     continue;
        int ret = backtrack(su, row + 1);
        if (ret == 1)
            continue;
        else if (ret == 0)
            return ret;
    } while (su->serial < permu_size);
    return 2;
}

/* generate a sudoku without skew rule
 */
short **semi_sudoku(short round)
{
    struct su *su = su_init(round);
    while (backtrack(su, 1) == 2) {
        su->serial = rand() % permu_size;   // change the starting and re-try
        vector_copy(&su->matrix[0], &permu[su->serial], su->size);
    }
    // backtrack success
    short **ans = su->matrix;
    free(su);
    for (int i = 0; i < permu_size; i++)
        free(permu[i]);
    free(permu);
    return ans;
}

