#include <stdio.h>

#define MAX 10

int main() {
    int i;
    // for should not be translated inside this comment: for
    printf("if and for inside string: if for\n");
    for (i = 0; i < MAX; ++i) {
        if (i % 2 == 0) {
            continue;
        } else {
            break;
        }
    }
    return 0;
}
