#include "stringutils.h"
#include <ctype.h>

#define MAX_DOUBLE_LENGTH 24

char* strduplicate(char *s)
{
    char* result = malloc(strlen(s)+1);

    if (result == NULL) 
    {
        fprintf(stderr, "NO MEMORY\n");
        exit(EXIT_FAILURE);
    }

    strcpy(result, s);
    return result;
}

char* double_to_string(double value)
{
    char* buffer = malloc(sizeof(char) * MAX_DOUBLE_LENGTH);

    if (buffer == NULL)
    {
        fprintf(stderr, "NO MEMORY\n");
        exit(EXIT_FAILURE);
    }

    sprintf(buffer, "%f", value);
    return buffer;
}

void trimwhitespace(char* str)
{
	char* end;

	while (isspace((unsigned char)*str))
		str++;

	if (*str == 0)
		return;

	end = str + strlen(str) - 1;

	while (end > str && isspace((unsigned char)*end))
		end--;

	*(end + 1) = 0;
}