void charMultiDim(){//TODO: FIX
    char arr[2][2] = {{'a','b'},{'c','d'}};
    char x = arr[1][0];
    printf("%c\n",x);
}
void f12(){
    char c = 'a';
    char d = 'b';
    char *ptr = &c;
    char *ptr2 = &d;
    printf("%c\n",c);
    printf("%c\n",*ptr);
    printf("%c\n",*ptr2);
    *ptr = 'c';
    printf("%c\n",c);
}

void f(){
    // char *u[] = {"Hello", "World"}; // Contact me if you are fixing this
    // char **v = {"Hello", "World"};
    // char *x[2] = {"Hello", "World"};
    // char *y = "Hello";
    // printf("[f] y = %s\n", y);
    // printf("[f] x[0] = %s\n", x[0]);    
}
//dynamic mem, var args, command line, char inside union and float, multid