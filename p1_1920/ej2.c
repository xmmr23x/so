#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int main() {
	pid_t pid, childpid;
	int status;

	pid = fork();

	switch(pid) {
		case -1:
			perror("fork error");
			printf("errno value = %d\n", errno);
			exit(EXIT_FAILURE);
		case 0:
			printf("proceso hijo %d; mi padre = %d\n", getpid(), getppid());
			exit(EXIT_SUCCESS);
		default:
			sleep(20);
			childpid = wait(&status);

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
			}
	}

	// El proceso padre es el que se crea al ejecutar el programa
	// su padre es el proceso de ejecucion de la terminal
	printf("Proceso padre %d; mi padre %d\n", getpid(), getppid());
	
	exit(EXIT_SUCCESS); //return 0;
}
