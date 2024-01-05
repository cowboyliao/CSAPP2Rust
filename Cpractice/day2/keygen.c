#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void generateKey(char *key, int size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const int charsetLength = strlen(charset);
    srand(time(NULL));

    for (int i = 0; i < size; i++) {
        key[i] = charset[rand() % charsetLength];
    }

    key[size] = '\0';
}
