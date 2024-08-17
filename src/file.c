#include "../include/file.h"

char *read_source_file(const char *path)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);

        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }

        fclose(f);
    }

    return buffer;
}
