#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct {
    int x;
    int y;
    char *name;
} Point;

int main(void) {
    Point my_point = {0};
    my_point.x = 2;
    my_point.y = 3;
    my_point.name = malloc(5);
    strcpy(my_point.name, "test");

    Point your_point = {0};
    your_point = my_point;

    printf("my point:\n");
    printf("pointer:\t%p\n", &my_point);
    printf("x:\t%i\n", my_point.x);
    printf("y:\t%i\n", my_point.y);
    printf("name:\t");
    for (size_t i = 0; i < strlen(my_point.name); i++) {
        printf("%c", my_point.name[i]);
    }
    printf("\n\n");

    printf("your point:\n");
    printf("pointer:\t%p\n", &your_point);
    printf("x:\t%i\n", your_point.x);
    printf("y:\t%i\n", your_point.y);
    printf("name:\t");
    for (size_t i = 0; i < strlen(your_point.name); i++) {
        printf("%c", your_point.name[i]);
    }
    printf("\n\n");

    return 0;
}
