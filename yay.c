// static int x = 7;

int yay()
{
    int x = 0;
    x++;
    return x;
}

// int foo()
// {
//     int y = 2 * x;
//     y = y - 1;
//     yay();
//     return yay() + y;
// }

// int bar()
// {
//     x = x + 1;
//     foo();
//     return foo();
// }

int main()
{
    int x = 4;
    int y;
    x++;
    y = yay();
    printf("%d %d\n", x, y);
}