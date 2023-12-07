const char _G_[] = "\x1b[32m";// green color for parent process output color
const char _P_[] = "\x1b[35m";// purple color for errors process output color
const int MAX_WORDS = 100;
const int MAX_LENGTH = 20;
const int FILE_NAME_SIZE = 20;

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
// #include <sys/types.h>
#include <fcntl.h>

int isVowel(char letter) {
    switch (letter) {
        case 'A':
        case 'E':
        case 'I':
        case 'O':
        case 'U':
        case 'Y':
        case 'a':
        case 'e':
        case 'i':
        case 'o':
        case 'u':
        case 'y':
            return 1;

        default:
            return 0;
    }
}

int contain(char letter, char word[MAX_WORDS]) {
    for (int i = 0; i < MAX_LENGTH; ++i) {
        if (word[i] == letter) {
            return 1;
        }
    }
    return 0;
}

void write_to_file(char filename[FILE_NAME_SIZE], char words[MAX_WORDS][MAX_LENGTH]) {
    int fd = open(filename, O_RDONLY, S_IRUSR | S_IWUSR);
    struct stat sb;

    if (fstat(fd, &sb) == -1){
        perror("couldn't get file size.\n");
    }

    char *file = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

    if (file == NULL) {
        printf("file wasn't open",_P_);
        exit(-1);
    }

    for (int index_of_word = 0; index_of_word < MAX_WORDS && words[index_of_word][0] != EOF; ++index_of_word) {
        if (contain('_', words[index_of_word])) {
            for (int index_of_letters = 0;
                 index_of_letters < MAX_LENGTH && words[index_of_word][index_of_letters] != '\0'; ++index_of_letters) {
                if (words[index_of_word][index_of_letters] != '_') {
                    fprintf(file, "%c", words[index_of_word][index_of_letters]);
                    printf("%c", words[index_of_word][index_of_letters]);
                }
            }
            fprintf(file, "%c", '\n');
            printf("\n");
            continue;
        }
        fprintf(file, "%s\n", words[index_of_word]);
        printf("%s\n", words[index_of_word]);
    }
    munmap(file, sb.st_size);
    close(fd);
}

void remove_vowels(char filename[FILE_NAME_SIZE], char words[MAX_WORDS][MAX_LENGTH]) {
    printf(_G_);
    for (int index_of_word = 0; index_of_word < MAX_WORDS && words[index_of_word][0] != '\0'; ++index_of_word) {
        for (int index_of_letter = 0;
             index_of_letter < MAX_LENGTH && words[index_of_word][index_of_letter] != '\0'; ++index_of_letter) {
            if (isVowel(words[index_of_word][index_of_letter])) {
                words[index_of_word][index_of_letter] = '_';
            }
        }
    }
}

int main(int argc, char* argv[]) {
    printf(_G_);
    char filename[FILE_NAME_SIZE];
    filename[FILE_NAME_SIZE] = argv[0];

    FILE* file = fopen(filename, "r");

    char words[MAX_WORDS][MAX_LENGTH];
    for (int counter_of_input_words = 0; counter_of_input_words < MAX_WORDS; ++counter_of_input_words) {
        read(STDIN_FILENO, words[counter_of_input_words], sizeof(words[counter_of_input_words]));
    }


    remove_vowels(filename, words);
    write_to_file(filename, words);

}