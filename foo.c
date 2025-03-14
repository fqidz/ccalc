#include <stdlib.h>
#include <string.h>

int main(void)
{
    const char *string = "1.2 3.2";
    char *end;
    double result = strtod(string, &end);
    unsigned long len = strlen(end);
    (void)len;
    (void)result;
}
