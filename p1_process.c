#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* Production controller (parent) creates one child process per production task.
   Each child "runs" its machine and returns the units produced as its exit status. */
int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);            /* unbuffered: clean output across fork() */
    const char *task[]    = {"Cutting   (CNC-M1)", "Welding   (Robot-M2)",
                             "Painting  (Booth-M3)", "Packaging (Line-M4)"};
    const int   units[]   = {40, 35, 30, 25};    /* units produced by each task */
    const int   N = 4;

    printf("Production Controller (parent): PID=%d, PPID=%d\n", getpid(), getppid());
    for (int i = 0; i < N; i++) {
        pid_t pid = fork();                      /* one child per production task */
        if (pid < 0) { perror("fork"); exit(1); }
        if (pid == 0) {                          /* child = production task */
            printf("  Task %d %-20s: child PID=%d, parent PID=%d\n",
                   i + 1, task[i], getpid(), getppid());
            exit(units[i]);                      /* exit status = units produced */
        }
        int status;
        pid_t done = wait(&status);              /* controller waits for this stage */
        if (WIFEXITED(status))
            printf("  Controller: task PID %d finished, produced %d units\n",
                   done, WEXITSTATUS(status));
    }
    printf("Production Controller: all %d production tasks completed.\n", N);
    return 0;
}
