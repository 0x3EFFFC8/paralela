#include<stdio.h>
#include<string.h>

void encrypt(char *str, int rotation) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = (str[i] - 'a' + rotation) % 26 + 'a';
        else if (str[i] >= 'A' && str[i] <= 'Z') str[i] = (str[i] - 'A' + rotation) % 26 + 'A';
    }
}

int main(int argc, char *argv[]) {
    int rotation = atoi(argv[1]);
    char phrase[100];
    strcpy(phrase, argv[2]);
    encrypt(phrase, rotation);
    printf("Encrypted phrase: %s\n", phrase);
    return 0;
}
