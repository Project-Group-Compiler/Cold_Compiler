
void foo()
{
    static int arr[3] = {0, 1, 2};
    arr[0]++;
    printf("arr : %d %d %d\n", arr[0], arr[1], arr[2]);
}

int main()
{
    foo();
    foo();
    foo();

    return 0;
}