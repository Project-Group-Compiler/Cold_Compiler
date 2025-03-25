struct person{
    char name[20];
    int age;
};
int age=20;
int returnAge(int x,int y){
    
    return age;

}
void solve(){
    struct person p;
    p.age = 20;   
    returnAge();
}