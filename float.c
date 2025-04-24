int main(){
    int a = 5;
    float b = 3.14;
    char c = a;
    void* x = &a;
    int* y= (int*) x;
    b = (float) c;
    printf("%d\n", a);
}