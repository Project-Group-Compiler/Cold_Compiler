enum COLOR
{
    R, G, B
};

enum ErrorCode
{
    X = 205,
    Y = 101
};

int main()
{
    enum COLOR myColor1 = G;
    enum ErrorCode error1 = X;
    enum ErrorCode error2 = Y;

    int x = G + 5;
    enum COLOR myColor1 = 75;

    return 0;
}