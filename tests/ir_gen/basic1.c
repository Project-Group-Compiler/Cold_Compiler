int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    // Function pointer declaration
    int x,y;
    int (*operation)(int, int) = add;
    x = operation(3,4);
    
    operation = multiply;
    y = operation(3,4);

    return 0;
}

// Function definitions


// void test_loops(int x,int y,int z,int b) {
//     int a;
//     // test_loops(1,1+2, a = 5,5);
//     a = (a == 5 ? (6+10):7);
//     test_loops(1,1+2, (a == 5 ? (6+10):7),4);
// }

// void solve() {
//     int x = 0;
//     x = 10; 
//     if (x == 5)
//         x = 6;
// }

// void test_loops(int x) {
//     do {
//         x++;
//     } while (x < 5);
// }

// int func(){
//     int arr[5]={1,2,3,4,5};
//     int i=0;
//     char str1[] = "Hello";
//     char str2[] = "Hello";
//     for(i=0;i<5;i++){
//         printf("%d ", arr[i]);
//     }
//     printf("Array in reverse");
//     while(i>=0){
//         printf("%d ", arr[i]);
//         if(i==0)continue;
//         i--;
//     }
//     if (strcmp(str1, str2) == 0) {
//         return strlen(str1);
//     }
//     return 0;
// }

// int main(){
//     const int a=5;
//     static char b='c';
//     int* ptr; // pointer
//     return 0;
// }
