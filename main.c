#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Please provide a name of a file to open!\n");

        return -1;
    }

    if (argc > 2) {
        printf("Too many arguments! Please provide a name to a file!\n");

        return -1;
    }

    FILE* filePointer = fopen(argv[1], "rb");

    if (filePointer == NULL) {
        printf("Error: Couldn't open the file!\n");

        return -1;
    }

    unsigned char buffer[16];
    size_t bytesRead = 0;
    
    unsigned long long memoryOffset = 0;
    while ((bytesRead = fread(buffer, 1, 16, filePointer)) != 0) {
        printf("%016llX  |  ", memoryOffset);

        for(size_t i = 0; i < bytesRead; i++) {
            printf("%02X ", buffer[i]);
        }

        for (size_t j = 0; j < (16 - bytesRead); j++) {
            printf("   ");
        }

        printf(" |  ");

        for (size_t k = 0; k < bytesRead; k++) {
            if (buffer[k] >= 0x20 && buffer[k] <= 0x7e) {
                printf("%c", buffer[k]);
            } else {
                printf("%c", 0x2e);
            }
        }

        printf("\n");
        memoryOffset += 16;
    }

    fclose(filePointer);

    return 0;
}