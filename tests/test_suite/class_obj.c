class person{
    private{
        int age;
    };
    public{
        int foo()
        {
            return 42;
        }
        void bar(int a, int b)
        {
            int c = a + b;
        }
    };
};


int main()
{	
    class person p;
    class person *ptr;
    p.bar(4, 6);
    (*ptr).foo();
    ptr->bar(4, 6);
	return 0;
}
