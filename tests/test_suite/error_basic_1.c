void f()
{
    int a = 1;
    int a = 2; // variable redeclaration
}

void g()
{
    char ch = 'a';
}

void g() // function redeclaration
{
    int x = 3;
}

void h()
{
    int a = b + c; // undeclared variables
}

void test_args(int x)
{
    int a = x;
}

void error_args()
{
    test_args(1, 2); // incorrect no of arguments
}

void undeclared_func()
{
    random_func(); // undeclared function
}
