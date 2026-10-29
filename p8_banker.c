#include <stdio.h>
#define P 5
#define R 3

/* Resources: A = Robotic arms (9), B = CNC machines (5), C = Forklifts (6)
   Processes P0-P4 = production jobs that hold and request these resources. */
int main(void) {
    int alloc[P][R] = {{1,1,0},{2,0,1},{2,1,1},{0,1,2},{1,0,1}};
    int max[P][R]   = {{4,3,2},{3,2,2},{6,2,3},{2,2,3},{3,1,2}};
    int total[R]    = {9, 5, 6};
    int avail[R], need[P][R], finish[P] = {0}, seq[P], count = 0;

    for (int j = 0; j < R; j++) {                     /* Available = Total - sum(Alloc) */
        avail[j] = total[j];
        for (int i = 0; i < P; i++) avail[j] -= alloc[i][j];
    }
    printf("%-5s %-8s %-8s %-8s\n", "Job", "Alloc", "Max", "Need");
    for (int i = 0; i < P; i++) {
        printf("P%-4d ", i);
        for (int j = 0; j < R; j++) printf("%d ", alloc[i][j]);
        printf("   ");
        for (int j = 0; j < R; j++) printf("%d ", max[i][j]);
        printf("   ");
        for (int j = 0; j < R; j++) { need[i][j] = max[i][j] - alloc[i][j]; printf("%d ", need[i][j]); }
        printf("\n");
    }
    printf("Total    : %d %d %d\nAvailable: %d %d %d\n\n",
           total[0], total[1], total[2], avail[0], avail[1], avail[2]);

    while (count < P) {
        int found = 0;
        for (int i = 0; i < P; i++) {
            if (finish[i]) continue;
            int ok = 1;
            for (int j = 0; j < R; j++) if (need[i][j] > avail[j]) { ok = 0; break; }
            if (ok) {
                for (int j = 0; j < R; j++) avail[j] += alloc[i][j];   /* job finishes, releases */
                printf("P%d runs -> Available = %d %d %d\n", i, avail[0], avail[1], avail[2]);
                seq[count++] = i; finish[i] = 1; found = 1;
            }
        }
        if (!found) { printf("\nSystem is in an UNSAFE state (possible deadlock)\n"); return 0; }
    }
    printf("\nSystem is in a SAFE state.\nSafe sequence: ");
    for (int i = 0; i < P; i++) printf("P%d%s", seq[i], i < P - 1 ? " -> " : "\n");
    return 0;
}
