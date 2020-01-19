#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

void * nombreFuncion(void * arg) {
	float a, b, *suma = malloc(sizeof(float));
	srand(time(NULL));

	a = rand() % 100;
	b = rand()% 100;

	*suma = a + b;

	printf("los valores aleatorios son a = %f y b = %f; la suma vale %f.\n", a, b, *suma);


	pthread_exit((void *) suma);
}

int main() {
	int n;
	float *sum, suma;

	printf("Numero de hilos: ");
	scanf("%d", &n);
	printf("%d\n", n);
	pthread_t thread[n];

	for (int i = 0; i < n; ++i) {
		if (pthread_create(&thread[i], NULL, (void *) nombreFuncion, NULL)) {
			perror("Error creando el hilo...\n");
			exit(EXIT_FAILURE);
		}

		sleep(1);
	}

	for (int i = 0; i < n; ++i) {
		if (pthread_join(thread[i], (void **) &sum)) {
			perror("Error en el join...\n");
			exit(EXIT_FAILURE);
		}
		
		suma += *(float *)sum;
	}

	printf("la suma total es: %f\n", suma);
	exit(EXIT_SUCCESS);
}
