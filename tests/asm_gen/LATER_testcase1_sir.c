// int printf(char *, ...); // not needed

int glb;
int glb1 = 10;

static int glb2;
struct foo
{
    int a;
    int b;
    struct foo *next;
};

int bar(int a, float b) // no need to comment static
{
    static int lcl = 10;
    while (b)
    {
        b-=1; // for float, b-- won't work, instead use b -= 1
    }
    if (a == 0)
        return 0;
    bar(--a, a + b + lcl); // what if I use a++; (a++ also OK)
    return 1;
}

int main()
{
    int a = 100;
    int b[20];
    int ret;
    int i; // declarations must come before function call in a scope as per Cold grammar
    int *c;
    struct foo newS[5];
    for (i = 0; i < a; i++)
    {
        int b = 0;
        b += i;
        if (b % 2 != b % 3)
            printf("%d\n", a + b);
        else if (b % 2 == 0)
        {
            switch (b)
            {
            case -100:
                printf("1\n");
                break;
            case 1000:
                printf("2\n");
            default:
                printf("2\n");
            }
        }
        ret = i;
        // b[19] = i; // can't be inside loop since b is int inside
    }
    b[19] = i; // no need to comment ... if put outside loop since b is array outside not inside
    // c = b * ret; // error since incompatible multiplication of int* with int
jump:
    newS[3].b=100;
    printf("Final = %d", newS[3].b + ret);
    if (*(b + 2) == 0)
        goto jump;
    bar(1, (float)*b); // need to explicitly typecase function arguments
    return 0;
}