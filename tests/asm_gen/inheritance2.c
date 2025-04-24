// SEMNATICS WORKING

class Person { 
    // data members of person 
     public{
         int aadhar_number;
         int c0;
     };  
     private{
         int name;
         int c1;
     }; 
     public{ 
         char * returnName()  {
            (*this).c0=0;
            (*this).c1=1;
            printf("c0 :%d\n", this->c0);
            printf("c1 :%d\n", this->c1);
            (*this).name=2;
                printf("%d\n", (*this).name);
                return (*this).name; 
         } 
     };
 }; 
 
 class Faculty <- Person { 
    // data members of Faculty
     protected{
         int employeeID;
     };  
     private{
        int c4;
     };
     public{
        int c3;

         int getEmployeeID(){ 
            (*this).c0=0;
            this->c4=4;
            printf("c0 :%d\n", (*this).c0);
            printf("c4 :%d\n", (*this).c4);
            this->returnName();
          return (*this).employeeID; 
         }
     }; 
 }; 
   
 class Student <- Person { 
    // data members of Student 
    private {
        int rollNo;
    };
     public{ 
         int getRollNo(int x) { 
            int i=x;
             (*this).rollNo=(*this).aadhar_number; // or should it be (*this).aadhar_number??  2nd option seems better   -> throws error works fine
             (*this).returnName();//till now we created object and then called function -> *this does the same thing
             //ofc there should be errors on using private vars
            printf("%d\n", (*this).rollNo);
            printf("local variable %d\n", i);
             return (*this).rollNo; 
         } 
     };
 }; 
   
 class Instructor <- Faculty  {//need to flag errors for multiple inheritance as it is not allowed
     private{
         int maxSize ;//int maxSize =  1024; maybe we can think of flagging errors for assignment here
         int numCourses;
     }; 
     public{
        // int courses[10];
        int courses[10][20][23];
         void printCourses(int x){ 
             int i;
            //  this->numCourses=x;
             for(i=0; i<x;i++){//checking blocks
                (*this).courses[i][0][2]=i;
                printf("%d ", (*this).courses[i][0][2]);
             }
         }
     };
 }; 
   
 int main()  { 
     class Instructor inst1;
     class Student stud1;
     inst1.aadhar_number = 123456789; // offset = 20
     inst1.printCourses(2); 
     printf("%d\n", inst1.courses[0][0]);
     if(inst1.courses[1][0][2] == 1)
        printf("Hello\n");
     else
        printf("World\n");
    //  inst1.courses[0][0]=4; 
     printf("%d\n", inst1.courses[0][0]);
    printf("aadhar number is %d\n", inst1.aadhar_number);
        inst1.getEmployeeID();
        inst1.returnName();
        inst1.c3=3;
        printf("c3 : %d\n", inst1.c3);
        stud1.aadhar_number = 234; // offset = 20
        stud1.getRollNo(5);
        printf("%d\n", inst1.aadhar_number);
 } 
 
