#include "../include/file.h"

char *read_source_file(const char *path)
{
    char *buffer = 0;
    long length;
    FILE *file = fopen(path, "rb");

    if (file)
    {
        length = file_length(file);

        buffer = (char *)malloc(length + 1);
        if (buffer)
        {
            fread(buffer, 1, length, file);
            buffer[length] = '\0';
        }

        fclose(file);
    }

    return buffer;
}

// Note : This sets the cursor of the file to the start.
size_t file_length(FILE *file)
{
    long length;

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    return length;
}
