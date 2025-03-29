class Person 
{
    public{
        int age;
        char* name;
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
    p1.age=5;
	p1.returnAge(c);//p1.returnAge(c,this) ->this to added in 3AC
    p1.solve();
	return 0;
}
