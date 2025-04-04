enum COLOR
{
    R,
    G,
    B
};

enum ErrorCode
{
    X = 205,
    Y = 101
};

int x;

int main()
{

    enum COLOR myColor1 = G;
    enum ErrorCode error1 = X;
    enum ErrorCode error2 = Y;

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

    switch (B)
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