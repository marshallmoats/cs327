#include <stdio.h>
#include <stdlib.h>



int main() {
    if (fork()) {
        printf("Hello from parent [%d - %d]\n", getpid(), getppid());
    } else {
        printf("Hello from child [%d - %d]\n", getpid(), getppid());
    }
}
