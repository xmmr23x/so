#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
	pid_t pid, childpid;
	int status;

	//////////////////////
	////	hijo 1	  ////
	//////////////////////

	pid = fork();

	switch(pid) {
		case -1:
			perror("fork error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			printf("proceso hijo 1 %d; mi padre = %d\n", getpid(), getppid());
			if (execlp(argv[1], argv[1], NULL)<0) {
				perror("exec");
				printf("errno value = %d\n", errno);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
	}

	//////////////////////
	////	hijo 2	  ////
	//////////////////////

	pid = fork();

	switch(pid) {
		case -1:
			perror("fork error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			printf("proceso hijo 2 %d; mi padre = %d\n", getpid(), getppid());
			if (execvp(argv[2], &argv[2])<0) {
				perror("exec");
				printf("errno value = %d\n", errno);
				exit(EXIT_FAILURE);
			}
			exit(EXIT_SUCCESS);
	}

	childpid = wait(&status);
	while (childpid>0) {
		printf("%d\n", childpid);
		if (childpid>0) {
			if (WIFEXITED(status)) {
				printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
			} else if (WIFSIGNALED(status)) {
				printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
			} else if (WIFSTOPPED(status)) {
				printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
			} else  {
				perror("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal");
				printf("errno value= %d\n", errno);
				exit(EXIT_FAILURE);
			}
		} else  {
			perror("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal");
			printf("errno value= %d\n", errno);
			exit(EXIT_FAILURE);
		}
		childpid = wait(&status);
	}

	// El proceso padre es el que se crea al ejecutar el programa
	// su padre es el proceso de ejecucion de la terminal
	printf("Proceso padre %d; mi padre %d\n", getpid(), getppid());
	
	exit(EXIT_SUCCESS); //return 0;
}
