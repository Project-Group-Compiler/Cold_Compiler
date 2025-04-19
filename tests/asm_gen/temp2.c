void f(){
    int x = 5, y = 10;
    int *ptr = &x;
    int *ptr2 = &y;
    ptr2 = &(*ptr); 
    y = *(&(*ptr));
    printf("%d\n",x);
    *(&(*ptr)) = 10;
    printf("%d\n",y);
    printf("%d\n",x);
}

// int f2(){
//     int x;
//     int arr[5];
//     arr[2] = 1;
//     int arr[5];
//     arr[0] = 1;
//     x = arr[0];
//     printf("%d\n",x);

//     // arr[0] = arr[1] + arr[2];
//     // arr[0] = arr[1] = arr[2] = 3;
//     // int *ptr = arr;
// }

int main(){
    f();
}