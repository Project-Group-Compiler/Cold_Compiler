struct sample
{
    int a;
    int b;
};

void test1()
{
    struct sample s;
    s.c = 4; // undeclared member
}

void foo(int x)
{
    int a = x;
}

void test2()
{
    struct sample s;
    foo(s); // incompatible argument type
}

void test3()
{
    struct undef t; // undefined struct
}

struct redecl
{
    int a;
    int a; // redeclaration inside struct
};
