
// #include <stdio.h>

int foo(char ch, float f,char c1,  char c, int x){
    char c4 = ch;
    int z=ch+f+c1+c+x;

    printf("%c %f %c %c %d %c\n", ch, f, c1, c, z, c4);
    return z;
}

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
