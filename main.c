#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include "retrieve_file.c"

extern int retrieve_file(int fd);

int main(int argc, char *argv[])
{
    int fd;
    char buffer[4096];
    int num_indirect_blocks = 0;

    fd = open("/dev/sdc2", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    if (fd >= 0)
    {
        retrieve_file(fd);
    }

    return 0;
}