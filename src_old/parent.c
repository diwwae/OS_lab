#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

size_t strlen(const char *str) {
    size_t length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}

int main(){
    int fd_1[2], fd_2[2];

    char filename_1[256];
    char filename_2[256];
    printf("Enter first filename:");
    fgets(filename_1, sizeof(filename_1), stdin);
    
    printf("Enter second filename:");
    fgets(filename_2, sizeof(filename_2), stdin); 

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

        // printf("Its child1");

        close(fd_1[1]);
   
        // fclose(stdout);
        // stdout = fopen(filename_1, "w");
        dup2(File1, STDOUT_FILENO);

        dup2(fd_1[0], STDIN_FILENO);
        
        execl("./child1", "child1", NULL);
        

    } else {

        int pid_2 = fork();

        if (pid_2 == -1){
            printf("Error: creating process");
            return 2;
        }

        if (pid_2 == 0){
            //CHILD_2 PROCESS

            // printf("Its child2");
            
            close(fd_2[1]);
            // fclose(stdout);
            // stdout = fopen(filename_2, "w");

            dup2(fd_2[0], STDIN_FILENO);
            dup2(File2, STDOUT_FILENO);

            execl("./child1", "child1", NULL);

        } else {
            //PARENT PROCESS

            // printf("Its parent");

            close(fd_1[0]);
            close(fd_2[0]);
            char buffer[255];

            printf("Enter number of lines:");
            int number;
            scanf("%d", number);


            printf("Enter lines:");

            for (int i=0; i<number; i++){
                fgets(buffer, sizeof(char) * 255, stdin);

                if (strlen(buffer) > 10){
                    
                    write(fd_2[1], &buffer, sizeof(buffer));

                } else {

                    write(fd_1[1], &buffer, sizeof(buffer));
                }
            }

            // while (1) {

            //     if (*buffer == EOF) {
            //         break;
            //     }

            //     fgets(buffer, sizeof(char) * 255, stdin);

            //     if (strlen(buffer) > 10){
                    
            //         write(fd_2[1], &buffer, sizeof(buffer));

            //     } else {

            //         write(fd_1[1], &buffer, sizeof(buffer));
            //     }
            // }
            wait(NULL);
            close(fd_1[1]);
            close(fd_2[1]);
            close(File1);
            close(File2);
        }
    }
    

}