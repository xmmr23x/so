// practica 1 ejercicio 1
// autor: Manuel Mariscal
// file: p1.c
// date: 5/10/2018

// ejecrcicio 1

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main() {
	pid_t pid;
	int status, child_pid, i;

	for (i = 0; i < 3; i++) {
		pid = fork(); // crea el proceso hijo

		if (pid == -1) {
			printf("Error creando el proceso hijo.\n");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			printf("\tProceso hijo: %d\n", getpid());
			exit(EXIT_SUCCESS); // Cada vez que el proceso hijo acaba, hay que salir de el. Necesaria la librería <stdlib.h> 
		} else {
			printf("padre con pid: %ld\n", (long)getpid()); 
			child_pid=wait(&status);
			if(child_pid>0) {
				if (WIFEXITED(status)) {
					printf("\tchild %d exited, status=%d\n",child_pid, WEXITSTATUS(status));
				} else if (WIFSIGNALED(status)) {
					printf("\tchild %d killed (signal %d)\n", child_pid, WTERMSIG(status));
						
				} else if (WIFSTOPPED(status)) {
					printf("\tchild %d stopped (signal %d)\n", child_pid, WSTOPSIG(status));
				} 
			}
		}
	}
}
