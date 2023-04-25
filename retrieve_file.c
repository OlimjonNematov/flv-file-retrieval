#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h> // include string.h for memcmp

#define BLOCK_SIZE 4096

int retrieve_file(int fd)
{
    char *data = malloc(BLOCK_SIZE);
    if (data == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    size_t data_size = 0;
    ssize_t bytes_read;

    while ((bytes_read = read(fd, &data[data_size], BLOCK_SIZE)) > 0)
    {
        data_size += bytes_read;
        char *tmp = realloc(data, data_size + BLOCK_SIZE);

        if (tmp == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }

        data = tmp;

        for (int i = 0; i < bytes_read - 8; i++)
        {
            if (data[data_size - bytes_read + i] == 0x46 &&
                data[data_size - bytes_read + i + 1] == 0x4C &&
                data[data_size - bytes_read + i + 2] == 0x56 &&
                data[data_size - bytes_read + i + 3] == 0x01 &&
                data[data_size - bytes_read + i + 4] == 0x05 &&
                data[data_size - bytes_read + i + 5] == 0x00 &&
                data[data_size - bytes_read + i + 6] == 0x00 &&
                data[data_size - bytes_read + i + 7] == 0x00 &&
                data[data_size - bytes_read + i + 8] == 0x09)
            {
                printf("%02X %02X %02X %02X %02X %02X %02X %02X %02X\n",
                       data[data_size - bytes_read + i], data[data_size - bytes_read + i + 1], data[data_size - bytes_read + i + 2],
                       data[data_size - bytes_read + i + 3], data[data_size - bytes_read + i + 4], data[data_size - bytes_read + i + 5],
                       data[data_size - bytes_read + i + 6], data[data_size - bytes_read + i + 7], data[data_size - bytes_read + i + 8]);
            }
        }

        if (memcmp(data, "\x46\x4C\x56\x01\x05\x00\x00\x00\x09", 9 * sizeof(char)) == 0) // use sizeof(char) to specify the correct type for the third argument of memcmp
        {
            printf("Found FLV signature at offset 0\n");

            // Process the data
            // ...
        }
        else
        {
            // printf("No FLV signature found at offset 0\n");
        }
    }

    if (bytes_read == -1)
    {
        perror("read");
        free(data);
        exit(EXIT_FAILURE);
    }

    if (data_size == 0)
    {
        fprintf(stderr, "No data read from partition\n");
        free(data);
        exit(EXIT_FAILURE);
    }

    printf("Read %zu bytes from partition:\n", data_size);
    // print or process the data as needed

    free(data);
    return 0;
}
