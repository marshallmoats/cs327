#include <stdio.h>
#include <stdlib.h>



int main() {
    int x[5];
    char *p, s[100];
    long n;
    while (fgets(s, sizeof(s), stdin)) {
        n = strtol(s, &p, 10);
        x[n] = 6;
        if (n >= 5) {
            printf("hello\n");
        } else {
            printf("valid\n");
        }
    }
}
