/*
 * Clone Lab - part2.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

#include "libprocesslab.h"

#include <stdio.h>

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
// -------------------------------------------------

#define PART2_OUTPUT_FILE_PATH "part2Output.txt"

void part2() {

    // Ouverture du fichier de sortie pour la question 2.3
    FILE* part2OutputFile = fopen(PART2_OUTPUT_FILE_PATH, "a");
    // TODO
    //level0
    char pidLevel0[100];
    char tokens[100] = "aa20661c904ca730307722fa";
    sprintf(pidLevel0, "%d", getpid());
    fprintf(part2OutputFile, "Root process has pid %s (message from process ", pidLevel0);
    if (fork() == 0) {
        //level1.1
        if (fork() == 0) {
            //level 2.1
            registerProc(2, 1, getpid(), getppid());
            fprintf(part2OutputFile, "level2.1)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.1", "level2.1", NULL);
            exit(1);
        }

        if (fork() == 0) {
            //level 2.2
            registerProc(2, 2, getpid(), getppid());
            fprintf(part2OutputFile, "level2.2)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.2", "level2.2", NULL);
            exit(1);
        }
        registerProc(1, 1, getpid(), getppid());
        fprintf(part2OutputFile, "level1.1)\n");
        fclose(part2OutputFile);
        execl("./part2/level1.1", "level1.1", pidLevel0, NULL);
        while(wait(NULL) > 0);
        exit(1);
    }

    if (fork() == 0) {
        //level1.2
        if (fork() == 0) {
            //level 2.3
            registerProc(2, 3, getpid(), getppid());
            char* envArgs[] = {"DISABLEFORK_TOKEN=86c00f80759f992afb3618d9", NULL};
            execle("./part2/level2.3", "level2.3", pidLevel0, NULL, envArgs);
            wait(NULL);
            exit(1);
        }
        registerProc(1, 2, getpid(), getppid());
        execl("./part2/level1.2", "level1.2", pidLevel0, NULL);
        wait(NULL);
        exit(1);
    }

    if (fork() == 0) {
        //level1.3
        if (fork() == 0) {
            //level 2.4
            registerProc(2, 4, getpid(), getppid());
            fprintf(part2OutputFile, "level2.4)\n");
            fclose(part2OutputFile);
            execl("./part2/level2.4", "level2.4", NULL);
            exit(1);
        }
        registerProc(1, 3, getpid(), getppid());
        fprintf(part2OutputFile, "level1.3)\n");
        fclose(part2OutputFile);
        execl("./part2/level1.3", "level1.3", pidLevel0, NULL);
        wait(NULL);
        exit(1);
    }

    if (fork() == 0) {
        //level1.4
        if (fork() == 0) {
            //level 2.5
            registerProc(2, 5, getpid(), getppid());
            execl("./part2/level2.5", "level2.5", NULL);
            wait(NULL);
            exit(1);
        }
        registerProc(1, 4, getpid(), getppid());
        execl("./part2/level1.4", "level1.4", pidLevel0, NULL);
        wait(NULL);
        exit(1);
    }
    fprintf(part2OutputFile, "level0)\n");
    fclose(part2OutputFile);

    execl("./part2/level0", "level0", tokens, NULL);

    while(wait(NULL) > 0);
    exit(1);
}