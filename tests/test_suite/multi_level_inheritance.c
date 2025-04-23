
class Person 
{
    private{
        int age;
    };
    public{
        void setAge(int a)  
        { 
            this->age = a; 
        }
        int getAge()  
        { 
            return this->age; 
        }
    };
}; 
 
class Faculty <- Person 
{ 
    private{
        int employeeID;
    };  
    public{
        void setEmployeeID(int id)
        { 
            this->employeeID = id; 
        }
        int getEmployeeID()
        { 
            return this->employeeID; 
        }
    }; 
}; 
   
class Instructor <- Faculty  
{
    private{
         int numCourses;
    }; 
    public{
        void setCourses(int num)
        { 
            this->numCourses = num; 
        }
        int getCourses()
        { 
            return this->numCourses; 
        }
    };
}; 
   
int main()  
{ 
    class Instructor inst1;
    int age, empID, numCourses;
    inst1.setAge(25);
    inst1.setEmployeeID(101);
    inst1.setCourses(5);

    age = inst1.getAge();
    empID = inst1.getEmployeeID();
    numCourses = inst1.getCourses();

    printf("[main] Results:\n");
    printf("    Age = %d\n", age);
    printf("    Employee ID = %d\n", empID);
    printf("    Number of Courses = %d\n", numCourses);
}
