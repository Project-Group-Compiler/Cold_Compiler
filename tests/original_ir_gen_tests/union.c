// IR WORKING

struct DataS {
    int i;
    float f;
};

union DataU {
    int i;
    float f;
};

void test_union() {
    union DataU datau;
    struct DataS datas;
    datau.i = 10;
    datau.f = 2.3;

    datas.i = 10;
    datas.f = 5.3;
}
