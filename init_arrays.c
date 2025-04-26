
// Global array with compound initializer
int global_array[5] = {10, 20, 30, 40, 50};

// Static global array with compound initializer
static int static_global_array[3] = {100, 200, 300};

void print_arrays() {
    // Static local array with compound initializer
    int i = 0;
    static int static_local_array[4] = {1000, 2000, 3000, 4000};
    
    printf("Global array: ");
    for (i = 0; i < 5; i++) {
        printf("%d ", global_array[i]);
    }
    printf("\n");
    
    printf("Static global array: ");
    for (i = 0; i < 3; i++) {
        printf("%d ", static_global_array[i]);
    }
    printf("\n");
    
    printf("Static local array: ");
    for (i = 0; i < 4; i++) {
        printf("%d ", static_local_array[i]);
    }
    printf("\n");
}

int main() {
    print_arrays();
    
    // Modify some values
    global_array[0] = 99;
    static_global_array[1] = 999;
    
    printf("\nAfter modification:\n");
    print_arrays();
    
    return 0;
}