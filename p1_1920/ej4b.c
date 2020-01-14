#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	pid_t hijo1, hijo2, childpid;
	int status;

	hijo1 = fork();

	switch(hijo1) {
		case -1:
			perror("fork error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			printf("proceso hijo %d; padre = %d\n", getpid(), getppid());
			if (execl(argv[1], argv[1], argv[2], NULL)<0) {
				perror("fallo en la ejecucion exec");
				printf("errno value = %d\n", errno);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
	}

	hijo2 = fork();

	switch(hijo2) {
		case -1:
			perror("fork error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
		printf("proceso hijo %d; padre = %d\n", getpid(), getppid());
		if (execl(argv[1], argv[1], argv[3], NULL)<0) {
			perror("fallo en la ejecucion exec");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}

	printf("proceso padre %d, con padre %d\n", getpid(), getppid());

	childpid = wait(&status);
	while (childpid > 0) {
		if (WIFEXITED(status)) { printf("child %d exited, status %d\n", childpid, WEXITSTATUS(status)); }
		else if (WIFSIGNALED(status)) { printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status)); }
		else if (WIFSTOPPED(status)) { printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status)); }
		else  {
			perror("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal");
			printf("errno value= %d\n", errno);
			exit(EXIT_FAILURE);
		}
		childpid = wait(&status);
	}

	exit(EXIT_SUCCESS);
}
