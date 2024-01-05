#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isValidFile(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Failed to open file '%s'.\n", filename);
        return 0;
    }
    fclose(file);
    return 1;
}

int isValidKey(const char *key) {
    const int requiredKeySize = 16; // 16 bytes = 128 bits for AES-128
    int keySize = strlen(key);
    if (keySize == requiredKeySize) {
        return 1;
    }
    else {
        printf("Invalid key size. Key must be %d characters (bytes).\n", requiredKeySize);
        return 0;
    }
}
