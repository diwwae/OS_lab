#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>


// Родительский процесс создает два дочерних процесса. Первой строкой пользователь в консоль
// родительского процесса вводит имя файла, которое будет использовано для открытия File с таким
// именем на запись для child1. Аналогично для второй строки и процесса child2. Родительский и
// дочерний процесс должны быть представлены разными программами.

// Родительский процесс принимает от пользователя строки произвольной длины и пересылает их в
// pipe1 или в pipe2 в зависимости от правила фильтрации. Процесс child1 и child2 производят работу
// над строками. Процессы пишут результаты своей работы в стандартный вывод.
// Вариант 17) Правило фильтрации: строки длины больше 10 символов отправляются в pipe2, иначе
// в pipe1. Дочерние процессы удаляют все гласные из строк

#define READ 0
#define WRITE 1

size_t strlen(const char *str) {
    size_t length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}

void delete_vowels(char* str){
    int letter=0, new_letter=0;
    while (str[letter]){
        if (str[letter] != 'a' && str[letter] != 'u' && str[letter] != 'e' && str[letter] != 'o' && str[letter] != 'i'
            && str[letter] != 'A' && str[letter] != 'U' && str[letter] != 'E' && str[letter] != 'O' && str[letter] != 'I'){
            str[new_letter++] = str[letter];
        }
        letter++;
    }
    str[new_letter]='\0';
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
        dup2(File1, STDOUT_FILENO);
        while (read(fd_1[READ], &size, sizeof(int))>0){
            read(fd_1[READ], &buffer_1, sizeof(char) * size);

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
            while (read(fd_2[READ], &size, sizeof(int))>0){
                char buffer_2[255];
                read(fd_2[READ], &buffer_2, sizeof(char) * size);
                dup2(File2, STDOUT_FILENO);
                delete_vowels(buffer_2);
                printf(buffer_2);
            }

        } else {
            //PARENT PROCESS
            close(fd_1[READ]);
            close(fd_2[READ]);
            char buffer[255];

            printf("Enter lines:");
            while (1){
                fgets(buffer, sizeof(char) * 255, stdin);
                if (buffer[0] == '\n') {
                    break;
                }

                if (strlen(buffer) > 10){
                    int size = strlen(buffer) + 1;
                    write(fd_2[WRITE], &size, sizeof(int));
                    write(fd_2[WRITE], &buffer, sizeof(char) * size);
                } else {
                    int size = strlen(buffer) + 1;
                    write(fd_1[WRITE], &size, sizeof(int));
                    write(fd_1[WRITE], &buffer, sizeof(char) * size);
                }
            }
            close(fd_1[WRITE]);
            close(fd_2[WRITE]);
            close(File1);
            close(File2);
            wait(NULL);
        }
    }
}