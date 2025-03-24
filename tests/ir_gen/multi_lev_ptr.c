void test_multi_level_pointers() {
    int val = 10;
    int *ptr = &val;
    int **ptr2 = &ptr;
    int ***ptr3 = &ptr2;
    //
    if (***ptr3 == 10) {
        // Test Passed
    }

    ptr2 = &ptr;
    *ptr = 10; 
    val = *ptr;

    ptr = *ptr2;
    **ptr2 = 20;

    *ptr2 = ptr;
    
}

void pointerAdd(){ //int* + int
    int val, **ptr2;
    val = (*( *(ptr2 + 2) + 1)) + 3;
}
