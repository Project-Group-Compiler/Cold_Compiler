// not working
enum Color { RED, GREEN, BLUE };

void test_enum() {
    enum Color c;

    c = GREEN;
    printf("[test_enum] Enum value assigned: c = %d\n", c);

    if (c == GREEN) {
        printf("[test_enum] c is GREEN\n");
    }
}

union Data {
    int i;
    char ch; // TODO: FIX
    float f; // TODO: FIX
};

int test_union() {
    union Data data;

    data.i = 10;
    printf("[test_union] After setting i = 10, data.i = %d\n", data.i);
    data.ch = 'j';
    printf("[test_union] After setting ch = 'j', data.ch = %c\n", data.ch);
    data.f = 3.14;
    printf("[test_union] After setting f = 3.14, data.f = %f\n", data.f);
    return 0;
}

int main() {
    printf("[main] Testing enum...\n");
    test_enum();

    printf("\n[main] Testing union...\n");
    test_union();

    return 0;
}
