#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

/* Machine processes send status messages to the controller through a
   System V message queue. mtype = machine id, so messages can be filtered. */
struct msg { long mtype; char text[80]; };

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    int qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);      /* create queue */
    if (qid < 0) { perror("msgget"); return 1; }

    const char *status[] = {"CNC-M1 : batch B101 cut, 40 units OK",
                            "Robot-M2: weld done, temp 68C",
                            "Booth-M3: paint level LOW - refill needed"};
    for (int m = 0; m < 3; m++) {
        if (fork() == 0) {                                /* child = machine */
            struct msg s = { m + 1, "" };
            strcpy(s.text, status[m]);
            msgsnd(qid, &s, sizeof s.text, 0);
            printf("Machine %d (PID %d) sent   -> %s\n", m + 1, getpid(), s.text);
            exit(0);
        }
    }
    while (wait(NULL) > 0);                               /* all machines reported */

    struct msg r;
    for (int m = 1; m <= 3; m++) {                        /* controller reads by type */
        msgrcv(qid, &r, sizeof r.text, m, 0);
        printf("Controller received <- [type %ld] %s\n", r.mtype, r.text);
    }
    msgctl(qid, IPC_RMID, NULL);                          /* remove queue */
    printf("Message queue removed.\n");
    return 0;
}
