#include "main.h"
#include <stdlib.h>

ssize_t read_textfile(const char *filename, size_t max_bytes)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
        return 0;

    char *buf = malloc(letters);
    if (!buf)
    {
        close(fd);
        return 0;
    }

    ssize_t t = read(fd, buf, letters);
    ssize_t w = write(STDOUT_FILENO, buf, t);

    free(buf);
    close(fd);
    return w;
}

