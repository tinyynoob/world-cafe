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
    int pp;  // partial permutation
};

static bool areColumnsValid(const struct su *su)
{
    unsigned check;
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
    su->pp = 1;
    for (int i = 1; i <= su->size - 1; i++)
        su->pp *= i;
    return su;
}

/* a solution is found if true is returned
 */
static bool backtrack(struct su *su, int row)
{
    if (row == su->size) {  // end recursion
        if (areColumnsValid(su))
            return true;
        else
            return false;
    }
    int serial = row * su->pp - 1;  // -1 to compensate
loop:;
    for (serial++; serial < (row + 1) * su->pp; serial++) {
        vector_copy(&su->matrix[row], &permu[serial], su->size);
        for (int c = 1; c < su->size; c++)
            if (su->matrix[row][c] == su->matrix[row - 1][c])  // quick check
                goto loop;
        if (backtrack(su, row + 1))
            return true;
        else
            continue;
    }
    return false;
}

/* generate a sudoku without skew rule
 */
short **semi_sudoku(short round)
{
    puts("enter semi_sudoku()...");
    struct su *su = su_init(round);
    /* Random generate half of the matrix
     * Since column 0 is always increasing, matrix[0][0] must be 0 to guarantee
     * the existence of a solution.
     */
    for (int r = 0; r < su->size - 2; r++) {
        int rng;
    regen:;
        rng = r * su->pp + ((unsigned) ((rand() << 16) | rand()) %
                            su->pp);  // matrix[r][0] is fixed
        vector_copy(&su->matrix[r], &permu[rng], su->size);
        for (int j = 0; j < r; j++) {
            for (int c = 1; c < su->size; c++)  // compare from [ ][1]
                if (su->matrix[r][c] == su->matrix[j][c])  // check validity
                    goto regen;
        }
    }
    print_matrix(su->matrix, su->size);
    puts("enter backtrack()...");
    int row = su->size - 2 > 0 ? su->size - 2 : 0;
    if (!backtrack(su, row))
        perror("ERROR happened at backtrack()\n");
    // backtrack success
    short **ans = su->matrix;
    free(su);
    return ans;
}
