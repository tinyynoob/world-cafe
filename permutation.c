#include "permutation.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static struct perm {
    short n;
    short **result;
    int resSize;
    int used;
    short *seq;  // seq[i] \in [0, n)
    unsigned checkxor;
};

static inline bool isValid(short *sequence, int seqSize, unsigned checkxor)
{
    for (int i = 0; i < seqSize; i++)
        checkxor ^= 1u << sequence[i];
    return !checkxor;
}

static void generate_permute(struct perm *p, int pos)
{
    if (pos == p->n) {  // end recursion
        if (!isValid(p->seq, p->n, p->checkxor))
            return;
        ++(p->used);
        for (int i = 0; i < p->n; i++)
            p->result[p->used][i] = p->seq[i];
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
    p->n = n;
    p->resSize = resSize;
    p->result = (short **) malloc(sizeof(short *) * resSize);
    for (int i = 0; i < resSize; i++)
        p->result[i] = (short *) malloc(sizeof(short) * n);
    p->used = -1;
    p->seq = (short *) malloc(sizeof(short) * n);
    p->checkxor = 0;
    for (int i = 0; i < n; i++) {
        p->seq[i] = 0;
        p->checkxor ^= 1u << i;
    }
    return p;
}

/* generate all permutations with elements [0, n)
 */
short **permutation(short n, int *returnSize)
{
    *returnSize = 1;
    for (int i = 1; i <= n; i++)
        (*returnSize) *= i;
    struct perm *p = perm_init(n, *returnSize);
    generate_permute(p, 0);
    short **ans = p->result;
    free(p->seq);
    free(p);
    return ans;
}