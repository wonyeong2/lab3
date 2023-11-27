#include <sys/types.h>
#include <semaphore.h>
#include <sys/ipc.h> 
#include <sys/shm.h>
#include <sys/sem.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#define SHMSIZE 1024
#define SHMKEY (key_t)0111
#define SEMKEY (key_t)0111

static int  semid;

union semapore
{
    int value;
    struct semid_ds *buf;
    int *array;
};

// parent process
int parent_process_read(char *filename)
{
	int shmid, len;
	void *shmaddr;
	FILE *fp = fopen(filename, "r");
	int size = strlen((char *)shmaddr);
	
	printf("read start\n");
	sleep(3);

	if ((shmid = shmget(SHMKEY, SHMSIZE, IPC_CREAT|0666)) == -1) { 
		perror("shmget failed"); 
		exit(1); 
	} 

	if ((shmaddr = shmat(shmid, NULL, 0)) == (void *)-1) { 
		perror("shmat failed"); 
		exit(1); 
	} 

	fgets((char *)shmaddr, size, fp);

	if (shmdt(shmaddr) == -1) { 
		perror("shmdt failed"); 
		exit(1);
	}

	printf("Read Success\n");
	fclose(fp);
	printf("read end.\n\n");
}

// child process
int child_process_write(char *filename)
{
	int shmid, len; 
	void *shmaddr;
	FILE *fp = fopen(filename, "w");

	printf("write start\n");
	sleep(2);

	if ((shmid = shmget(SHMKEY, SHMSIZE,IPC_CREAT|0666)) == -1) { 
		perror ("shmget failed"); 
		exit(1); 
	}

	if ((shmaddr=shmat(shmid, NULL, 0)) == (void *)-1) { 
		perror ("shmat failed"); 
		exit(1); 
	}

	fputs((char *)shmaddr, fp);
	printf("Copy Success!\n");

	if (shmdt(shmaddr) == -1) { 
		perror ("shmdt failed"); 
		exit (1); 
	}

	if (shmctl(shmid, IPC_RMID, 0) == -1) { 
		perror ("shmctl failed"); 
		exit (1); 
	}

	fclose(fp);
	printf("write end\n\n");
}

int main(int argc, char **argv)
{
	FILE* fp;
	union semapore semapore;

	struct sembuf mysem_open  = {0, -1, SEM_UNDO};
        struct sembuf mysem_close = {0, 1, SEM_UNDO};

	semid = semget((key_t)234, 1, 0660|IPC_CREAT);
    
	if (semid == -1) {
            perror("semget error");
            exit(0);
        }

	semapore.value = 1;

        if (semctl(semid, 0, SETVAL, semapore) == -1) {   
            perror("semctl error");
	    exit(1); 
        }

	if(semop(semid, &mysem_open, 1) == -1) {
            perror("semop error");
            exit(0);
        }

        if ((fp = fopen(argv[1], "r+")) == NULL) {
            perror("fopen error");
            exit(0);
        }

	parent_process_read(argv[1]);
	rewind(fp);

	sleep(2);

	child_process_write(argv[2]);
	fclose(fp);

	semop(semid, &mysem_close, 1);

    return 0;
}
