#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    mkfifo("/tmp/pipe_a", 0660);
    mkfifo("/tmp/pipe_b", 0660);
    char *jeton1 = "cfeb56cdac293b1c";
    char *jeton2 = "d05b5d1f71c62dfa";
    char m[50];

    char pid[100];
    sprintf(pid, "%d", getpid());

    if (fork() == 0)
    {
        // Fils 1
        // marioKart

        execl("./executables/Question1/marioKart", "marioKart", pid, NULL);
        wait(NULL);

        exit(1);
    }
    if (fork() == 0)
    {
        // Fils 2
        // horloge

        execl("./executables/Question1/horloge", "horloge", pid, NULL);
        wait(NULL);
        exit(1);
    }

    if (fork() == 0)
    {
        //Fils 3
        // pipe_a

        int fd = open("/tmp/pipe_a", O_WRONLY);
        write(fd, jeton1, strlen(jeton1));
        close(fd);
        wait(NULL);
        exit(0);
    }

    if (fork() == 0)
    {
        // Fils 4
        // pipe_b

        int fd = open("/tmp/pipe_b", O_WRONLY);
        write(fd, jeton2, strlen(jeton2));
        close(fd);
        wait(NULL);
        exit(0);
    }
    while (wait(NULL) > 0)
        ;

    exit(0);
}