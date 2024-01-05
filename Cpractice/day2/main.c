#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encryption.h"
#include "decryption.h"
#include "keygen.h"
#include "validation.h"
#include "error_handling.h"

#define MAX_KEY_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Usage: %s <-e/-d> <source> <destination> <key>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *mode = argv[1];
    char *source = argv[2];
    char *destination = argv[3];
    char *key = argv[4];

    if (strcmp(mode, "-e") == 0) {
        if (!isValidFile(source) || !isValidKey(key)) {
            printError("Invalid source file or key.");
            return EXIT_FAILURE;
        }

        if (encryptFile(source, destination, key) != 0) {
            printError("Encryption failed.");
            return EXIT_FAILURE;
        }

        printf("File encrypted successfully.\n");
    }
    else if (strcmp(mode, "-d") == 0) {
        if (!isValidFile(source) || !isValidKey(key)) {
            printError("Invalid source file or key.");
            return EXIT_FAILURE;
        }

        if (decryptFile(source, destination, key) != 0) {
            printError("Decryption failed.");
            return EXIT_FAILURE;
        }

        printf("File decrypted successfully.\n");
    }
    else {
        printError("Invalid mode.");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
