
int g=99;

int fact(int n)
{
    int y;
    int k=n-1;
    int z=0;
    g--;
    if (n == z)
        return 1;
    y=fact(k);
    return n * y;
}

int main()
{
    int x=5;
     x=fact(x);
    printf("%d\n", x);
    return 0;
}