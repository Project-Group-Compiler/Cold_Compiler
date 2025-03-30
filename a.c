class Person 
{
    private{
        char* name;
    };
    public{
        int age;
        void ok(){}
        int returnAge(int x)  {
            int y=x;
            return (*this).age;
        }//this->age;
    
    
        void solve(){
            int z=0;
            (*this).returnAge(z);//don't call by (*this).returnAge(0) ->due to typecasting
        }
    };
      
};
int main()
{
    int c;
	class Person p1;
    int z=p1.age;
    p1.age=5;
	p1.returnAge(p1.age);//p1.returnAge(c,this) ->this to added in 3AC
    p1.solve();
	return 0;
}
// int solve(){
//   int x;
// }
// struct myStructure {
//   int myNum;
//   char myLetter;
// };

// int main() {
//   struct myStructure s1;
//   s1.myNum = 5;
//   return 0;
// }
