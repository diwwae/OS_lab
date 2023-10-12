#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void delete_vowels(char* str){
    int i=0, j=0;
    while (str[i]){
        char lower = tolower(str[i]);
        if (lower != 'a' && lower != 'u' && lower != 'e' && lower != 'o' && lower != 'i'){
            str[j++] = str[i];
        }
        i++;
    }
    str[j]='\0';
}

int main(){
    int fd_1[2], fd_2[2];

    char filename_1[256];
    char filename_2[256];
    printf("Enter first filename:");
    fgets(filename_1, sizeof filename_1, stdin);
    printf("Enter second filename:");
    fgets(filename_2, sizeof filename_2, stdin); 
    int File1 = open(filename_1, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    int File2 = open(filename_2, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    
    pipe(fd_1);
    pipe(fd_2);

    int pid_1 = fork();
    if (pid_1 == -1){
        printf("Error: creating process");
        return 1;
    }

    if (pid_1 == 0){
        //CHILD_1 PROCESS
        close(fd_1[1]);
        char buffer_1[255];
        int size;
        while (read(fd_1[0], &size, sizeof(int))>0){
            read(fd_1[0], &buffer_1, sizeof(char) * size);
            dup2(File1, STDOUT_FILENO);
            delete_vowels(buffer_1);
            printf(buffer_1);
        }
    } else {
        int pid_2 = fork();

        if (pid_2 == -1){
            printf("Error: creating process");
            return 2;
        }

        if (pid_2 == 0){
            //CHILD_2 PROCESS
            close(fd_2[1]);
            int size;
            while (read(fd_2[0], &size, sizeof(int))>0){
                char buffer_2[255];
                read(fd_2[0], &buffer_2, sizeof(char) * size);
                dup2(File2, STDOUT_FILENO);
                delete_vowels(buffer_2);
                printf(buffer_2);
            }

        } else {
            //PARENT PROCESS
            close(fd_1[0]);
            close(fd_2[0]);
            char buffer[255];

            printf("Enter lines:");
            while (1){
                fgets(buffer, sizeof(char) * 255, stdin);
                if (buffer[0] == '\n') {
                    break;
                }

                if (strlen(buffer) > 10){
                    int size = strlen(buffer) + 1;
                    write(fd_2[1], &size, sizeof(int));
                    write(fd_2[1], &buffer, sizeof(char) * size);
                } else {
                    int size = strlen(buffer) + 1;
                    write(fd_1[1], &size, sizeof(int));
                    write(fd_1[1], &buffer, sizeof(char) * size);
                }
            }
            close(fd_1[1]);
            close(fd_2[1]);
            close(File1);
            close(File2);
        }
    }
}