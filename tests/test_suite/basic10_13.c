
int glo = 7;
int count = 5;
static int glos = 9;

void staticKywrd() {
    int x = 8;
    static int count = 3;
    count += 1;
    printf("[staticKywrd] x = %d, static count = %d\n", x, count);
}

int prntfScnf(int i, char c) {
    printf("[prntfScnf] Enter an integer and a character: ");
    if (scanf("%d %c", &i, &c) != 2) {
        printf("Input error\n");
        return 1;
    }

    printf("[prntfScnf] Integer: %d\n", i);
    printf("[prntfScnf] Character: %c\n", c);
    return 0;
}

void functionCall() {
    int a = 10, b = 11;
    printf("[functionCall] Calling prntfScnf with (a+b=)%d and character 'c'\n", a + b);
    prntfScnf(a + b, 'c');
}

void gotoBrkCont() {
    int i = 10;
    printf("[gotoBrkCont] Starting loop...\n");

    while (i) {
        i -= 1;
        // break;
        if (i == 7){
            printf("Skip i = %d\n", i);
            continue;  
        }

        if (i == 5)
            break;  

        printf("[gotoBrkCont] i = %d\n", i);
    }

    goto skip;
    printf("[gotoBrkCont] This line should NOT print.\n");

skip:
    printf("[gotoBrkCont] Goto landed here successfully.\n");
}

int main() {
    printf("[main] Testing global variables: glo = %d, count = %d, glos = %d\n", glo, count, glos);

    staticKywrd();
    staticKywrd(); // Call again to see static behavior

    functionCall();

    gotoBrkCont();

    return 0;
}
