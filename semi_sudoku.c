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
    int serial = row * su->pp - 1;  // compensate
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
    struct su *su = su_init(round);
    /* Random generate half of the matrix 
     * Since column 0 is always increasing, matrix[0][0] must be 0 to guarantee
     * the existence of a solution.
     */
    for (int i = 0; i <= su->size >> 1; i++) {
        int rng;
regen:;
        printf("i = %d\n", i);
        rng = i * su->pp + (rand() % su->pp);   // matrix[i][0] is fixed
        vector_copy(&su->matrix[i], &permu[rng], su->size);
        for (int j = 0; j < i; j++) {
            for (int c = 1; c < su->size; c++)  // compare from [ ][1]
                if (su->matrix[i][c] == su->matrix[j][c])   // check validity
                    goto regen;
        }
    }
    print_matrix(su->matrix, su->size);
    puts("enter backtrack()...");
    if (!backtrack(su, (su->size >> 1) + 1))
        perror("ERROR happened at backtrack()\n");
    // backtrack success
    short **ans = su->matrix;
    free(su);
    return ans;
}
