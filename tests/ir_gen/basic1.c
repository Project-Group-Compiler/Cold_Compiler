
int f() {
    char op;
    int a, b, c;
    int hulu;

    switch (op) {
        case '+':
            c = a + b;
            c = 10;
            break;
        case '-':
            c = a + b;
            c = 10;
            break;
        default:
            c = 11;
            c = 9;
    }
    hulu = 0;
    switch (op) {
        case '+':
            c = a + b;
            c = 10;
            break;
        default:
            c = 10;
    }
    hulu = 1;
    switch (op) {
        default:
            c = 10;
    }
    hulu = 2;
    switch (op) {
        case '+':
            c = a + b;
            c = 10;
            break;
        default:
            c = 10;
    }
    hulu = 3;
    switch (op) {
        case '+':
            c = a + b;
            c = 10;
            break;
    }
    hulu = 4;
    switch (op) {
        case '+':
            c = a + b;
            c = 10;
        case '-':
            c = a + b;
            c = 10;
            break;
        default:
            c = 11;
    }
    hulu = 5;

    switch (op) {
        case '+':
            c = a + b;
            c = 10;
        case '-':
            c = a + b;
            c = 10;
        default:
            c = 11;
    }
}



// Function definitions

void test_loops(int x,int y,int z,int b) {
    int a;
    // test_loops(1,1+2, a = 5,5);
    a = (a == 5 ? (6+10):7);
    test_loops(1,1+2, (a == 5 ? (6+10):7),4);
}


int func(){
    char str1[20], str2[20];
    int arr[5]={1,2,3,4,5};
    int i=0;
    for(i=0;i<5;i++){
        printf("%d ", arr[i]);
    }
    printf("Array in reverse");
    while(i>=0){
        printf("%d ", arr[i]);
        if(i==0)continue;
        i--;
    }
    if (strcmp(str1, str2) == 0) {
        return strlen(str1);
    }
    return 0;
}

