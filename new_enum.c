// IR WORKING

// COLOR enums in symbol table
// R, G, B global with enum flag
// initialized R, G, B at time of reduction with right values
/*
During reduction
-> vector enumlist
-> initialize
-> R-> gloal int -> enum flag (handle as constant )
-> enum type variables (or maybe put as int) in symbol table (mycolor1)
-> intially treat enum as integer with flag in symbol table and finally change

*/

enum COLOR
{
    R,
    G,
    B
};

enum ErrorCode
{
    X = 205,
    Y,
    Z = 890,
    A,
    B = 90
};

int main()
{
    enum COLOR myColor1 = G;
    enum ErrorCode error1 = 9;
    R = 78;
    return 0;
}