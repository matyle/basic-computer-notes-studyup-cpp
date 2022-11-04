#include "csapp.h"

void doit()
{
    Fork();
    Fork();
    printf("hello\n");
    return;
}

int main(void)
{
    doit();
    printf("hello\n");
    exit(0);
}