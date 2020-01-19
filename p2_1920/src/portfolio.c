#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct {
	long *v;
	int n;
	int comienzo;
}datos;

void imprimeVector(long *v, int n, int comienzo) {
	printf("vector = {");
	for (int i = comienzo; i < comienzo + n; ++i) {
		printf("%d", v[i]);
		if (i != comienzo + n-1) printf(",");
	}
	printf("}\n");
} 

void * nombreFuncion(void * arg) {
	datos *arc = (datos *)arg;
	long *v = arc->v;
	int comienzo = arc->comienzo;
	int n = arc->n;
	long *suma = (long *)calloc(1, sizeof(long));

	imprimeVector(v, n, comienzo);

	for (int i = comienzo; i < comienzo + n; ++i) {
		*suma += v[i];
	}

	printf("suma = %d\n", *suma);

	pthread_exit((void *)suma);
}

long * reservaVector() {
	srand(time(NULL));
	long *v = (long *)malloc(10*sizeof(long));

	for (int i = 0; i < 10; ++i) v[i] = rand() % 10;

	return v;
}

int main(int argc, char *argv[]) {

	// comprobar de errores
	if (argc != 2) {
		printf("el numero de argumentos debe ser 2...\n");
		exit(EXIT_FAILURE);
	}

	int n = atoi(argv[1]);
	if (n < 2 || n > 10) {
		printf("el numero de hilos es incorrecto...\n");
		exit(EXIT_FAILURE);
	}

	// crear vector aleatorio
	long *v = reservaVector();
	imprimeVector(v, 10, 0);
	
	// variables
	datos *arg = malloc(n*sizeof(datos));
	arg[1].comienzo = 0;
	int mod = 10%n;
	long *suma, total = 0;
	
	// hilos
	pthread_t thread[n];

	for (int i = 0; i < n; ++i) {
		arg[i].v = v;
		if (mod != 0) { 
			arg[i].n = 1 + (10/n); 
			mod--;
		} else arg[i].n = 10/n;
		
		if (pthread_create(&thread[i], NULL, (void *)nombreFuncion, (void *)&arg[i])) {
			perror("error creando el hilo...");
			exit(EXIT_FAILURE);
		}
		
		if (i != n-1) arg[i+1].comienzo = arg[i].comienzo + arg[i].n;
	}

	for (int i = 0; i < n; ++i) {
		if (pthread_join(thread[i], (void **)&suma)) {
			perror("error en join...");
			exit(EXIT_FAILURE);
		}
		total += *(long *)suma;
	}

	printf("la suma total es: %d\n", total);

	exit(EXIT_SUCCESS);
}
