#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct {
	long *vector;
	long *fila;
} Datos;

void * multiplicacion(void * arg) {
	Datos *dato = (Datos *)arg;
	long *res = calloc(1, sizeof(long));

	for (int i = 0; i < 3; ++i) {
		*res += (dato->fila[i] * dato->vector[i]);
	}

	pthread_exit((void *)res);
}

long ** reservaMatriz() {
	srand(time(NULL));

	long **m = (long **)malloc(3*sizeof(long *));
	for (int i = 0; i < 3; ++i) m[i] = (long *)malloc(3*sizeof(long));
	
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m[i][j] = rand() % 20;
		}
	}

	return m;
}

long * reservaVector() {
	srand(time(NULL));
	long *v = (long *)malloc(3*sizeof(long));

	for (int i = 0; i < 3; ++i) {
		v[i] = rand() % 20;
	}

	return v;
}

int main() {
	long *res, vresultado[3];
	// rellenar vector
	printf("Rellenando vector con datos aleatorios...\n");
	long *v = reservaVector();
	sleep(1);

	// rellenar matriz
	printf("Rellenando matriz con datos aleatorios...\n");
	long **m = reservaMatriz();

	Datos dato[3];

	for (int i = 0; i < 3; ++i) {
		dato[i].vector = v;
		dato[i].fila = m[i];
	}

	pthread_t thread[3];
	
	for (int i = 0; i < 3; i++) {
		if (pthread_create(&thread[i], NULL, (void *)multiplicacion, (void *)&dato[i])) {
			perror("error al crear el hilo...");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < 3; i++) {
		if (pthread_join(thread[i], (void **)&res)) {
			perror("error en el join...");
			exit(EXIT_FAILURE);
		}

		printf("calculando los datos en el hilo %d...\n", i);
		printf("fila de la matriz = {%ld,%ld,%ld}\n", dato[i].fila[0], dato[i].fila[1], dato[i].fila[2]);
		printf("vector = {%ld,%ld,%ld}\n", dato->vector[0], dato->vector[1], dato->vector[2]);
		printf("resulrado en el hilo %d = %ld\n", i, *(long *)res);
		vresultado[i] = *(long *)res;
	}

	sleep(1);
	printf("resultado final = {%ld,%ld,%ld}\n", vresultado[0], vresultado[1], vresultado[2]);

	exit(EXIT_SUCCESS);
}
