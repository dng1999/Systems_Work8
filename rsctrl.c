#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO
			      (Linux-specific) */
};

int main(int argc, char *argv[]){
  if (argc < 2){
    printf("no arguments");
    return 1;
  }
  int key = ftok(argv[0], 22);
  
  if (strcmp(argv[1], "-c") == 0){
    int shmid = shmget(key, 4, IPC_CREAT|0644);
    struct shmid_ds buf;
    int semid = semget(key, 1, IPC_CREAT|0644);
    union semun data;
    data.val = 1;
    int file = open("story.txt", O_TRUNC, 0644);

    shmctl(shmid, IPC_STAT, &buf);
    semctl(semid, 0, IPC_SETVAL, data);
    printf("sem val: %d\n", data);
  }
  else if (strcmp(argv[1], "-r") == 0){
    int shmid = shmget(key, 4, 0);
    int semid = semget(key, 1, 0);
    shmctl(shmid, IPC_RMID, &buf);
    semctl(semid, 0, IPC_RMID);
  }
  else {
    printf("invalid argument");
  }
  return 0;
}
