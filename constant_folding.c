int main()
{
    float e = 3.14;
    float d, g = 1.9 , h = 9.1;
    d=sin(e);
    printf("%f\n", d);
    d=cos(e);
    printf("%f\n", d);
    d=tan(e);
    printf("%f\n", d);
    d=log(1.0);
    printf("%f\n", d);
    d=exp(e);
    printf("%f\n", d);
    d=sqrt(e);
    printf("%f\n", d);
    
    g = h + 2.4;
    printf("%f %f\n", g, h);
    h = g - 7.0;
    printf("%f %f\n", g, h);
    g = g * 4.3;
    printf("%f %f\n", g, h);
    h = h / 8.1;
    printf("%f %f\n", g, h);
}