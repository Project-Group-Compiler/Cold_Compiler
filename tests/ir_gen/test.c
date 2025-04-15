int glb;
int glb1 = 10;

static int glb2;
struct foo
{
    int a;
    int b;
    struct foo *next;
};

static int bar(int a, float b)
{
    static int lcl = 10;
    while (b)
    {
        b = b - 1;
    }
    if (a == 0)
        return 0;
    bar(--a, a + b + lcl); // what if I use a++;
}

int main()
{
    int a = 100, b[20], ret, i;
    int* c;
    struct foo aa,bb;
    struct foo newS[5];
    bb=newS[2];
 
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
    }
    b[19] = i;
    c = b + ret; // change to *
jump:
    
    printf("Final = %d", newS[3].b + ret);
    if (*(b + 2) == 0)
        goto jump;
     bar(1, (float)(*b));
}