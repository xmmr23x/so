#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void * leeFichero(void * arg) {
	long *i = calloc(1, sizeof(long));
	char *cad = (char *)arg, cadena[200];
	FILE *f = fopen(cad, "r");

	while (fgets(cadena, 200, f)) (*i)++;

	printf("el fichero \"%s\" tiene %ld lineas\n", cad, *i);

	pthread_exit((void *)i);
}

int main(int argc, char *argv[]) {
	int n = argc - 1;
	long *aux, nlineas = 0;
	pthread_t thread[n];

	if (argc == 1) {
		printf("No se han indicado ficheros...\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < n; ++i) {
		if (pthread_create(&thread[i], NULL, (void *) leeFichero, (void *) argv[i+1])) {
			perror("error creando el hilo...");
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < n; ++i) {
		if (pthread_join(thread[i], (void **)&aux)) {
			perror("fallo en el join...");
			exit(EXIT_FAILURE);
		}

		nlineas += *(long *)aux;
	}

	printf("la suma total de las lineas es: %ld\n", nlineas);

	exit(EXIT_SUCCESS);
}
