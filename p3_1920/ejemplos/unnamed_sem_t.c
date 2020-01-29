/***************************************************************************
 *   Copyright (C) 2018 by Nicolas Garcia-Pedrajas                         *
 *   npedrajas@uco.es                                                      *
 *                                                                         *
 *   This program is FREE software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <semaphore.h>

void adder(int, sem_t *, bool);

#define ITER        1000
#define CHILDREN    5

// The process finished correctly
#define Ok  7

int main(int argc, char *argv[])
{
    int shmid, status;
    key_t key;
    double *counter;
    struct shmid_ds buf;
    sem_t mutex;
    bool use_semaphore;

    if (argc < 2) {
        printf("unnamed_sem_t y/n (use semaphore)\n");
        return 1;
    }

    use_semaphore = (argv[1][0] == 'y') ? true : false;

    // Shared memory
    key = ftok("unnamed_sem_t.c", 1);
    if ((shmid = shmget(key, sizeof(double), IPC_CREAT | 0777)) == -1)
        exit(1);

    counter = (double *) shmat(shmid, NULL, 0);
    *counter = 0.0;

    // Initialize
    sem_init(&mutex, 1, 1);

    // Run children
    for (int i = 0; i < CHILDREN; i++) {
        if (!fork()) {
            adder(i, &mutex, use_semaphore);
            exit(0);
        }
    }

    // Wait to finish
    for (int i = 0; i < CHILDREN; i++) {
        pid_t pid = wait(&status);
        printf("\nChild %d finished with status %d\n", pid, WEXITSTATUS(status));
    }

    // Final result
    fprintf(stdout, "Counter: %f\n", *counter);
    printf("WARNING: This program does not work, is an example of WHAT NOT TO DO!!!!\n");

    // Detach segment
    shmdt(counter);

    // Mark the segment to be destroyed
    shmctl(shmid, IPC_RMID, &buf);

    return 0;
}

void adder(int id, sem_t *mutex, bool use_it)
{
    double x = 1.0, l, *counter;
    int shmid;
    key_t key;

    // Shared memory
    key = ftok("unnamed_sem_t.c", 1);
    if ((shmid = shmget(key, sizeof(double), 0)) == -1)
        perror("Child: ");
    counter = (double *) shmat(shmid, NULL, 0);

    for (int i = 0; i < ITER; i++) {
        if (use_it)
            sem_wait(mutex);

        l = *counter;
        fprintf(stdout, "Process %d: %d ", id, (int) *counter);
        l += x;
        *counter = l;

        if (use_it)
            sem_post(mutex);
    }

    shmdt(counter);

    sem_close(mutex);

    exit(Ok);
}
