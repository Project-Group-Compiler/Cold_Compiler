void multiLvlPtr() {
    int val;
    int *ptr;
    int **ptr2;
    int ***ptr3;

    val = 10;
    ptr = &val;
    ptr2 = &ptr;
    ptr3 = &ptr2;

    printf("\n[multiLvlPtr] ===== Multi-Level Pointer Test =====\n");
    printf("[multiLvlPtr] Initial val = %d\n", val);
    printf("[multiLvlPtr] *ptr = %d\n", *ptr);
    printf("[multiLvlPtr] **ptr2 = %d\n", **ptr2);
    printf("[multiLvlPtr] ***ptr3 = %d\n", ***ptr3);

    ***ptr3 = 20;

    printf("[multiLvlPtr] Modified val via ***ptr3 = 20\n");
    printf("[multiLvlPtr] Final val = %d\n", val);
    printf("[multiLvlPtr] *ptr = %d, **ptr2 = %d, ***ptr3 = %d\n", *ptr, **ptr2, ***ptr3);
}

void multiDimArr() {
    int x;
    int v2[2][3] = { {1, 2, 0}, {3, 4, 0} };

    printf("\n[multiDimArr] ===== Multi-Dimensional Array Test =====\n");
    printf("[multiDimArr] Initial values:\n");
    printf("    v2[0][0] = %d\n", v2[0][0]);
    printf("    v2[0][1] = %d\n", v2[0][1]);
    printf("    v2[0][2] = %d\n", v2[0][2]);

    v2[0][2] = 42;
    x = v2[0][2];

    printf("[multiDimArr] v2[0][2] set to 42\n");
    printf("[multiDimArr] Retrieved x = %d\n", x);

    printf("[multiDimArr] Final matrix state:\n");
    printf("    v2[0] = {%d, %d, %d}\n", v2[0][0], v2[0][1], v2[0][2]);
    printf("    v2[1] = {%d, %d, %d}\n", v2[1][0], v2[1][1], v2[1][2]);
}

int main() {
    printf("[main] Starting test for multi-level pointers and multi-dimensional arrays.\n");

    multiLvlPtr();
    multiDimArr();

    printf("\n[main] All tests completed.\n");

    return 0;
}
