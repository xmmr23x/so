#include <sys/types.h> //Para tipo pid_t 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Para fork()
#include <errno.h>
#include <sys/wait.h>

int main() {
	pid_t pid, childpid;
	int status, n, i;

	printf("Cuantos procesos hijos se van a crear? ");
	scanf("%d", &n);

	for (i = 0; i < n; ++i) {
		pid = fork();

		switch(pid) {
			case -1:
				perror("fork error");
				printf("errno value= %d\n", errno);
				exit(EXIT_FAILURE); //Necesaria la librería <stdlib.h>
			case 0:
				printf("Proceso hijo %d; mi padre = %d \n", getpid(), getppid()); 
				exit(EXIT_SUCCESS);
		}
	}
	
	printf("Proceso padre %d; mi padre %d\n", getpid(), getppid());

	while ((childpid = wait(&status)) > 0) {
		if(childpid>0) {
			if (WIFEXITED(status)) {
				printf("child %d exited, status=%d\n",childpid, WEXITSTATUS(status));
			} 
				else if (WIFSIGNALED(status)) {
				printf("child %d killed (signal %d)\n", childpid, WTERMSIG(status));
			} 
				else if (WIFSTOPPED(status)) {
				printf("child %d stopped (signal %d)\n", childpid, WSTOPSIG(status));
			} 
		}
		else  {
			perror("Error en la invocacion de wait o la llamada ha sido interrumpida por una señal");
			printf("errno value= %d\n", errno);
			exit(EXIT_FAILURE);
		} 
	} ;
	
	exit(EXIT_SUCCESS); //return 0;
}