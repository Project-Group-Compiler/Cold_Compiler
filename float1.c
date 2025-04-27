    // #include <stdio.h>

    int main() {
        float f = 5.5;
        int i = 2;
        char c = 'A'; // ASCII 65

        printf("==== Comparison Tests: using variables in conditions ====\n");
        i=2;
        i=2;
        i=2;
        
        // i vs f
        // if (i == f) printf("2 == 5.5 is true\n"); else printf("2 == 5.5 is false\n");
        // if (i != f) printf("2 != 5.5 is true\n"); else printf("2 != 5.5 is false\n");
        if (i < f)  printf("2 < 5.5 is true\n");  else printf("2 < 5.5 is false\n");
        if (i <= f) printf("2 <= 5.5 is true\n"); else printf("2 <= 5.5 is false\n");
        if (i > f)  printf("2 > 5.5 is true\n");  else printf("2 > 5.5 is false\n");
        if (i >= f) printf("2 >= 5.5 is true\n"); else printf("2 >= 5.5 is false\n");
        c='B';
        // // c vs f
        // // if (c == f) printf("'A' == 5.5 is true\n"); else printf("'A' == 5.5 is false\n");
        // // if (c != f) printf("'A' != 5.5 is true\n"); else printf("'A' != 5.5 is false\n");
        if (c < f)  printf("'A' < 5.5 is true\n");  else printf("'A' < 5.5 is false\n");
        if (c <= f) printf("'A' <= 5.5 is true\n"); else printf("'A' <= 5.5 is false\n");
        if (c > f)  printf("'A' > 5.5 is true\n");  else printf("'A' > 5.5 is false\n");
        if (c >= f) printf("'A' >= 5.5 is true\n"); else printf("'A' >= 5.5 is false\n");
        i=2;
        i=2;
        i=2;
        // // c vs i
        // // if (c == i) printf("'A' == 2 is true\n"); else printf("'A' == 2 is false\n");
        // // if (c != i) printf("'A' != 2 is true\n"); else printf("'A' != 2 is false\n");
        if (c < i)  printf("'A' < 2 is true\n");  else printf("'A' < 2 is false\n");
        if (c <= i) printf("'A' <= 2 is true\n"); else printf("'A' <= 2 is false\n");
        if (c > i)  printf("'A' > 2 is true\n");  else printf("'A' > 2 is false\n");
        if (c >= i) printf("'A' >= 2 is true\n"); else printf("'A' >= 2 is false\n");
        f=5.5;
        i=2;
        i=2;
        // // f vs c
        // // if (f == c) printf("5.5 == 'A' is true\n"); else printf("5.5 == 'A' is false\n");
        // // if (f != c) printf("5.5 != 'A' is true\n"); else printf("5.5 != 'A' is false\n");
        if (f < c)  printf("5.5 < 'A' is true\n");  else printf("5.5 < 'A' is false\n");
        if (f <= c) printf("5.5 <= 'A' is true\n"); else printf("5.5 <= 'A' is false\n");
        if (f > c)  printf("5.5 > 'A' is true\n");  else printf("5.5 > 'A' is false\n");
        if (f >= c) printf("5.5 >= 'A' is true\n"); else printf("5.5 >= 'A' is false\n");

        return 0;
    }
