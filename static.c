static int glo=0;
void increment() {
    // Static variable declaration and initialization
    static int count = 0; 
    count++;
    printf("Count: %d\n", count);
}
int add(int myNumber) {
  static int total = 0;
  total += myNumber;
  return total;
}
int main() {
  int x;
    int y;
    char c;
    static float p;
    increment(); 
    increment(); 
    increment(); 
    glo=glo+5;
     add(5);
    add(2);
     add(4);
    add(9);
    glo++;

    return 0;
}

  