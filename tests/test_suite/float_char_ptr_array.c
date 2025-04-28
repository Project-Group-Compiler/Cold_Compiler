void f(){
    float f =  1.5;
    float g = 1.4;
    float *p = &f;
    float *q = &g;
    printf("%f\n", f);
    *p = 2.4;
    printf("%f\n", f);
    printf("%f\n", g);
    *q = *p;
    printf("%f\n", g);
}

void f4(){
    char f =  'a';
    char g = 'b';
    char *p = &f;
    char *q = &g;
    printf("%c\n", f);
    *p = 'c';
    printf("%c\n", f);
    printf("%c\n", g);
    *q = *p;
    printf("%c\n", g);
}

void f2(){
    float g = 1.4;
    float f[3] = {1.2, 1.3, g};
   
    f[1]= 1.0;
    // f[2]= 1.2;
    g = f[1];
    printf("%f\n", g);
    printf("%f\n", f[2]);
}


int main() {
    f();
    // f2();
    // f4();
    return 0;
}
