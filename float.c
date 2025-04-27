// int f(){
//     float f =  1.5;
//     float *p = &f;
//     float *q = (p+1);

// }

// int f2(){
//     float g = 1.4;
//     float f[3] = {1.2, 1.3, g};
   
//     // f[1]= 1.0;
//     // f[2]= 1.2;
//     g = f[1];
//     printf("%f\n", g);
//     printf("%f\n", f[2]);
// }

// void f3(){
//     float x;
//     float v2[2][3] = { {1.2, 2.1, 2.3}, 
//                         {3.2, 4.5, 0.8} };
//     // float v2[2][3];

//     // // printf("%d\n", v2[0][2]);

//     // v2[0][0] = 42.22;
//     x = v2[0][0];
//     // x = 1.2;

//     printf("%f\n", x);
    
//     printf("%f\n", v2[0][1]);
// }

// void f4(){
//     char g = 'c';
//     char f[3] = {'a','b', g};
//     // char f[3];
//     // f[1]= 'd;
//     // f[2]= 'f;
//     // f[1] = 'e';
//     // f[2] = 'y';
//     g = f[1];
//     printf("%c\n", g);
//     printf("%c\n", f[2]);
// }

void f5(){
    // char g = 'c';
    char g;
    char f[2][2] = {{'a','b'},{'c','d'}};
    // char f[1][1];
    f[0][0] = 'w';
    g = f[0][0];
    g = f[1][1];

    printf("%c\n", f[0][1]);
    printf("%c\n", g);
}


int main() {
    // f4();
    f5();
    // f3();
    // f2();
    // int i[5]= {1, 2, 3, 4, 5};
    // float f[5]= {1.0, 2.0, 3.0, 4.0, 5.0};
    // float g = f[0];
    // float* ptr;
    // int j = i[0];
    // i[0]= 1;
    // f[0]= 1.0;
    // g= f[0];
    // printf("%d\n", j);
    // printf("%f\n", g);
    return 0;
}
