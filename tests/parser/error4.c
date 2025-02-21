int main() {
    int x = 5;
    if (x > 3) {
        printf("x is greater than 3\n");
    }

    if (x < 2) {
        printf("x is less than 2\n");
    } else {
        printf("x is not less than 2\n");
    }

    switch (x) {
        case 1: printf("x is 1\n"); break;
        case 5: printf("x is 5\n"); break;
        default: printf("x is something else\n");
    }

    if(){
        printf("This is an error\n");
    }
    
    if () { 
        printf("This is an error\n");
    }
    else {
    }

    switch () {
        case 1: printf("Error case\n"); break;
    }

    return 0;
}