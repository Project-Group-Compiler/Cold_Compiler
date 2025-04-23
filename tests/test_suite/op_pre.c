int main() 
{
    int a = 0 == 1 & 2;  // Expected new output: 1 (0 == (1 & 2))
    printf("[test] a = %d\n", a);
    return a;
}

