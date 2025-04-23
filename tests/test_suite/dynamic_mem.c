// #include <stdio.h>
// #include <stdlib.h>

void test() {
    int *ptr;

    ptr = (int *)malloc(sizeof(int));
    if (ptr) {
        *ptr = 42;
        printf("[test] Successfully allocated memory. Value set to: %d\n", *ptr);
        free(ptr);
        printf("[test] Memory freed.\n");
    } else {
        printf("[test] Memory allocation failed.\n");
    }
}

int main() {
    printf("[main] Running dynamic memory allocation test...\n");
    test();
    return 0;
}
