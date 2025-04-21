
// // #include <stdio.h>

int foo(char ch, float f,char c1,  char c, int x){
    char c4 = ch;
    int z=ch+f+c1+c+x;

    printf("%c %f %c %c %d %c\n", ch, f, c1, c, z, c4);
    return z;
}
class B{
    public{
        int a[2][3];
        static char c;
        void f(int a[2][3]){
            int b[2][3];
            int *p = a[0];
            int *q = b[0];
        }
    };
};

int main()
{
    int x=97;

 char c = x;
 char ch = 'A'; 
 char c1 = 'B';   
 float f=1.0;
 int z=ch+f+c1+c+x;
    printf("%d\n", z);

foo(ch, f, c1, c, x);
}
/* yay.c ->

int g=99;

int fact(int n,int i,int j)
{
    return i;
}

int main()
{
    int x=5;
     x=fact(x,1,1);
    printf("%d\n", x);
    return 0;
}
*/