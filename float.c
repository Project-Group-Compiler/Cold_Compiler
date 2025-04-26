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

int f3(){
    float x;
    float v2[2][3] = { {1.2, 2.1, 2.3}, {3.2, 4.5, 0.8} };

    printf("\n[multiDimArr] ===== Multi-Dimensional Array Test =====\n");
    printf("[multiDimArr] Initial values:\n");
    printf("    v2[0][0] = %d\n", v2[0][0]);
    printf("    v2[0][1] = %d\n", v2[0][1]);
    printf("    v2[0][2] = %d\n", v2[0][2]);

    v2[0][2] = 42.22;
    x = v2[0][2];

    printf("[multiDimArr] v2[0][2] set to 42\n");
    printf("[multiDimArr] Retrieved x = %d\n", x);

    printf("[multiDimArr] Final matrix state:\n");
    printf("    v2[0] = {%d, %d, %d}\n", v2[0][0], v2[0][1], v2[0][2]);
    printf("    v2[1] = {%d, %d, %d}\n", v2[1][0], v2[1][1], v2[1][2]);
}

int main() {
    f3();
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
