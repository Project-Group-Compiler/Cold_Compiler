// int printf(char *, ...); // not needed
 
class check
{
    private{ // need access specifier with each member
        int a;
    };
    public{
    void setA(int a){
        this->a = a;
    } // need to define function
    };

    protected{
        int b;
    };
};

union Data
{
    int intValue;
    float floatValue;
    char strValue[50];
};
struct foo
{
    int a;
    int b;
    struct foo *next;
};
void setA(int a)
{

}

// class newCheck : public check // inheritance mode is always public.
// typedef syntax :
typedef class newCheck <- check
{
    private{  // need access specifier with each member
        int c;
    };
} newType;

// typedef class newCheck newType;

int main()
{
    // No new, use malloc and write 'class' while declaring a variable
    // class check *temp = new class check();
    // class newCheck *temp1 = new class newCheck();
    // class newType *temp2 = new class newCheck();

    class check *temp = (class check *)malloc(sizeof(class check));
    class newCheck *temp1 = (class newCheck *)malloc(sizeof(class newCheck));
    class newType *temp2 = (class newType *)malloc(sizeof(class newType));

    union Data *temp3 = (union Data *)malloc(sizeof(union Data));//not sir's code
    // int *temp4 = (struct foo *)malloc(sizeof(struct foo));//not sir's code ->gives error
    // temp->b = 6;

    // void (*func_ptr)(int); // func_ptr not implemented
    int arr[10][20][30];
    // func_ptr = &(setA); // func_ptr not implemented
    // func_ptr(5); // func_ptr not implemented
    union Data data; // need to write 'union' while declaring a variable
    data.intValue = 2006;
    data.floatValue = 2.2;
    ***(arr + 2) = 20;
}