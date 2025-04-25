
struct hhh{
    int a[10];
    int b[2][3];
};
struct D{
    int cda;
    struct hhh a;
}
class A{
    public{
        int z[3];
        int a[2][1];
        void testing(){
            int b[2][3]={{1,2,3},{4,5,6}};
            this->a[0][0]=b[0][1];
            this->a[1][0]=b[1][2];
            printf("%d\n",this->a[0][0]);
        }
    };
};
int main(){
    // int b[3][2];


    // int x;
    struct hhh h1;
    class A h2;
    struct D d;
    // int *ptr;
    // // ptr = a+1;
    // ptr = h1.a + 1;
    // // ptr = h2.a + 1;

    // h2.a[1][1] = 11;
    // x = h2.a[1][1];
    // printf("%d\n",x);
    // h1.a[1][1][1] = 12;
    // x = h1.a[1][1][1];
    // printf("%d\n",x);


    // x = b[1][1];

    int x;
    int a[3] = {3,2,4};
    int *ptr;
    h2.testing();
    h2.a[1][0]=3;
    printf("Checking class arrays multidim %d\n",h2.a[0][0]);
    printf("%d\n",h2.a[1][0]);
    h1.b[1][2]=4;
    printf("Checking class struct multidim %d\n",h1.b[1][2]);

    d.a.b[1][2]=5;
    printf("checking nested struct arrays %d\n",d.a.b[1][2]);
    for(x=0;x<3;x++){
        h1.a[x]=x;
    }
    ptr = a+1;
    h2.z[0]=*ptr+2;
    printf("pointer arithmetic with class array members %d\n",h2.z[0]);
    printf("%d\n",*ptr);
    ptr=h1.a+1;
    printf("%d\n",*ptr);
    // x = *(a);
    // printf("%d\n",x);
    x = *(2+a);
    *a = x;
    printf("don't know %d\n",*a);
    // x = *(a+2);
    // printf("%d\n",x);

}


/*Things I am going to handle
ptr = arr
ptr = 1+arr ->done
 //valid but 1-arr not 

*/

/*
ptr = arr
ptr = arr + 10
x = *arr
x = *(arr+10)
*arr = 5


casting can help
For multidim
v[2][3]
x = *(*(v+1)+2)

*/