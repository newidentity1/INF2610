#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int fd[2];
    if (pipe(fd) != -1)
    {
        if (fork() == 0)
        {
            // commande 1
            // ./prog1 2>&1 | ./prog2
            dup2(STDOUT_FILENO, STDERR_FILENO);
            dup2(fd[1], STDERR_FILENO);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[0]);
            close(fd[1]);
            execl("./executables/Question2/prog1", "prog1", NULL);
            wait(NULL);
            exit(1);
        }

        if (fork() == 0)
        {
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);
            close(fd[1]);
            execl("./executables/Question2/prog2", "prog2", NULL);
            wait(NULL);
            exit(1);
        }
        while (wait(NULL) > 0)
            ;
        close(fd[0]);
        close(fd[1]);
    }

    if (fork() == 0)
    {
        // commande 2
        // ./prog3 < tmpfile
        int file = open("./executables/Question2/tmpfile", O_RDONLY);
        dup2(file, STDIN_FILENO);
        close(file);
        execl("./executables/Question2/prog3", "prog3", NULL);
        wait(NULL);
        exit(1);
    }

    while (wait(NULL) > 0)
        ;

    exit(0);
}