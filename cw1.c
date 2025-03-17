#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

// Key Expansion: Generates a longer key using a simple PRNG (Pseudo-Random Number Generator)
void expandKey(const char *key, char *expandedKey, size_t length) {
    size_t keyLen = strlen(key);
    for (size_t i = 0; i < length; i++) {
        expandedKey[i] = key[i % keyLen] ^ (i * 31) % 256;  // Simple PRNG-like expansion
    }
}

// Encryption/Decryption function (XOR Cipher with Key Expansion)
void encryptDecrypt(const char *input, char *output, const char *key) {
    size_t inputLen = strlen(input);
    char expandedKey[inputLen];
    expandKey(key, expandedKey, inputLen);

    for (size_t i = 0; i < inputLen; i++) {
        output[i] = input[i] ^ expandedKey[i];
    }
    output[inputLen] = '\0';
}

// Encrypt/Decrypt a File
void encryptDecryptFile(const char *inputFile, const char *outputFile, const char *key) {
    FILE *in = fopen(inputFile, "rb");
    FILE *out = fopen(outputFile, "wb");

    if (!in || !out) {
        printf("Error opening file(s).\n");
        return;
    }

    char buffer[BUFFER_SIZE];
    char keyBuffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, BUFFER_SIZE, in)) > 0) {
        expandKey(key, keyBuffer, bytesRead);
        for (size_t i = 0; i < bytesRead; i++) {
            buffer[i] ^= keyBuffer[i];
        }
        fwrite(buffer, 1, bytesRead, out);
    }

    fclose(in);
    fclose(out);
    printf("File successfully processed!\n");
}

int main() {
    int choice;
    char text[BUFFER_SIZE], key[BUFFER_SIZE], encrypted[BUFFER_SIZE], decrypted[BUFFER_SIZE];
    char inputFile[256], outputFile[256];

    while (1) {
        printf("\n===== Encryptor / Decryptor Tool =====\n");
        printf("1. Encrypt Text\n");
        printf("2. Decrypt Text\n");
        printf("3. Encrypt File\n");
        printf("4. Decrypt File\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();  // Clear newline from buffer

        switch (choice) {
            case 1:
                printf("Enter text to encrypt: ");
                fgets(text, BUFFER_SIZE, stdin);
                text[strcspn(text, "\n")] = 0;

                printf("Enter key: ");
                fgets(key, BUFFER_SIZE, stdin);
                key[strcspn(key, "\n")] = 0;

                encryptDecrypt(text, encrypted, key);
                printf("Encrypted Text: %s\n", encrypted);
                break;

            case 2:
                printf("Enter text to decrypt: ");
                fgets(text, BUFFER_SIZE, stdin);
                text[strcspn(text, "\n")] = 0;

                printf("Enter key: ");
                fgets(key, BUFFER_SIZE, stdin);
                key[strcspn(key, "\n")] = 0;

                encryptDecrypt(text, decrypted, key);
                printf("Decrypted Text: %s\n", decrypted);
                break;

            case 3:
                printf("Enter input file name: ");
                scanf("%s", inputFile);
                printf("Enter output file name: ");
                scanf("%s", outputFile);
                printf("Enter key: ");
                scanf("%s", key);
                
                encryptDecryptFile(inputFile, outputFile, key);
                break;

            case 4:
                printf("Enter encrypted file name: ");
                scanf("%s", inputFile);
                printf("Enter output file name: ");
                scanf("%s", outputFile);
                printf("Enter key: ");
                scanf("%s", key);
                
                encryptDecryptFile(inputFile, outputFile, key);
                break;

            case 5:
                printf("Exiting program.\n");
                exit(0);

            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}
