int func(int x) {
    char c;
    return x * x; 
}
int func(int y) {
    return y + y; 
}
float func(float x) {
    return x / 2; 
}
int func(int x,int y) {
    return x + y;
}
int func(){
    return 0; 
}
void solve() {
    int num1 = 4;
    float num2 = 10.5;
    int z;
    char* b="jhel";
    func(num1);
    func(num2);
    func(num1, num1);
    func2(z);
}

int main() {
    solve();
    return 0;
}