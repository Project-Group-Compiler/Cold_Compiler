#include <iostream>

int main() {
    int value = 10;    // Original integer
    int *x = &value;   // Pointer to int
    int **ptr = &x;    // Pointer to pointer

    // 3AC Representation
    int *t1 = *ptr;  // Get the address stored in x (pointer to int)
    int t2 = 5;      // Load immediate value 5 into t2
    *t1 = t2;        // Store t2 (5) at the memory location pointed to by t1

    // Output the updated value
    std::cout << "Updated value = " << value << std::endl;

    **ptr = 12;  // Assign 5 to the integer via double pointer

    std::cout << "After: value = " << value << std::endl;

    return 0;
}
