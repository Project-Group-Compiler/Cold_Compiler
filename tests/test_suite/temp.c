
// int main(){
//     int a = 10;
//     int *ptr = &a;
//     int d = *ptr;
//     int b = 0;
//     int c = 11;
//     c = d + 0;
//     printf("%d\n",c);
//     c = d - 0;
//     printf("%d\n",c);
//     c = d * 1;
//     printf("%d\n",c);
//     c = d ^ 0;
//     printf("%d\n",c);
//     c = d | 0;
//     printf("%d\n",c);
// }

// void arith_log()
// {
//     int a = 6;
//     int b = 3;
//     int c = 2;
//     int res1, res2, res3, res4, res5;

//     ++a;
//     c += 7;
    
//     res1 = a * b + c;
//     res2 = a / b;
//     res3 = (a << 1) & (b | c) ^ (a >> 1);

//     printf("arith_log results: res1=%d, res2=%d, res3=%d\n", res1, res2, res3);
// }

// void ifElse(){
//     int x = 11; // Initialize x for testing
//     if (x > 10)
//         x = x - 1;
//     else if(x == 10){
//         x = x + 2;
//     }else{
//         x--;
//     }
//     printf("ifElse result: x=%d\n", x);
// }

// void forLoop(){
//     int j;

//     for (j = 0; j < 5; j++)
//     {
//         j += j;
//     }
//     printf("forLoop result: j=%d\n", j);
// }

// void whileLoop(){
//     int i = 10;

//     while(i >= 0){
//         if(i == 5) break;
//         i--;
//     }
//     printf("whileLoop result: i=%d\n", i);
// }

// void doWhile(){
//     int i = 10;
//     do{
//         i--;
//     }while(i >= 0);
//     printf("doWhile result: i=%d\n", i);
// }


int main() {
    // printf("Testing arith_log:\n");
    // arith_log();

    // printf("\nTesting ifElse:\n");
    // ifElse();

    // printf("\nTesting forLoop:\n");
    // forLoop();

    // printf("\nTesting whileLoop:\n");
    // whileLoop();

    // printf("\nTesting doWhile:\n");
    // doWhile();
    int a = 5;
    int b = 16;
    int *ptr = &a;
    // int b;
    b = a * b;
    b = a*15;
    b = 2 * a;
    b = a/8;
    b = 8/a;
    return 0;
}





