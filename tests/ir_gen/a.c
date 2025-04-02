// SEMANTICS WORKING

class Person { 
    // data members of person 
     public{
         long aadhar_number;
     };  
     private{
         char* name;
     }; 
     public{ 
         char * returnName()  { return (*this).name;} 
     };
 }; 
 
 class Faculty <- Person { 
    // data members of Faculty
     protected{
         long employeeID;
     };  
     public{
         long getEmployeeID(){ 
          return (*this).employeeID; 
         }
     }; 
 }; 
   
 class Student <- Person { 
    // data members of Student 
    private {
        long rollNo;
    };
     public{ 
         long getRollNo(int x) { 
            int i;
             (*this).rollNo=(*this).aadhar_number; // or should it be (*this).aadhar_number??  2nd option seems better   -> throws error works fine
             (*this).returnName();//till now we created object and then called function -> *this does the same thing
             //ofc there should be errors on using private vars
             return (*this).rollNo; 
         } 
     };
 }; 
   
 class Instructor <- Faculty  {//need to flag errors for multiple inheritance as it is not allowed
     private{
         char** courses;
         int maxSize ;//int maxSize =  1024; maybe we can think of flagging errors for assignment here
         int numCourses;
     }; 
     public{
         void printCourses(){ 
             int i;
             for(i=0; i<(*this).numCourses;i++){//checking blocks
                char y;
                (*this).courses[i]=&y;
             }
                 
         }
     };
 }; 
   
 int main()  { 
     class Instructor inst1;
     inst1.printCourses(); 
 } 
 