int b[2][3];
struct A{
    float ft;
    int a[2][3];
    char c;
};
void f(int a[2][3]){
    int b[2][3];
    int *p = a[0];
    int *q = b[0];
}
class B{
    public{
        int a[2][3];
        static char c;
        void f(int a[2][3]){
            int b[2][3];
            int *p = a[0];
            int *q = b[0];
        }
    };
};

int main(){
    int arr[5][2][3];
    int *p = arr[0][0];
    int a[2]={1,2};
}