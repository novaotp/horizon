#include <stdio.h>
#include <stdlib.h>

#ifndef FILE_H
#define FILE_H

char *read_source_file(const char *path);
size_t file_length(FILE *file);

#endif
