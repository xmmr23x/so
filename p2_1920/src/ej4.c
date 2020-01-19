#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;

void * nombreFuncion() {
	for (int i = 0; i < 50; ++i) {
		x++;
	}
}

int main() {
	pthread_t thread[2];

	for (int i = 0; i < 2; ++i) {
		pthread_create(&thread[i], NULL, (void *) nombreFuncion, NULL);
	}
		
	pthread_join(thread[0], NULL);
	pthread_join(thread[1], NULL);

	printf("x = %d\n", x);

	exit(EXIT_SUCCESS);
}
