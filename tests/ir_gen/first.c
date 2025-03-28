// void print(int n1, int n2) {
    
// }
// void print2(int n1, int n2, int mx) {
    
// }

// void Test1() {
//     int a = 5;       
//     int b = 3;       
//     int c = 2;       

    
//     int shiftLeft = a << 1;       
//     int shiftRight = b >> 1;      
//     int qeq = (a == b); 
    
//     int andResult = a & c;        

    
//     int xorResult = a ^ b;        

    
//     int orResult = a | b;         

    
//     int complexResult = (a << 1) & (b | c) ^ (a >> 1);

// }


// void Test2() {
//     int a = 6;
//     int b = 3;
//     int c = 2;
//     int result1, result2, result3, result4, result5;

    
//     result1 = a * b + c;    
//     result2 = a / b;    
//     result3 = a % c;    

    
//     result4 = a + b;    
//     result5 = a - c;    

// }

// void Test3(){
//     int a = 5;
//     // int b = -(int)a;  
//     int b = -a;  
//     int c = sizeof(a);    
//     ++a;                  
//     --b;                  
// }

// void Test4(){
//     int n1, n2, max;
    // print(n1,n2&n2);
    // max = (n1 > n2) ? n1 : n2;
    // n1 ++;
    // while (1) {
    //     if (max % n1 == 0 && max % n2 == 0) {
    //         // print2(n1, n2, max);
    //         break;
    //     }
    //     ++max;
    // }
// }

void Test_5(){
    int a,b,x,y,z,c=10,f = 5;;
    if ((x && y) && (a && b)) { 
        x = 1;
    }
    if ((x && y) || (a && b)) { 
        x = 1;
    }


    x = y + z;
    a = 5, b = 10, c = a + b;
}

void Test5(){
    int x = 5;
    
    if (x > 0) {
        x = x + 1;
    }

    if (x > 10) {
        x = x - 1;
    } else {
        x = x + 2;
    }

    // switch (x) {
    //     case 1:
    //         x = x * 2;
    //         break;
    //     case 2:
    //         x = x * 3;
    //         break;
    //     default:
    //         x = x + 5;
    // }
}

void Test6(){
    int i = 0,j,k;

    while (i < 5) {
        i++;
    }

    do {
        i--;
    } while (i > 0);

    for (j = 0; j < 5; j++) {
        i += j;
    }

    for (k = 0; k < 5; k += 2) {
        i -= k;
    }

    return i;
}
// void Test7(){
//     int y = 10;
//     int *x = &y;
// }

// int x;
// int arr[10];
// int func(int a, int b);
// int *ptr;
// int matrix[5][5];
// void foo();


int add(int a, int b) {
    return a + b;
}

void greet() {
    add(1,2);
}

// multiply(int x, int y) {
//     return x * y;
// }

// square(x) {
//     return x * x;
// }

// void testGoto() {
//     int x = 5;
//     goto label;
//     x = 10;
// label:
//     printf("Value of x: %d\n", x);
// }


int main() {
    int x;
    int v[4][5];
    x = v[3][4];


    // int a,b,c,d,x,y,z;
    // int x = 6;
    // int z = x,y = 10;
    // int y = x+6;

    // y += 5;
    // x = (a > b) ? c : d;

    // foo();
    // Test1();
    // Test2();
    // Test3();
    // Test4();
    return 0;
}