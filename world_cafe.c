#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "permutation.h"
#include "semi_sudoku.h"

short **permu;
int permu_size;

static struct wc {  // world cafe
    short **players;
    int player_num;
    short round;
};

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

static struct wc *wc_init(short round, int player_num)
{
    struct wc *w = (struct wc *) malloc(sizeof(struct wc));
    w->player_num = player_num;
    w->players = (short **) malloc(sizeof(short *) * w->player_num);
    w->round = round;
    return w;
}

static inline void wc_swap_player(struct wc *w, int a, int b)
{
    short *temp = w->players[a];
    w->players[a] = w->players[b];
    w->players[b] = temp;
}

static void wc_shuffle_players(struct wc *w)
{
    for (int i = 0; i < w->player_num; i++) {
        int random = ((unsigned) ((rand() << 16) | rand())) % w->player_num;
        wc_swap_player(w, i, random);
    }
}

static void wc_printout_players(struct wc *w, FILE *f)
{
    for (int i = 0; i < w->player_num; i++) {
        fprintf(f, "player##%d,", i + 1);
        for (int r = 0; r < w->round; r++)
            fprintf(f, "%hd,", w->players[i][r] + 1);
        putc('\n', f);
    }
}

static void wc_printout_table(struct wc *w, FILE *f)
{
    // TODO
}

static inline void wc_destroy(struct wc *w)
{
    for (int i = 0; i < w->player_num; i++)
        free(w->players[i]);
    free(w->players);
    free(w);
}

int main()
{
    srand(time(NULL));
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
    permu = permutation(round, &permu_size);
    FILE *f = fopen("world_cafe.tmp", "w");
    for (int count = 0; count < player + round;) {
        short **matrix = semi_sudoku(round);
        for (int i = 0; i < round; i++) {
            ++count;
            for (int j = 0; j < round; j++)
                fprintf(f, "%hd,", matrix[i][j]);
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

    f = fopen("world_cafe.tmp", "r");
    struct wc *w = wc_init(round, player);
    for (int i = 0; i < w->player_num; i++) {
        w->players[i] = (short *) malloc(sizeof(short) * w->round);
        for (int r = 0; r < w->round; r++)
            fscanf(f, "%hd,", &w->players[i][r]);
        fscanf(f, "\n");
    }
    fclose(f);
    remove("world_cafe.tmp");
    wc_shuffle_players(w);
    f = fopen("world_cafe.csv", "w");
    wc_printout_players(w, f);
    fclose(f);
    wc_destroy(w);
    w = NULL;
    return 0;
}
