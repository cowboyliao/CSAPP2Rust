#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>    // Include the OpenSSL EVP library

// Function to decrypt the input file and write the decrypted data to the output file
int decryptFile(const char *source, const char *destination, const char *key) {
    // Open the input and output files
    FILE *inputFile = fopen(source, "rb");
    FILE *outputFile = fopen(destination, "wb");
    if (inputFile == NULL || outputFile == NULL) {
        perror("Failed to open files");
        return -1;
    }

    // Create a decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        perror("Failed to create decryption context");
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Initialize the decryption operation
    if (EVP_DecryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL) != 1) {
        perror("Failed to initialize decryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Set the AES-GCM decryption key
    if (EVP_DecryptInit_ex(ctx, NULL, NULL, (const unsigned char *)key, NULL) != 1) {
        perror("Failed to set decryption key");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Read the IV (Initialization Vector) from the input file
    unsigned char iv[12];    // Initialization Vector (12 bytes)
    if (fread(iv, 1, sizeof(iv), inputFile) != sizeof(iv)) {
        perror("Failed to read IV from input file");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Set the IV for decryption
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, sizeof(iv), NULL) != 1) {
        perror("Failed to set decryption IV length");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Set the encrypted tag (authentication tag) length for verification
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, NULL) != 1) {
        perror("Failed to set decryption tag length");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Read and decrypt the data from the input file
    unsigned char input[1024];    // Input data buffer
    unsigned char output[1024];   // Output (decrypted) data buffer
    int bytesRead;
    int decryptedLen;
    
    while ((bytesRead = fread(input, 1, sizeof(input), inputFile)) > 0) {
        
        // Decrypt the input data and retrieve the decrypted data length
        if (EVP_DecryptUpdate(ctx, output, &decryptedLen, input, bytesRead) != 1) {
            perror("Failed to decrypt data");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return -1;
        }

        // Write the decrypted data to the output file
        if (fwrite(output, 1, decryptedLen, outputFile) != (size_t)decryptedLen) {
            perror("Failed to write decrypted data to output file");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return -1;
        }
    }

    // Finalize the decryption and verify the authentication tag
    if (EVP_DecryptFinal_ex(ctx, output, &decryptedLen) != 1) {
        perror("Failed to finalize decryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Write the final decrypted data to the output file
    if (fwrite(output, 1, decryptedLen, outputFile) != (size_t)decryptedLen) {
        perror("Failed to write final decrypted data to output file");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Clean up
    EVP_CIPHER_CTX_free(ctx);
    fclose(inputFile);
    fclose(outputFile);

    return 0;
}
