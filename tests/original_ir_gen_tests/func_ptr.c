int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int main() {
    // Function pointer declaration
    int x,y;
    int (*operation)(int, int) = add;
    x = operation(3,4);
    operation = multiply;
    y = operation(3,4);
    return 0;
}