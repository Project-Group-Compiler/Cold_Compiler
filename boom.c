static int x = 7;

int yay()
{
    x++;
    return x;
}

int foo()
{
    int y = 2 * x;
    y = y - 1;
    yay();
    return yay() + y;
}

int bar()
{
    x = x + 1;
    foo();
    return foo();
}

int main()
{
    int x = bar();
    printf("%d\n", x);
}