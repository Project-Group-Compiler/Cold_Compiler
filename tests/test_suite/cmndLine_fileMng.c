
void fileMng() {
    FILE *file;

    file = fopen("test.txt", "w");
    if (file) {
        fprintf(file, "Hello, File!");
        printf("[fileMng] Wrote to test.txt\n");
        fclose(file);
        printf("[fileMng] File closed.\n");
    } else {
        printf("[fileMng] Failed to open file for writing.\n");
    }
}



int main(int argc, char* argv[]) {  // TODO: FIX 
    char *x;
    
    printf("[main] argc = %d\n", argc);
    if (argc > 2) {
        x = argv[2];
        printf("[main] argv[2] = %s\n", x);
    } else {
        printf("[main] Not enough arguments. Please provide at least 3.\n");
    }

    fileMng();

    return 0;
}
