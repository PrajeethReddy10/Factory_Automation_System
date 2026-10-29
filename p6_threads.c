#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/* Each thread is one concurrent machine activity on the factory floor. */
void *conveyor(void *a) {
    for (int i = 1; i <= 2; i++) { usleep(100000); printf("Thread 1 [Conveyor]   : moved tray %d to next station\n", i); }
    return NULL;
}
void *robot_arm(void *a) {
    for (int i = 1; i <= 2; i++) { usleep(150000); printf("Thread 2 [Robot Arm]  : welded part %d\n", i); }
    return NULL;
}
void *monitor(void *a) {
    for (int i = 1; i <= 2; i++) { usleep(120000); printf("Thread 3 [Temp Sensor]: reading %d = %d C\n", i, 64 + i); }
    return NULL;
}
void *inspection(void *a) {
    usleep(330000); printf("Thread 4 [Inspection] : batch B101 passed QC\n");
    return NULL;
}

int main(void) {
    pthread_t t[4];
    void *(*fn[4])(void *) = {conveyor, robot_arm, monitor, inspection};
    printf("Main: starting 4 machine-activity threads\n");
    for (int i = 0; i < 4; i++) pthread_create(&t[i], NULL, fn[i], NULL);
    for (int i = 0; i < 4; i++) pthread_join(t[i], NULL);
    printf("Main: all threads joined - shift cycle complete\n");
    return 0;
}
