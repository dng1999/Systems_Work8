#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int key = ftok("makefile", 22);

    int semid = semget(key, 1, 0);
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_flg = SEM_UNDO;
    sb.sem_op = -1;
    if (semop(semid, &sb, 1) == -1)
    {
        printf("Resource unavailable.\n");
        return 1;
    }

    int shmid = shmget(key, 4, 0);
    int *len = shmat(shmid, NULL, 0);
    if (*len > 0)
    {
        char *s = malloc(*len * sizeof(char));
        int fd = open("story.txt", O_RDONLY);
        lseek(fd, -*len, SEEK_END);
        read(fd, s, *len * sizeof(char));
        printf("\t> %s\n", s);
        free(s);
    }
    printf("Add: ");
    char s[512];
    fgets(s, sizeof(s), stdin);
    int fd = open("story.txt", O_WRONLY|O_APPEND);
    write(fd, s, strlen(s));
    close(fd);
    *len = strlen(s);

    sb.sem_op = 1;
    semop(semid, &sb, 1);
}
