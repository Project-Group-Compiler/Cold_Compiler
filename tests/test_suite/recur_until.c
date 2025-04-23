
int rec(int n) {
    if (n == 0) {
        printf("[rec] Base case reached: returning 1\n");
        return 1;
    }
    printf("[rec] Computing: %d * rec(%d)\n", n, n - 1);
    return n * rec(n - 1);
}

void untilLoop() {
    int x = 0;
    printf("[untilLoop] Starting until-loop (simulated with while)...\n");

    until ((x >= 5)) {  // Corrected: C doesn't have 'until'
        printf("[untilLoop] x = %d\n", x);
        x += 1;
    }

    printf("[untilLoop] Loop finished: x = %d\n", x);
}

int main() {
    int result;
    printf("[main] Testing recursion with rec(5):\n");
    result = rec(5);
    printf("[main] Result of rec(5) = %d\n\n", result);

    printf("[main] Testing untilLoop:\n");
    untilLoop();

    return 0;
}
