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

void help()
{
    printf("Invalid argument.\n"
            "Supported:\n"
            "-c\t\tCreate\n"
            "-r\t\tRemove\n"
            "-v\t\tView\n");
    exit(1);
}

void print_story()
{
    FILE *fp = fopen("story.txt", "r");
    if (!fp)
    {
        return;
    }
    char s[512];
    while (fgets(s, sizeof(s), fp))
    {
        printf("\t> %s", s);
        memset(s, '\0', sizeof(s));
    }
}

int main(int argc, char *argv[]){
    if (argc < 2)
    {
        help();
    }
    int key = ftok("makefile", 22);

    if (strcmp(argv[1], "-c") == 0)
    {
        int shmid = shmget(key, 4, IPC_CREAT|0644);
        int *len = shmat(shmid, NULL, 0);
        *len = 0;

        int semid = semget(key, 1, IPC_CREAT|0644);
        union semun data;
        data.val = 1;
        semctl(semid, 0, SETVAL, data);

        int fd = open("story.txt", O_CREAT|O_TRUNC, 0644);
        close(fd);
    }
    else if (strcmp(argv[1], "-v") == 0)
    {
        print_story();
    }
    else if (strcmp(argv[1], "-r") == 0)
    {
        int shmid = shmget(key, 4, 0);
        int *len = shmat(shmid, NULL, 0);
        int semid = semget(key, 1, 0);

        print_story();
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }
    else
    {
        help();
    }
    return 0;
}
