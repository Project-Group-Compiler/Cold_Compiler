float static a = 1.0f;

int foo(int y)
{
    float static a = 2.0f;
    a = 8;
}

int g()
{
    a -= 1;
}

float h(char a)
{
    int x = foo(g());
}

char fj(int a)
{
    return 'a';
}

char *hi()
{
    return "hello";
}

int *bye()
{
    int x = 0;
    int *p = &x;
    return p;
}

int boo(int, int)
{
}

int main()
{
    char* b;
    int a=(int)b;

    // int a;
    // int *y;
    // float i = h((char)foo(g()));

    // char x = fj((int)(h('9')));
    // char z;
    // malloc(foo(3));

    // foo(abs(8));
    // foo(printf("hello"));
    // printf(hi());

    // printf(foo(3), 4 + 5, 5, 6); // OK
    // // printf(4, foo(3)); // not ok
    // memcpy(bye(), y, 9); // OK
    // // memcpy(y, bye(), 9); // not ok

    // fj(8);
    // a = fj(8);
    // x = fj(8);

    // all the commented lines below throw errors

    // i = hi();
    // a = (int)hi();
    // i = (float)hi();

    // i = bye();
    // a = (int)bye();
    // i = (float)bye();

    // y = bye();
    // y = (int*)foo(8);
    // y = (int*)hi();

    // boo(foo(3), 4 + 5); // ok
    // boo(7, foo(8));     // not ok
    // boo(foo(5), foo(8) + foo(9)); // not ok
}
