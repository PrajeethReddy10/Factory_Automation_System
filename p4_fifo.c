#include <stdio.h>
#define F 3

/* FIFO page replacement. Pages = parts of the machine-control program / recipe
   tables (0 = cutting recipe, 1 = welding, 2 = painting, 3 = packaging,
   4 = inspection, 5 = alarm handler) touched as production runs. */
int main(void) {
    int ref[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int n = sizeof ref / sizeof ref[0];
    int frame[F], next = 0, faults = 0, hits = 0;
    for (int i = 0; i < F; i++) frame[i] = -1;

    printf("FIFO, %d frames\n%-5s %-12s %s\n", F, "Page", "Frames", "Result");
    for (int t = 0; t < n; t++) {
        int hit = 0;
        for (int i = 0; i < F; i++) if (frame[i] == ref[t]) hit = 1;
        if (hit) hits++;
        else { frame[next] = ref[t]; next = (next + 1) % F; faults++; }  /* replace oldest */
        printf("%-5d ", ref[t]);
        for (int i = 0; i < F; i++) frame[i] < 0 ? printf("- ") : printf("%d ", frame[i]);
        printf("%*s%s\n", 6, "", hit ? "HIT" : "FAULT");
    }
    printf("\nTotal references = %d\nPage faults = %d\nPage hits   = %d\n", n, faults, hits);
    printf("Hit ratio = %.2f%%\n", 100.0 * hits / n);
    return 0;
}
