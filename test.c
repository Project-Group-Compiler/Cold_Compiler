void multiLvlPtr()
{
    int val = 10;
    int *ptr = &val;
    int **ptr2 = &ptr;
    int ***ptr3 = &ptr2;

    ***ptr3 = 20;
    // val = ***ptr2;
    printf("val = %d\n", val);
}
int main(){

    int val = 10,x;
    int *ptr = &val;
    int **ptr2 = &ptr;
    printf("ptr2 = %p\n", ptr2);
    printf("ptr = %p\n", &ptr);
    printf("ptr = %p\n", ptr);
    x=*ptr;
    printf("x = %d\n", x);
    printf("*ptr2 = %p\n", *ptr2);
    ptr = *ptr2;
    printf("*ptr2 = %p\n", *ptr2);
    printf("ptr = %p\n", ptr);
    printf("val = %p\n", &val);
    if(ptr == &val) {
        printf("ptr contains the correct address: %p\n", ptr);
    } else {
        printf("Error: ptr contains incorrect address: %p, expected: %p\n", ptr, &val);
    }
    printf("val = %d\n", val);
    val = *ptr; 
    printf("val = %d\n", val);
    if (val == 10) {
        printf("Test passed\n");
    }
    multiLvlPtr();
}