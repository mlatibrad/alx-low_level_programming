#include "main.h"

int create_file(const char *filename, char *text_content)
{
	if (!filename)
		return -1;

	int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return -1;

	ssize_t len = (text_content) ? write(fd, text_content, strlen(text_content)) : 0;
	close(fd);

	return ((len == -1) ? -1 : 1);
}
