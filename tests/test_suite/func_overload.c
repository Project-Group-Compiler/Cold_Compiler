// Function overloading example
int func()
{
    return 0; 
}

int func(int x) 
{
    return x * x; 
}

int func(int x,int y) 
{
    return x + y;
}

int main() 
{
    int x,y,z;
    x = func();
    y = func(4);
    z = func(1, 2);
    printf("%d %d %d\n", x, y, z);
}