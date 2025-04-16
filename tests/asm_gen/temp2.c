int f(){
    int a,b;
    ++a;
    b = a++;
    --b;
    b--;
    if(--b){

    }else if(b--){

    }else if(a++){

    }else if(++a){

    }
}

// struct node{
//     int x;
//     int y;
// };

// class cls{
//     public{
//     int x;
//     };
// };


// void f(){
//     // int x;char y;float z; struct node w; class cls v;
//     // int a[2] = {1+2,y};
//     // char b[2] = {'a','b'};
//     // float c[2] = {1.0,1};
//     // struct node d[2] = {w,w};
//     // class cls e[2] = {v,v};
//     // a[1] = x;
//     // b[1] = 'c';
//     // c[1] = z;
//     // d[1] = w;
//     // e[1] = v;
    
//     // x = a[1];
//     // y = b[1];
    
//     int *p; char *q;
//     struct node* r; class cls *s;
//     p = p + 1;
//     q = q - 1;
//     r = r - 1;
//     r -= 1;
//     s += 1; 
// }

// int main(){
//     int x = 0;
//     int y = 5;
//     int z = 10;
//     y = z + x;
//     x = z + y;
//     z = y + x;
//     x = y + x;
//     y = y + z;
//     z = y + x;
//     y = y + z;
// }

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