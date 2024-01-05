#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>

int encryptFile(const char *source, const char *destination, const char *key) {
    FILE *sourceFile = fopen(source, "rb");
    if (sourceFile == NULL) {
        printf("Failed to open source file.\n");
        return -1;
    }

    FILE *destinationFile = fopen(destination, "wb");
    if (destinationFile == NULL) {
        printf("Failed to open destination file.\n");
        fclose(sourceFile);
        return -1;
    }
    
    // Initialize AES encryption with the provided key
    AES_KEY aesKey;
    if (AES_set_encrypt_key((const unsigned char *)key, 128, &aesKey) < 0) {
        printf("Failed to set encryption key.\n");
        fclose(sourceFile);
        fclose(destinationFile);
        return -1;
    }

    const int bufferSize = AES_BLOCK_SIZE;
    unsigned char input[bufferSize];
    unsigned char output[bufferSize];

    int bytesRead = 0;
    while ((bytesRead = fread(input, sizeof(unsigned char), bufferSize, sourceFile)) > 0) {
        AES_encrypt(input, output, &aesKey);
        fwrite(output, sizeof(unsigned char), bytesRead, destinationFile);
    }

    fclose(sourceFile);
    fclose(destinationFile);
    return 0;
}
