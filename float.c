    // #include <stdio.h>

    int main() {
        float f = 5.1, g=7.5;
        int i = 2;
        char c = 'a';
        float *fp = &f;
        int *ip = &i;
        char *cp = &c;
        f=*fp+f+i;
        f=*ip;
        i=*ip+1;
        c=*cp+1;
        c= c + 1;   
        *fp = i;
        *ip = 36;
        printf("f: %f, i: %d, c: %d\n", f, i, *cp);
        printf("c: %d\n", c);
        
        return 0;
    }
