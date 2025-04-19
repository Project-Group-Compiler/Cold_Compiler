// void f(){
//     int x = 5, y = 10;
//     int *ptr = &x;
//     int *ptr2 = &y;
//     ptr2 = &(*ptr); 
//     y = *(&(*ptr));
//     printf("%d\n",x);
//     *(&(*ptr)) = 10;
//     printf("%d\n",y);
//     printf("%d\n",x);
// }


// int f2(){
//     int x = 5;
//     int *ptr = &x;
//     printf("%p\n",ptr);
//     ptr = ptr + 1;
//     printf("%p\n",ptr);
//     // *ptr = 5;
//     ptr = ptr - 1;
//     printf("%d\n",x);
//     printf("%p\n",ptr);
//     *ptr = 6;
//     printf("%d\n",x);
// }

// int f3(){
//     int x = 10,y;
//     int arr[5];
//     arr[2] = 1;


    // printf("%d\n",x);

    // printf("%p\n",arr);
    // printf("%p\n",arr+1);
    // y = x + 10;
    // printf("%d\n",y);
    // arr[0] = 1;
    // printf("%d\n",x);
    // x = arr[0];
    // printf("%d\n",x);

    // int arr[5];
    // int x = 10;
    // // printf("%d\n",x);
    // arr[2] = 1 + 2;
    // // printf("%d\n",x);
    // x = arr[2];
    // printf("%d\n",x);

    // arr[0] = arr[1] + arr[2];
    // arr[0] = arr[1] = arr[2] = 3;
    // int *ptr = arr;
// }

// void f4(){
//     int arr[3]={10,20,31};
//     int x=arr[1];
//     printf("%d\n",x);
// }

int main(){
    // int arr[2][2] = {{10,12},{3,5}};
    // int arr[1][1] = {{10},{3}};
    // int x = arr[0][0];
    // printf("%d\n",x);

    int arr[2][2][3];
    int x = arr[1][1][2];


    // f();
    // f2();
    // f3();
    // const int x = 5;
    // int arr[x]={1,2,3};


    // int y=arr[2];
    // arr[2] = 100;
    // printf("%d\n",y);
    // y = arr[2];
    // printf("%d\n",y);
    // arr[0]=2;

    // int y;
    // int *ptr;
    // int *ptr2;
    // int arr[5];
    // arr[2] = 4;
    // // y = arr[0];
    // ptr = arr;
    // ptr2 = &arr[0];
    // printf("ptr %d\n",arr[2]);
    // printf("ptr %p\n",ptr2);
    // printf("%d\n",y);
}