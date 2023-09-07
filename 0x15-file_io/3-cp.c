#include "main.h"
#include <stdio.h>
#include <stdlib.h>

char *create_buffer(char *file);
void close_file(int fd);

char *create_buffer(char *file)
{
    char *buffer = malloc(1024);
    if (!buffer)
    {
        dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file);
        exit(99);
    }
    return buffer;
}

void close_file(int fd)
{
    if (close(fd) == -1)
    {
        dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
        exit(100);
    }
}

int main(int argc, char *argv[])
{
    int from, to, r, w;
    char *buffer;

    if (argc != 3)
    {
        dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
        exit(97);
    }

    buffer = create_buffer(argv[2]);
    from = open(argv[1], O_RDONLY);
    to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);

    while ((r = read(from, buffer, 1024)) > 0)
    {
        if (from == -1 || r == -1 || to == -1 || (w = write(to, buffer, r)) == -1)
        {
            dprintf(STDERR_FILENO, "Error: Can't copy from %s to %s\n", argv[1], argv[2]);
            free(buffer);
            exit(99);
        }
    }

    free(buffer);
    close_file(from);
    close_file(to);

    return 0;
}
