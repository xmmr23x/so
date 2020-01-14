#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

void createChild(int *n) {
	pid_t pid, childpid;
	int status;
	(*n)--;
	pid = fork();

	switch(pid) {
		case -1:
			perror("fork error");
			printf("errno value= %d\n", errno);
			exit(EXIT_FAILURE); //Necesaria la librería <stdlib.h>
		case 0:
			printf("Proceso hijo %d; mi padre = %d \n", getpid(), getppid()); 
			if (*n>0) createChild(n);
			exit(EXIT_SUCCESS);
		default:
			childpid = wait(&status);
			if(childpid>0) {
				if (WIFEXITED(status)) {
					printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
				} else if (WIFSIGNALED(status)) {
					printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
				} else if (WIFSTOPPED(status)) {
					printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
				} 
			}
			else  {
				perror("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal");
				printf("errno value= %d\n", errno);
				exit(EXIT_FAILURE);
			}
	}
}

int main() {
	int n;

	printf("Cuantos procesos hijos se van a crear? ");
	scanf("%d", &n);

	createChild(&n);

	// El proceso padre es el que se crea al ejecutar el programa
	// su padre es el proceso de ejecucion de la terminal
	printf("Proceso padre %d; mi padre %d\n", getpid(), getppid());
	
	exit(EXIT_SUCCESS); //return 0;
}
