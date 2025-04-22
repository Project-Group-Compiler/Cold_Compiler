int main()
{
    char c = 97;
    int i = 8;
    float f = 0.9;
    char* cp=&c;
    int* ip=&i;
    float* fp=&f;
    f=*fp+f;
    c=*cp+i;
    i = *ip+i;


    
	printf("c : %d\n", c);
	printf("i : %d\n", i);
	printf("f : %f\n", f);
    return 0;
}