
class person {
    private {
        int age;
    };
    public {
        int foo() {
            printf("[foo] Called foo(), returning 42\n");  // Expected: Called foo(), returning 42
            return 42;
        }
        void bar(int a, int b) {
            int c = a + b;
            printf("[bar] Called bar(%d, %d), sum = %d\n", a, b, c);  // Expected: Called bar(4, 6), sum = 10
        }
    };
};

int main()
{	
    class person p;
    class person *ptr;
    int f;

    p.bar(4, 6);  

    ptr = &p;
    
    f = (*ptr).foo();  

    printf("[main] foo() returned: %d\n", f);  

    ptr->bar(4, 6);  

    return 0;
}
