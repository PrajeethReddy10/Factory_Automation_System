#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define SIZE  3          /* conveyor belt holds 3 parts */
#define ITEMS 6

int belt[SIZE], in = 0, out = 0;
sem_t empty, full, mutex;           /* free slots, filled slots, belt lock */

void *assembly(void *a) {           /* producer: assembly machine puts parts on belt */
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&empty); sem_wait(&mutex);
        belt[in] = 500 + i; in = (in + 1) % SIZE;
        printf("Assembly : placed part #%d on conveyor\n", 500 + i);
        sem_post(&mutex); sem_post(&full);
        usleep(50000);
    }
    return NULL;
}
void *packaging(void *a) {          /* consumer: packaging unit takes parts off */
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(&full); sem_wait(&mutex);
        int p = belt[out]; out = (out + 1) % SIZE;
        printf("Packaging:   packed part #%d\n", p);
        sem_post(&mutex); sem_post(&empty);
        usleep(120000);
    }
    return NULL;
}
int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    pthread_t p, c;
    sem_init(&empty, 0, SIZE); sem_init(&full, 0, 0); sem_init(&mutex, 0, 1);
    pthread_create(&p, NULL, assembly, NULL);
    pthread_create(&c, NULL, packaging, NULL);
    pthread_join(p, NULL); pthread_join(c, NULL);
    printf("All %d parts produced and packed.\n", ITEMS);
    return 0;
}
