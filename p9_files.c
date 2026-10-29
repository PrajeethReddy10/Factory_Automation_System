#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

struct record { int batch; char machine[12]; char product[12]; int units; int defects; };

int main(void) {
    struct record r[3] = {
        {101, "CNC-M1",   "Gearbox",  40, 1},
        {102, "Robot-M2", "Chassis",  35, 0},
        {103, "Booth-M3", "Housing",  30, 2}};

    int fd = open("production.dat", O_CREAT | O_RDWR | O_TRUNC, 0640);    /* open */
    if (fd < 0) { perror("open"); return 1; }
    write(fd, r, sizeof r);                                               /* write */
    printf("Wrote %zu bytes (3 production records of %zu bytes)\n", sizeof r, sizeof r[0]);

    struct record x;
    lseek(fd, 2 * sizeof(struct record), SEEK_SET);                       /* jump to record 3 */
    read(fd, &x, sizeof x);                                               /* read */
    printf("Record 3 via lseek: batch=%d machine=%s product=%s units=%d defects=%d\n",
           x.batch, x.machine, x.product, x.units, x.defects);

    x.defects = 0; x.units = 32;                                          /* rework done */
    lseek(fd, 2 * sizeof(struct record), SEEK_SET);
    write(fd, &x, sizeof x);                                              /* update in place */
    lseek(fd, 2 * sizeof(struct record), SEEK_SET);
    read(fd, &x, sizeof x);
    printf("Updated record 3  : units=%d defects=%d\n", x.units, x.defects);

    struct stat st;
    stat("production.dat", &st);                                          /* stat */
    printf("stat: size=%ld bytes, permissions=%o, links=%ld\n",
           (long)st.st_size, st.st_mode & 0777, (long)st.st_nlink);
    close(fd);                                                            /* close */
    printf("File closed.\n");
    return 0;
}
