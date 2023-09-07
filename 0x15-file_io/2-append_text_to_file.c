#include "main.h"

int append_text_to_file(const char *filename, char *text_content)
{
    if (!filename)
        return -1;

    int o = open(filename, O_WRONLY | O_APPEND);
    if (o == -1)
        return -1;

    ssize_t len = (text_content) ? write(o, text_content, strlen(text_content)) : 0;
    close(o);

    return (len == -1) ? -1 : 1;
}
