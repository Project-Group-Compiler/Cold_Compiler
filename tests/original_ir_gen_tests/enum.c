int x;

enum COLOR
{
    R,
    G,
    B
};

enum ErrorCode
{
    W,
    X = 205,
    Z,
    Y = 101,
    Q
};

int main()
{
    enum COLOR myColor1 =  1;
    enum ErrorCode error1 = X;
    enum ErrorCode error2 = 5;

    switch (myColor1)
    {
    case R:
        x = 0;
        break;
    case G:
        x = 1;
        break;
    case B:
        x = 2;
        break;
    }

    if (error1 == X)
        x++;
    else
        x--;

    return 0;
}