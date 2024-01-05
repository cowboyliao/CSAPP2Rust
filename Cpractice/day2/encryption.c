#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>    // Include the OpenSSL EVP library

// Function to encrypt the input file and write the encrypted data to the output file
int encryptFile(const char *source, const char *destination, const char *key) {
    // Open the input and output files
    FILE *inputFile = fopen(source, "rb");
    FILE *outputFile = fopen(destination, "wb");
    if (inputFile == NULL || outputFile == NULL) {
        perror("Failed to open files");
        return -1;
    }

    // Create an encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (ctx == NULL) {
        perror("Failed to create encryption context");
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Initialize the encryption operation
    if (EVP_EncryptInit_ex(ctx, EVP_aes_128_gcm(), NULL, NULL, NULL) != 1) {
        perror("Failed to initialize encryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Set the AES-GCM encryption key
    if (EVP_EncryptInit_ex(ctx, NULL, NULL, (const unsigned char *)key, NULL) != 1) {
        perror("Failed to set encryption key");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Generate a random IV (Initialization Vector)
    unsigned char iv[12];    // Initialization Vector (12 bytes)
    if (EVP_EncryptInit_ex(ctx, NULL, NULL, NULL, NULL) != 1) {
        perror("Failed to generate IV");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_IV_GEN, sizeof(iv), iv) != 1) {
        perror("Failed to generate IV");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Write the IV to the output file
    if (fwrite(iv, 1, sizeof(iv), outputFile) != sizeof(iv)) {
        perror("Failed to write IV to output file");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Read and encrypt the data from the input file
    unsigned char input[1024];    // Input data buffer
    unsigned char output[1024];   // Output (encrypted) data buffer
    int bytesRead;
    int encryptedLen;
    
    while ((bytesRead = fread(input, 1, sizeof(input), inputFile)) > 0) {
        // Encrypt the input data and retrieve the encrypted data length
        if (EVP_EncryptUpdate(ctx, output, &encryptedLen, input, bytesRead) != 1) {
            perror("Failed to encrypt data");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return -1;
        }

        // Write the encrypted data to the output file
        if (fwrite(output, 1, encryptedLen, outputFile) != (size_t)encryptedLen) {
            perror("Failed to write encrypted data to output file");
            EVP_CIPHER_CTX_free(ctx);
            fclose(inputFile);
            fclose(outputFile);
            return -1;
        }
    }

    // Finalize the encryption and generate the authentication tag
    if (EVP_EncryptFinal_ex(ctx, output, &encryptedLen) != 1) {
        perror("Failed to finalize encryption");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Write the final encrypted data to the output file
    if (fwrite(output, 1, encryptedLen, outputFile) != (size_t)encryptedLen) {
        perror("Failed to write final encrypted data to output file");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }

    // Write the authentication tag to the output file
    unsigned char tag[16];   // Authentication Tag (16 bytes)
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, sizeof(tag), tag) != 1) {
        perror("Failed to get authentication tag");
        EVP_CIPHER_CTX_free(ctx);
        fclose(inputFile);
        fclose(outputFile);
        return -1;
    }
    if (fwrite(tag, 1, sizeof(tag), outputFile) != sizeof(tag)) {
        perror("Failed to write authentication tag to output file");
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
