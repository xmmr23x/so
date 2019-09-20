// practica 1
// autor: Manuel Mariscal
// file: p1_e2.c
// date: 5/10/2018
// El programa crea 3 procesos hijos para un proceso padre inicial. El padre crea un vector de enteros y lo rellena de forma aleatoria,
// mientras que los hijos realizan cada uno una tarea distinta.

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h> // para mostrar el error que se ha producido
#include <stdlib.h> // para generar numeros aleatorios
#include <time.h> // para generar numeros aleatorios

#define COUNT 100
#define COUNT1 10

int main() {
	pid_t pid;
	int status, child_pid, i, j, shmid;
	key_t key;
	int *vector;
	int indice, val, sum;

	srand(time(NULL));

	// Shared memory
	key = ftok("p1_e2.c", 1);
	if ((shmid = shmget(key, COUNT*sizeof(int), IPC_CREAT | 0777)) == -1) { perror("ftok"); exit(1); }

	vector = (int *) shmat(shmid, NULL, 0);

	// apartado 3. El proceso padre rellena el vector con numeros aleatorios
	for (i = 0; i < 100; ++i) {
		vector[i] = rand()%10;
	}

	printf("padre con pid: %ld\n", (long)getpid()); 

	for (i = 0; i < 3; ++i) { // apartado 1. crear 3 procesos hijos.
		pid = fork(); // crea el proceso hijo

		if (pid == -1) { // no se ha creado el proceso y devuelve un error
			printf("Error creando el proceso hijo.\n");
			exit(EXIT_FAILURE);
		} else if (pid == 0) { // el proceso hijo se ha creado, ejecutamos uns sentencia sitch para que cada hijo ejecute su tarea
			printf("\tProceso hijo: %d\n", getpid());

			switch (i) {
				case 0: // apartado 2.a
					key = ftok("p1_e2.c", 1);
					if ((shmid = shmget(key, COUNT*sizeof(int), 0)) == -1) { perror("ftok"); exit(1); }
					vector = (int *) shmat(shmid, NULL, 0);

					for (j = 0; j < COUNT1; ++j) {
						printf("\t\tPosicion del vector a modificar: ");
						scanf("%d", &indice);
						printf("\t\tNuevo valor: ");
						scanf("%d", &val);

						vector[indice] = val;
						printf("\t\tvector[%d] = %d\n", indice, vector[indice]);
					}
					exit(EXIT_SUCCESS);
				case 1: // apartado 2.b
					key = ftok("p1_e2.c", 1);
					if ((shmid = shmget(key, COUNT*sizeof(int), 0)) == -1) { perror("ftok"); exit(1); }
					vector = (int *) shmat(shmid, NULL, 0);

					for (j = 0; j < COUNT; ++j) {
						vector[rand()%COUNT] = rand()%10;
						sleep(1);
					}

					printf("\t\tvector = {");
					for (int j = 0; j < COUNT; ++j) {
						printf("%d", vector[j]);
						if (j != COUNT-1) { printf(", "); }
					}
					printf("}\n");
					exit(EXIT_SUCCESS);
				case 2: // apartado 2.c
					key = ftok("p1_e2.c", 1);
					if ((shmid = shmget(key, COUNT*sizeof(int), 0)) == -1) { perror("ftok"); exit(1); }
					vector = (int *) shmat(shmid, NULL, 0);

					for (j = 0; j < 5; ++j) {
						int a;
						sum = 0;
						for (a = 0; a < COUNT; ++a) {
							sum += vector[a];
						}
						printf("\t\tsuma numero %d = %d\n", j, sum);
						sleep(30); // hace que el programa se detenga durante 30 segundos
					}
            		exit(EXIT_SUCCESS);
			}

		} else {
			child_pid=wait(&status);
			if(child_pid>0) {
				if (WIFEXITED(status)) {
					printf("\tchild %d exited, status=%d\n\n",child_pid, WEXITSTATUS(status));
				} else if (WIFSIGNALED(status)) {
					printf("\tchild %d killed (signal %d)\n\n", child_pid, WTERMSIG(status));
						
				} else if (WIFSTOPPED(status)) {
					printf("\tchild %d stopped (signal %d)\n\n", child_pid, WSTOPSIG(status));
				} 
			}
		}
	}
}
