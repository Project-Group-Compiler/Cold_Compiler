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
// int f5(int x){
//     int *ptr = &x;
//     x=7;
//     return *ptr;
// }
// #include<stdio.h>
// #include<stdlib.h>
// #include <cstdio>
// #include <iostream>  
// void f6(){
//     int x = 5, y = 10, z = 10;
//     int *ptr = &y;
//     int *ptr2 = &x;
//     z = *ptr - *ptr2;
//     printf("%d\n",z);
//     z = (*ptr) * (*ptr2);
//     printf("%d\n",z);
//     z = (*ptr) / (*ptr2);
//     printf("%d\n",z);
//     z = (*ptr) % (*ptr2);
//     printf("%d\n",z);
// }

// void f7(){
//     int x = 5, y = 10, z = 10;
//     int *ptr = &y;
//     int *ptr2 = &x;
//     z = (*ptr) | (*ptr2);
//     printf("%d\n",z);
//     z = (*ptr) & (*ptr2);
//     printf("%d\n",z);
//     z = (*ptr) ^ (*ptr2);
//     printf("%d\n",z);
//     z = (*ptr) >> (*ptr2);
//     printf("%d\n",z);
//     z = (*ptr) << (*ptr2);
//     printf("%d\n",z);
// }

void f8(){
    int x = 5, y = 10, z = 10;
    int *ptr = &y;
    int *ptr2 = &x;
    z = x && y;
    printf("%d\n",z);
    z = x || y;
    printf("%d\n",z);
    z = !x;
    printf("---%d\n",z);
    z = (*ptr) && (*ptr2);
    printf("%d\n",z);
    z = (*ptr) || (*ptr2);
    printf("%d\n",z);
    z = !(*ptr);
    printf("%d\n",z);
}

void f9(){
    int x = 5, y = 10, z = 10;
    int *ptr = &y;
    int *ptr2 = &x;
    z = (*ptr) < (*ptr2);
    printf("%d\n",z);
    z = (*ptr) > (*ptr2);
    printf("%d\n",z);
    z = (*ptr) <= (*ptr2);
    printf("%d\n",z);
    z = (*ptr) >= (*ptr2);
    printf("%d\n",z);
    z = (*ptr) == (*ptr2);
    printf("%d\n",z);
    z = (*ptr) != (*ptr2);
    printf("%d\n",z);
}

void f10(){
    int x = 5, y = 10, z = 10;
    int *ptr2 = &x;
    int arr[5] = {1,2,3,4,5};
    arr[2] = arr[1] + arr[3];
    printf("%d\n",arr[2]); 
    arr[2] = arr[1] - arr[3];
    printf("%d\n",arr[2]);

    printf("%p\n",ptr2);
    ptr2 = ptr2 + 1;
    printf("%p\n",ptr2);
    ptr2 = ptr2 - 1;
    printf("%p\n",ptr2);
}

void f11(){
    int x = 5, y = 10, z = 10;
    int *ptr = &y;
    int *ptr2 = &x;
    z = -(*ptr);
    printf("%d\n",z);
    z = +(*ptr);
    printf("%d\n",z);
}

int main(){
    // f6();
    // f7();
    // f8();
    f9();
    f10();
    f11();
}


// void backup(){
    // int arr[2][2] = {{10,12},{3,5}};
    // int arr[1][1] = {{10},{3}};
    // int x = arr[0][0];
    // printf("%d\n",x);

    // int arr[2][2][3];
    // int x = arr[1][1][2];


    // int x = 5,a=1,b=2;
    // int c=a+b;
    // int *ptr1=&x;
    // int *ptr2=&a;
    // int *ptr3=&c;
    // printf("%d\n",c);
    // printf("%d\n",c);
    // *ptr1=*ptr2+*ptr3+*ptr1;
    // printf("%d\n",x);
    

    // int **ptr2 = &ptr;
    // **ptr2=6;
    // printf("%d\n",**ptr2);
    // printf("%d\n",*ptr);
    // printf("%d\n",x);

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
// }