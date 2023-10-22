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

int main(int argc, char* argv[]){

    char buffer_1[255];

    read(STDIN_FILENO, buffer_1, 255);

    delete_vowels(buffer_1);

    printf(buffer_1);

    return 0;

}