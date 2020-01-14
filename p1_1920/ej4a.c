#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	long int n = atoi(argv[1]), f = 1;

	for (int i = 1; i <= n; i++) {
		f *= i;
		printf("El factorial parcial de %d es %ld\n", n, f);
		sleep(1);
	}

	printf("El factorial de %d es %ld\n", n, f);
}
