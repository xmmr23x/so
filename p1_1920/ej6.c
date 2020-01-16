#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void controladorAlarma() {
	printf("RING\n");
}

unsigned int alarma(unsigned int nsecs) {
	alarm(nsecs);
	pause();
}

int main() {
	int i = 0;

	if (signal(SIGALRM, controladorAlarma) == SIG_ERR) {
		perror("signal error");
		printf("errno value = %d\n", errno);
		exit(EXIT_FAILURE);
	}

	alarma(5);
	alarma(3);

	while (1) {
		i++;
		alarma(1);
		printf("timbrazo numero %d\n", i);
		if (i == 4) kill(getpid(), 9);
	}
}
