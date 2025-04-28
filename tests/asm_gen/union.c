
union Data {
    int i,j;
    int a[20];
    char* p[10];
};
union test {
    int x, y;
};

void test(){
    // A union variable t
    union test t;

    t.x = 2; // t.y also gets value 2
    printf("After making x = 2:\n x = %d, y = %d\n\n", t.x,
           t.y);

    t.y = 10; // t.x is also updated to 10
    printf("After making y = 10:\n x = %d, y = %d\n\n", t.x,
           t.y);
    return;
}
int main()
{
    union Data d;
    int x,y;
    test();
    d.i=2;
    d.j=3;
    d.a[0]=4;
    d.a[1]=5;
    d.a[2]=6;
    printf("%d %d %d %d %d\n", d.i, d.j, d.a[0],d.a[1],d.a[2]);
    x=sizeof(int);
    y=sizeof(union test);
    printf("Sizeof A: %d\n", x);
    printf("Sizeof B: %d\n", y);
    return 0;
}

// A simple union test case

