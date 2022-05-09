#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "permutation.h"

struct perm {
    short n;
    int **result;
    int resSize;
    int res_used;
    int *seq;   // seq[i] \in [0, n)
    unsigned checkxor;
};

static inline bool isValid(int *sequence, int seqSize, unsigned checkxor)
{
    for (int i = 0; i < seqSize; i++)
        checkxor ^= 1u << sequence[i];
    return !checkxor;
}

static void generate_permute(struct perm *p, int pos)
{
    if (pos == p->n) {   // end recursion
        if (!isValid(p->seq, p->n, p->checkxor))
            return;
        ++(p->res_used);
        for (int i = 0; i < p->n; i++)
            p->result[p->res_used][i] = p->seq[i];
        return;
    }

    for (int i = 0; i < p->n; i++) {
        p->seq[pos] = i;
        generate_permute(p, pos + 1);
    }
}

static struct perm *perm_init(short n, int resSize)
{
    struct perm *p = (struct perm *) malloc(sizeof(struct perm));
    p->resSize = resSize;
    p->result = (int **) malloc(sizeof(int *) * resSize);
    for (int i = 0; i < resSize; i++)
        p->result[i] = (int *) malloc(sizeof(int) * n);
    p->res_used = -1;
    p->seq = (int *) malloc(sizeof(int) * n);
    p->checkxor = 0;
    for (int i = 0; i < n; i++) {
        p->seq[i] = 0;
        p->checkxor ^= 1u << i;
    }
    return p;
}

/* generate permutations with elements [0, n)
 */
int** permutation(short n, int* returnSize)
{
    *returnSize = 1;
    for (int i = 1; i <= n; i++)
        (*returnSize) *= i;
    struct perm *p = perm_init(n, *returnSize);
    generate_permute(p, 0);
    int **ans = p->result;
    free(p->seq);
    free(p);
    return ans;
}