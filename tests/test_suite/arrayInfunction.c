// int f(int *a){
//     int *ptr = a;
//     int x = a[0];
//     printf("%d\n",a[1]);
//     printf("%d\n",*(ptr+2));
//     printf("%d\n",x);
//     *ptr = 3;
//     *(ptr+1) = 4;
//     return 0;
// }

// int main(){
//     int a[3] = {10,11,111};
//     f(a);
//     printf("%d\n",a[0]);
//     printf("%d\n",a[1]);
//     printf("%d\n",a[2]);
//     return 0;
// }

int f(int **a){//Working
    int x = a[1][1];
    a[0][0] = 10;
    printf("%d\n",x);
    return 0;
}

int main(){
    int a[2][2] = {{1,2},{3,4}};
    f(a);
    printf("%d\n",a[0][0]);
    return 0;
}


// int f(int *a){
//     *a = 10;
//     return *a;
// }

// int main(){
//     int a = 5;
//     int *ptr = &a;
//     int y = f(ptr);
//     printf("%d\n",a);
//     printf("%d\n",y);
// }

