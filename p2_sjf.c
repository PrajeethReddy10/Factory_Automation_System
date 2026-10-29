#include <stdio.h>
#define N 5

/* Non-preemptive SJF: production jobs arrive at the controller and the job
   with the shortest machining (burst) time among the arrived jobs runs next. */
int main(void) {
    const char *name[N] = {"J1-Cutting", "J2-Drilling", "J3-Welding", "J4-Painting", "J5-QualityChk"};
    int at[N] = {0, 1, 2, 3, 4};      /* arrival time (min) */
    int bt[N] = {6, 2, 8, 3, 4};      /* burst time = machine time needed (min) */
    int ct[N], done[N] = {0}, order[N];
    int time = 0, finished = 0;

    while (finished < N) {
        int p = -1;
        for (int i = 0; i < N; i++)               /* shortest arrived job */
            if (!done[i] && at[i] <= time && (p < 0 || bt[i] < bt[p])) p = i;
        if (p < 0) { time++; continue; }          /* CPU idle */
        time += bt[p]; ct[p] = time; done[p] = 1;
        order[finished++] = p;
    }

    printf("SJF (non-preemptive)\nGantt: ");
    int t = 0;
    for (int k = 0; k < N; k++) {
        int p = order[k];
        printf("| J%d (%d-%d) ", p + 1, t, ct[p]); t = ct[p];
    }
    printf("|\n\n%-15s %3s %3s %4s %4s %4s\n", "Job", "AT", "BT", "CT", "TAT", "WT");
    float tt = 0, tw = 0;
    for (int i = 0; i < N; i++) {
        int tat = ct[i] - at[i], wt = tat - bt[i];
        tt += tat; tw += wt;
        printf("%-15s %3d %3d %4d %4d %4d\n", name[i], at[i], bt[i], ct[i], tat, wt);
    }
    printf("\nAverage TAT = %.2f, Average WT = %.2f\n", tt / N, tw / N);
    return 0;
}
