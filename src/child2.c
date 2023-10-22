#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void delete_vowels(char* str){
    int i=0, j=0;
    while (str[i]){
        if (str[i] != 'a' && str[i] != 'u' && str[i] != 'e' && str[i] != 'o' && str[i] != 'i'
            && str[i] != 'A' && str[i] != 'U' && str[i] != 'E' && str[i] != 'O' && str[i] != 'I'){
            str[j++] = str[i];
        }
        i++;
    }
    str[j]='\0';
}

int main(){
    int fd_2[2];
    pipe(fd_2);
    close(fd_2[1]);
    int size;
    while (read(fd_2[0], &size, sizeof(int))>0){
        char buffer_2[255];
        read(fd_2[0], &buffer_2, sizeof(char) * size);
        delete_vowels(buffer_2);
        printf(buffer_2);
    }
}