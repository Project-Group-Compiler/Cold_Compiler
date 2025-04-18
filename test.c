int x = 9;
void doWhile()
{
    int i = 5;
    do
    {
        i--;
        printf("%d\n", i);
    } while (i >= 0);
}
void whileLoop()
{
    int i = 10;
    printf("here\n");
    while (i >= 0)
    {
        printf("1:%d\n", i);
        if (i == 5)
            break;
        printf("2:%d\n", i);
        i--;
        printf("3:%d\n", i);
    }
}
void ifElse()
{
    if (x > 10)
        x = x + 1;
    else if (x == 10)
    {
        x = x + 2;
    }
    else
    {
        x--;
    }
    printf("%d\n", x);
}

void forLoop(int j)
{
    int i=0;
    for (i = 0; i < 5; i++)
    {
        j += j;
    }
    printf("%d\n", j);
}
int main()
{
    int a=1;
    goto Label;
    a=2;
    Label:
    printf("%d\n", a);
    printf("DO WHILE\n");
    doWhile();
    printf("WHILE\n");
    whileLoop();
    printf("IF ELSE\n");
    ifElse();
    printf("FOR LOOP\n");
    forLoop(3);
    return 0;
}