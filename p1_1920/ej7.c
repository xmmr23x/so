#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>


// controlador de la señal
// imprime un mensaje por pantalla cada vez que recibe una señal.
void tratarSennal(int signo) {
	printf("yo, hijo %d, he recibido la señal de mi padre %d!!!\n", getpid(), getppid());
	printf("Value of signo:%d\n",signo);
}

// programa principal
// el proceso padre crea un hijo y le envia una señal persnonalizada cada segundo.
int main() {
	pid_t pid, childpid;
	int i,status;

	pid = fork();

	switch(pid) {
		case -1:
			perror("fork error");
			printf("error value = %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			printf("hijo con pid %d\n", getpid());
			if (signal(SIGUSR1, tratarSennal) == SIG_ERR) {
				perror("signal error");
				printf("errno value = %d\n", errno);
				exit(EXIT_FAILURE);
			}
			for (i = 0; i < 5; ++i) pause();
			exit(EXIT_SUCCESS);
		default:
			for (i = 0; i < 5; ++i) {
				sleep(1);
				kill(pid, SIGUSR1);
			}
	}

	childpid = wait(&status);
	if (childpid > 0) {
		if (WIFEXITED(status)) printf("child %d exited, status = %d\n", childpid, WEXITSTATUS(status));
		else if (WIFSIGNALED(status)) printf("child %d exited, status = %d\n", childpid, WTERMSIG(status));
		else if (WIFSTOPPED(status)) printf("child %d exited, status = %d\n", childpid, WSTOPSIG(status));
	} else {
		perror("error en la invocacion de wait.");
		printf("errno value = %d\n", errno);
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
