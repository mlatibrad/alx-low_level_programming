#ifndef CUSTOM_UTILS_H
#define CUSTOM_UTILS_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

ssize_t custom_read_file(const char *filename, size_t max_bytes);
int custom_create_file(const char *filename, char *content);
int custom_append_text(const char *filename, char *text_content);

#endif

