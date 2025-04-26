    // #include <stdio.h>

int main() {
    float f = 5.1, g=7.5, h=7.3;
    // int i = '2';
    // char c = 'a';
    char d='z';
    // char e = 'b';
    float *fp = &f;
    float *gp = &h;
    // int *ip = &i;
    char *cp = &d;
    // char *dp=&e;
    // f=*fp+f+i;
    // f=*ip;
    // i=*ip+1;
    // c=*cp+1;
    d= *cp;
    // g= *gp;
    printf("f: %c, i: %f\n", *cp, *gp);
    *fp = *gp;
    *gp = g;
    // c= c + 1;   
    // *fp = g;
    // *ip = 36;
    // *cp = d;
    printf("f: %f, i: %f\n", *fp, *gp);
    // c=*cp;
    // d = *dp;
    // printf("i: %d, c: %d\n", c, d);

    // d= *cp;
    // *cp = *dp;
    // printf("i: %d, c: %d\n", c, d);
    // printf("c: %d\n", c);
    
    return 0;
    }
