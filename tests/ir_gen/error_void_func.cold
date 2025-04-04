// void_in_calculation.cold
void empty_function() {
    // This function returns void
}

int test_void_calc() {
    int x = 10;
    
    // ERROR: Trying to use void function result in a calculation
    x = x + empty_function();
    
    // ERROR: Trying to use void function as part of expression
    return empty_function() * 5;
}