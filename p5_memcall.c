#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>

int main(void) {
    printf("--- sbrk()/brk(): sensor reading buffer on the heap ---\n");
    void *old = sbrk(0);                         /* current program break */
    int  *temp = sbrk(5 * sizeof(int));          /* grow heap for 5 readings */
    for (int i = 0; i < 5; i++) temp[i] = 60 + 2 * i;
    printf("Program break before : %p\n", old);
    printf("Program break after  : %p (+%zu bytes)\n", sbrk(0), 5 * sizeof(int));
    printf("Temperatures (C)     : %d %d %d %d %d\n", temp[0], temp[1], temp[2], temp[3], temp[4]);
    brk(old);                                    /* release: move break back */
    printf("Program break freed  : %p\n", sbrk(0));

    printf("\n--- mmap()/munmap(): shared production schedule ---\n");
    size_t len = 4096;
    char *m = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (m == MAP_FAILED) { perror("mmap"); return 1; }
    printf("mmap allocated %zu bytes at %p\n", len, (void *)m);
    strcpy(m, "Shift-A | B101 Gearbox x40 | CNC-M1 -> Robot-M2 -> Booth-M3");
    printf("Stored in mapping    : %s\n", m);
    if (munmap(m, len) == 0) printf("munmap released the %zu-byte region\n", len);
    return 0;
}
