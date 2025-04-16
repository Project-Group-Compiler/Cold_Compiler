// Global initialized arrays
int global_int_array[5] = {1, 2, 3, 4, 5};
char global_char_array[10] = "Array";
static int global_sta_array[3] = {0, 0, 0};
char string_lit1[] = "First string";

int main()
{
    // Static initialized arrays
    static int static_int_array[3] = {10, 20, 30};
    static char local_static_array[20] = "Local static";
    return 0;
}