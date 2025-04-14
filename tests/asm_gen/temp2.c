int main(){
    int x = 0;
    int y = 5;
    int z = 10;
    y = z + x;
    x = z + y;
    z = y + x;
    x = y + x;
    y = y + z;
    z = y + x;
    y = y + z;
}

// void foo(int x){

// }
// foo((x > 0 ? y : z));
// int f3(){
//     int x = 0;
//     int y = 0, z = 2;
//     foo((x > 0 ? y : z));
// }

// int f() {
//     char op;
//     int a, b, c;
//     int hulu;

//     switch (op) {
//         case '+':
//             c = a + b;
//             c = 10;
//             break;
//         case '-':
//             c = a + b;
//             c = 10;
//             break;
//         default:
//             c = 11;
//             c = 9;
//     }
//     hulu = 0;
//     switch (op) {
//         case '+':
//             c = a + b;
//             c = 10;
//             break;
//         default:
//             c = 10;
//     }
// }



// // Function definitions

// void test_loops(int x,int y,int z,int b) {
//     int a;
//     // test_loops(1,1+2, a = 5,5);
//     a = (a == 5 ? (6+10):7);
//     test_loops(1,1+2, (a == 5 ? (6+10):7),4);
// }


// int func(){
//     char str1[20], str2[20];
//     int arr[5]={1,2,3,4,5};
//     int i=0;
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