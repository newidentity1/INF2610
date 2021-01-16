#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int nbLecteurs = 0;

struct Identifiant
{
    int id_boucle;
    pthread_t* tid;
    sem_t* semaphoreEcriture;
    sem_t* semaphoreLecture;
    sem_t* semaphoreTour;
};

// Tirer des notes de cours chapitre 5 partie 1 p.58
void* acces_bd_ecriture(int id_boucle, pthread_t tid, sem_t* semaphoreEcriture, sem_t* semaphoreTour) {
    sem_wait(semaphoreTour); 
    sem_wait(semaphoreEcriture);
    sem_post(semaphoreTour);
    //section critique
    //écriture
    sem_post(semaphoreEcriture); 
    return NULL ;
}

// Tirer des notes de cours chapitre 5 partie 1 p.58
void* acces_bd_lecture(int id_boucle, pthread_t tid, sem_t* semaphoreLecture, sem_t* semaphoreEcriture, sem_t* semaphoreTour) {
    sem_wait(semaphoreTour);
    sem_wait(semaphoreLecture);
    if (nbLecteurs == 0) sem_wait(semaphoreEcriture);
    sem_post(semaphoreTour);
    nbLecteurs++;
    sem_post(semaphoreLecture);
    //lecture
    sem_wait(semaphoreLecture);
    nbLecteurs--;
    if (nbLecteurs == 0) sem_post(semaphoreEcriture);
    sem_post(semaphoreLecture);
}

void* acces_bd_lecture_wrapper(void* castedParams) {
    struct Identifiant* params = (struct Identifiant*) castedParams;
    return acces_bd_lecture(params->id_boucle, *params->tid, params->semaphoreLecture, params->semaphoreEcriture, params->semaphoreTour);
}

void* acces_bd_ecriture_wrapper(void* castedParams) {
    struct Identifiant* params = (struct Identifiant*) castedParams;
    return acces_bd_ecriture(params->id_boucle, *params->tid, params->semaphoreEcriture, params->semaphoreTour);
}

sem_t* init_sem() {
    sem_t* semaphore = calloc(1, sizeof(sem_t));
    sem_init(semaphore, 0, 1);
    return semaphore;
}

void detruire_sem(sem_t* sem) {
    sem_destroy(sem);
    free(sem);
}

/* Question 1
Seulement 1 accès exclusif en mode écriture peut se faire simultanément. Si plusieurs accès en mode écriture se font simultanément, cela conduira à une incohérence
de résultat puisqu'il y aura une condition de concurrence entre ses accès et il sera donc impossible de prédir avec certitude le résultat.
*/
int main() {
     pthread_t threadsLecteur[10];
     pthread_t threadsEcrivain[10];
     struct Identifiant* identifiantThreadLecteur[10];
     struct Identifiant* identifiantThreadEcrivain[10];
     sem_t* semaphoreLecture = init_sem();
     sem_t* semaphoreEcriture= init_sem();
     sem_t* semaphoreTour = init_sem();
     
     for (int i = 0; i < 10; ++i) {
         identifiantThreadEcrivain[i] = calloc(1, sizeof(struct Identifiant));
         identifiantThreadEcrivain[i]->id_boucle = i;
         identifiantThreadEcrivain[i]->tid = &threadsEcrivain[i];
         identifiantThreadEcrivain[i]->semaphoreEcriture = semaphoreEcriture;
         identifiantThreadEcrivain[i]->semaphoreTour = semaphoreTour;
        if ( pthread_create(&(threadsEcrivain[i]), NULL, acces_bd_ecriture_wrapper, (void*)identifiantThreadEcrivain[i]) != 0) return -1;
     }

     for (int i = 0; i < 10; ++i) {
         identifiantThreadLecteur[i] = calloc(1, sizeof(struct Identifiant));
         identifiantThreadLecteur[i]->id_boucle = i;
         identifiantThreadLecteur[i]->tid = &threadsLecteur[i];
         identifiantThreadLecteur[i]->semaphoreLecture = semaphoreLecture;
         identifiantThreadLecteur[i]->semaphoreEcriture = semaphoreEcriture;
         identifiantThreadLecteur[i]->semaphoreTour = semaphoreTour;
        if ( pthread_create(&(threadsLecteur[i]), NULL, acces_bd_lecture_wrapper, (void*)identifiantThreadLecteur[i]) != 0) return -1;
     }

     for (int i = 0; i < 10; ++i) {
        pthread_join(threadsEcrivain[i], NULL);
        pthread_join(threadsLecteur[i], NULL);
        free(identifiantThreadLecteur[i]);
        free(identifiantThreadEcrivain[i]);
     }

     detruire_sem(semaphoreLecture);
     detruire_sem(semaphoreEcriture);
     detruire_sem(semaphoreTour);

    return 0;
}
