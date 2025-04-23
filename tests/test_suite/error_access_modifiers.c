class Person
{
    protected{
        int aadhar_number;
    };
};

class Employee <- Person
{
    private{
        int employeeID;
    };
    public{
        int getEmployeeID()
        {
            return this->employeeID;
        }
    };
};

int main()
{
    class Employee emp1;
    emp1.aadhar_number = 123;
}
