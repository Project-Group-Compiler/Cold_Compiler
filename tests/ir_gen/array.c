
void testArray() {
    int arr[5];  
    int x;
    int arr1[3] = {1, 2,3};

    arr[0] = 10;
    arr[1] = 20;

    arr[2] = arr[0] + arr[1];  

    x = arr[2];
}

int main() {
    testArray();
    return 0;
}
