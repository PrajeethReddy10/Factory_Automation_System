#include <stdio.h>
#define BLOCKS 32

int used[BLOCKS], next_blk[BLOCKS];   /* next_blk[b] = pointer stored in block b (-1 = end) */

int find_free(int from) {             /* next free block scanning forward */
    for (int i = from; i < BLOCKS; i++) if (!used[i]) return i;
    return -1;
}
void create_file(const char *name, int nblocks) {   /* linked allocation */
    int start = find_free(0), cur = start;
    used[cur] = 1;
    for (int k = 1; k < nblocks; k++) {
        int nb = find_free(cur + 1);
        used[nb] = 1; next_blk[cur] = nb; cur = nb;
    }
    next_blk[cur] = -1;
    printf("%-16s start=%2d end=%2d  chain: ", name, start, cur);
    for (int b = start; b != -1; b = next_blk[b]) printf("%d%s", b, next_blk[b] == -1 ? " -> NULL\n" : " -> ");
}
int main(void) {
    int reserved[] = {0, 1, 2, 4, 7, 8, 11, 15, 16, 20};   /* OS / already used blocks */
    for (int i = 0; i < 10; i++) used[reserved[i]] = 1;

    printf("Linked allocation on a %d-block disk\n", BLOCKS);
    create_file("production.dat", 4);
    create_file("machine.log", 3);
    create_file("maintenance.db", 5);

    printf("\nBlock map (# = used, . = free):\n");
    for (int i = 0; i < BLOCKS; i++) printf("%c", used[i] ? '#' : '.');
    int fb = 0; for (int i = 0; i < BLOCKS; i++) fb += !used[i];
    printf("\nFree blocks left = %d\n", fb);
    return 0;
}
