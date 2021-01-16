/*
 * Init Lab - q1.c
 * 
 * Ecole polytechnique de Montreal, 2018
 */

// TODO
// Si besoin, ajouter ici les directives d'inclusion
// -------------------------------------------------
#include <stdio.h>
#include <unistd.h>
#include <string.h>
// -------------------------------------------------

/*
 * Vous devez imprimer le message indiqué dans l'énoncé:
 * - En exécutant un premier appel à printf AVANT l'appel à write
 * - Sans utiliser la fonction fflush
 * - En terminant chaque ligne par le caractère '\n' de fin de ligne
 */
void question1() {
    // TODO
    printf("62fe2ed9c10a8c2d8857fa576f2007d3 (printed using printf)");
    const char messageWrite[] = "62fe2ed9c10a8c2d8857fa576f2007d3 (printed using write)\n";
    write(STDOUT_FILENO, messageWrite, strlen(messageWrite));
    printf("\n");
}