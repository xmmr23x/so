#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *incrementador(void *contador)
{
    extern pthread_mutex_t mutex;

    int i, a;
    struct timespec t;

    // Sleep structure
    t.tv_sec = 0;

    for (i = 0; i < 100; ++i) {
        pthread_mutex_lock(&mutex);
        (*(int *)contador)++;
        pthread_mutex_unlock(&mutex);

        // Sleep a few ms
        t.tv_nsec = rand() % 10000;
        nanosleep(&t, NULL);
    }
    printf("Contador FUNCIÓN=%i\n",*(int *)contador);
    pthread_exit(contador);
}

int main(int argc, char const *argv[])
{
    int nh, i, j, contador = 0, *final;

    srand(time(NULL));

    printf("Introduzca el numero de hilos\n");
    scanf("%i",&nh);
    pthread_t th[nh];

    for (i = 0; i < nh; i++)
        pthread_create(&th[i], NULL, incrementador, &contador);

    for (j = 0; j < nh; j++)
        pthread_join(th[j], (void **) &final);

    printf("Contador FINAL = %i\n", *final);

    return 0;
}

